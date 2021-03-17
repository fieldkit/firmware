#include "storage/storage.h"
#include "storage/file_ops_lfs.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

#include "storage/record_appender.h"
#include "storage/partitioned_reader.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

namespace lfs {

/**
 * File size that triggers rolling over to a new meta file. 0 disables
 * rollover, creating one long file.
 */
constexpr size_t MetaRolloverSize = 0;

/**
 * File size that triggers rolling over to a new data file.
 */
constexpr size_t DataRolloverSize = 64 * 2048 * 5;

FK_DECLARE_LOGGER("lfsops");

static uint8_t kind_to_attribute_index(SignedRecordKind kind) {
    switch (kind) {
    case SignedRecordKind::Modules:
        return LFS_DRIVER_FILE_ATTR_CONFIG_MODULES;
    case SignedRecordKind::Schedule:
        return LFS_DRIVER_FILE_ATTR_CONFIG_SCHEDULE;
    case SignedRecordKind::State:
        return LFS_DRIVER_FILE_ATTR_CONFIG_STATE;
    default:
        FK_ASSERT(false);
        return 0;
    }
}

MetaOps::MetaOps(LfsDriver &lfs) : lfs_(lfs), map_(&lfs_, "meta", 0, *lfs.pool()) {
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, Pool &pool) {
    return write_state(gs, &fkb_header, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, fkb_header_t const *fkb, Pool &pool) {
    MetaRecord record;
    record.include_state(gs, fkb, pool);
    return write_kind(gs, SignedRecordKind::State, record, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_modules(GlobalState *gs, fkb_header_t const *fkb, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    MetaRecord record;
    record.include_modules(gs, fkb, modules, readings, pool);
    return write_kind(gs, SignedRecordKind::Modules, record, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_kind(GlobalState *gs, SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    auto index = kind_to_attribute_index(kind);
    loginfo("writing kind: %d", index);

    RecordAppender appender{ &lfs_, &map_, MetaRolloverSize, pool };
    auto appended = appender.append_changes(index, &record.record(), fk_data_DataRecord_fields, pool);
    if (!appended) {
        logerror("error appending");
        return tl::unexpected<Error>(appended.error());
    }

    gs->storage.spi.used = lfs_.used();
    gs->update_meta_stream(appended->absolute_position + appended->record_size, appended->record);

    return appended->record;
}

tl::expected<FileAttributes, Error> MetaOps::attributes() {
    auto attributes = map_.attributes(*lfs_.pool());
    if (!attributes) {
        return tl::unexpected<Error>(attributes.error());
    }
    return FileAttributes{
        .size = attributes->size,
        .records = attributes->records
    };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    loginfo("reading meta record kind=%d", kind);

    PartitionedReader partitioned_reader{ &lfs_, &map_, pool };
    auto seek = partitioned_reader.seek_via_attr(kind_to_attribute_index(kind), pool);
    if (!seek) {
        logerror("seeking via attribute");
        return false;
    }

    fk_data_SignedRecord signed_record = fk_data_SignedRecord_init_default;
    signed_record.data.funcs.decode = pb_decode_data;
    signed_record.data.arg = (void *)&pool;
    signed_record.data.funcs.decode = pb_decode_data;
    signed_record.hash.arg = (void *)&pool;

    auto reader = partitioned_reader.open_reader(pool);
    FK_ASSERT(reader != nullptr);
    BufferedReader buffered{ reader, (uint8_t *)pool.malloc(1024), 1024u };
    auto stream = pb_istream_from_readable(&buffered);
    if (!pb_decode_delimited(&stream, fk_data_SignedRecord_fields, &signed_record)) {
        logerror("read-record: decoding record");
        return false;
    }

    auto data_ref = (pb_data_t *)signed_record.data.arg;
    auto record_stream = pb_istream_from_buffer((pb_byte_t *)data_ref->buffer, data_ref->length);
    if (!pb_decode_delimited(&record_stream, fk_data_DataRecord_fields, &record.for_decoding(pool))) {
        return false;
    }

    return true;
}

DataOps::DataOps(LfsDriver &lfs) : lfs_(lfs), map_(&lfs_, "data", 0, *lfs.pool()) {
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    uint32_t record_number = UINT32_MAX;

    auto iter = pool.subpool("appending", 4096);

    RecordAppender appender{ &lfs_, &map_, DataRolloverSize, pool };
    for (auto i = 0u; i < 10; ++i) {
        auto appended = appender.append_data_record(record, *iter);
        if (!appended) {
            return tl::unexpected<Error>(appended.error());
        }

        gs->update_data_stream(appended->absolute_position + appended->record_size, appended->record);

        record_number = appended->record;

        iter->clear();
    }

    return record_number;
}

tl::expected<FileAttributes, Error> DataOps::attributes() {
    auto attributes = map_.attributes(*lfs_.pool());
    if (!attributes) {
        return tl::unexpected<Error>(attributes.error());
    }
    return FileAttributes{
        .size = attributes->size,
        .records = attributes->records
    };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    loginfo("reading known location");

    PartitionedReader partitioned_reader{ &lfs_, &map_, pool };
    auto gps_record = partitioned_reader.seek_fixed(pool);
    if (!gps_record) {
        return false;
    }

    auto reader = partitioned_reader.open_reader(pool);
    FK_ASSERT(reader != nullptr);
    BufferedReader buffered{ reader, (uint8_t *)pool.malloc(1024), 1024u };
    auto stream = pb_istream_from_readable(&buffered);
    if (!pb_decode_delimited(&stream, fk_data_DataRecord_fields, &record.for_decoding(pool))) {
        return false;
    }

    return true;
}

FileReader::FileReader(LfsDriver &lfs, FileNumber file_number, Pool &pool)
    : lfs_(lfs), file_number_(file_number), pool_(pool),
      map_(&lfs_, file_number == Storage::Data ? "data" : "meta", 0, *lfs.pool()) {
}

tl::expected<FileReader::SizeInfo, Error> FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    // If they're asking for the final record of the file, we use the
    // specific one because seek is very strict about acceptable inputs.
    if (last_block == UINT32_MAX) {
        auto attributes = map_.attributes(pool);
        if (!attributes) {
            return tl::unexpected<Error>(attributes.error());
        }
        last_block = attributes->records;
    }

    auto seek_last = partitioned_.seek(last_block, pool);
    if (!seek_last) {
        return tl::unexpected<Error>(seek_last.error());
    }

    // NOTE: We seek to the starting position in the end so that if a
    // file reader is opened it'll be opened at this location.

    auto seek_first = partitioned_.seek(first_block, pool);
    if (!seek_first) {
        return tl::unexpected<Error>(seek_first.error());
    }

    return SizeInfo{
        .size = seek_last->absolute_position - seek_first->absolute_position,
        .last_block = seek_last->record,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
    sr.data.funcs.decode = pb_decode_data;
    sr.data.arg = (void *)&pool;
    sr.data.funcs.decode = pb_decode_data;
    sr.hash.arg = (void *)&pool;

    auto nread = read(&sr, fk_data_SignedRecord_fields);
    if (nread == 0) {
        return false;
    }

    auto data_ref = (pb_data_t *)sr.data.arg;
    auto stream = pb_istream_from_buffer((pb_byte_t *)data_ref->buffer, data_ref->length);
    if (!pb_decode_delimited(&stream, fields, record)) {
        return false;
    }

    return true;
}

bool FileReader::seek_record(RecordNumber record, Pool &pool) {
    if (!partitioned_.seek(record, pool)) {
        return false;
    }
    return true;
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    return partitioned_.open_reader(pool_)->read(record, size);
}

int32_t FileReader::read(void *record, pb_msgdesc_t const *fields) {
    auto reader = partitioned_.open_reader(pool_);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fields, record)) {
        return -1;
    }
    return 0;
}

} // namespace darwin

} // namespace fk
