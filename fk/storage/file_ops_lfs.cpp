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
    RecordAppender appender{ &lfs_, &map_, 64 * 2048 * 5, pool };
    auto appended = appender.append_changes((uint8_t)kind, &record.record(), fk_data_DataRecord_fields, pool);
    if (!appended) {
        return tl::unexpected<Error>(appended.error());
    }
    return appended->record;
}

tl::expected<FileAttributes, Error> MetaOps::attributes() {
    auto attributes = map_.attributes(*lfs_.pool());
    return FileAttributes{
        .size = attributes->size,
        .records = attributes->records
    };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    return false;
}

DataOps::DataOps(LfsDriver &lfs) : lfs_(lfs), map_(&lfs_, "data", 3, *lfs.pool()) {
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    RecordAppender appender{ &lfs_, &map_, 64 * 2048 * 5, pool };
    auto appended = appender.append_data_record(record, pool);
    if (!appended) {
        return tl::unexpected<Error>(appended.error());
    }
    return appended->record;
}

tl::expected<FileAttributes, Error> DataOps::attributes() {
    auto attributes = map_.attributes(*lfs_.pool());
    return FileAttributes{
        .size = attributes->size,
        .records = attributes->records
    };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
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

FileReader::SizeInfo FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    auto seek_first = partitioned_.seek(first_block, pool);
    FK_ASSERT(seek_first);

    auto seek_last = partitioned_.seek(last_block, pool);
    FK_ASSERT(seek_last);

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
