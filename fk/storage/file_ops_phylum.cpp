#include "storage/storage.h"
#include "storage/file_ops_phylum.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

#include "hal/flash.h"
#include "storage/phylum_data_file.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

namespace phylum_ops {

FK_DECLARE_LOGGER("phyops");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, Pool &pool) {
    return write_state(gs, &fkb_header, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, fkb_header_t const *fkb, Pool &pool) {
    MetaRecord record;
    record.include_state(gs, fkb, pool);
    return write_kind(gs, RecordType::State, record, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_modules(GlobalState *gs, fkb_header_t const *fkb, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    MetaRecord record;
    record.include_modules(gs, fkb, modules, readings, pool);
    return write_kind(gs, RecordType::Modules, record, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_kind(GlobalState *gs, RecordType record_type, MetaRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    auto record_number = file.attributes().record_number;

    err = file.append_immutable(record_type, fk_data_DataRecord_fields, &record.record(), pool);
    if (err < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    if (!storage_.phylum().sync()) {
        return tl::unexpected<Error>(Error::IO);
    }

    return record_number;
}

tl::expected<FileAttributes, Error> MetaOps::attributes() {
    return FileAttributes{
        0 /* size */,
        0 /* records */
    };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    return false;
}

DataOps::DataOps(Storage &storage) : storage_(storage) {
}

bool DataOps::touch(Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.create("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    return true;
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    auto record_number = file.attributes().record_number;

    record->readings.reading = record_number;

    auto bytes_wrote = file.append_always(RecordType::Data, fk_data_DataRecord_fields, record, pool);
    if (bytes_wrote == 0) {
        logerror("error saving readings");
        return tl::unexpected<Error>(Error::IO);
    }

    if (!storage_.phylum().sync()) {
        return tl::unexpected<Error>(Error::IO);
    }

    loginfo("wrote %zd bytes rec=(#%" PRIu32 ") (%" PRIu32 " bytes)",
            (size_t)bytes_wrote, record_number, 0);

    // gs->storage.spi.used = storage_.used();
    // gs->update_data_stream(file);

    return record_number;
}

tl::expected<FileAttributes, Error> DataOps::attributes() {
    return FileAttributes{
        0 /* size */,
        0 /* records */
    };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    err = file.seek_record_type(RecordType::Location, pool);
    if (err < 0) {
        logwarn("seeking gps/fixed record type");
        return false;
    }

    auto bytes_read = file.read(fk_data_DataRecord_fields, &record.for_decoding(pool), pool);
    if (bytes_read <= 0) {
        logwarn("reading gps/fixed record type");
        return false;
    }

    return true;
}

FileReader::FileReader(Storage &storage, FileNumber file_number, Pool &pool) : storage_(storage), file_number_(file_number), pdf_{ storage.phylum(), pool }, pool_(pool) {
}

tl::expected<FileReader::SizeInfo, Error> FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    if (file_number_ == Storage::Meta) {
        return SizeInfo{
            .size = 0,
            .last_block = 0,
        };
    }

    if (!open_if_necessary(pool)) {
        return tl::unexpected<Error>(Error::IO);
    }

    auto attributes = pdf_.attributes();

    return SizeInfo{
        .size = attributes.size,
        .last_block = attributes.record_number,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    FK_ASSERT(file_number_ == Storage::Data);
    return false;
}

bool FileReader::seek_record(RecordNumber record, Pool &pool) {
    FK_ASSERT(file_number_ == Storage::Data);

    if (!open_if_necessary(pool)) {
        return false;
    }

    auto err = pdf_.seek_record(record, pool_);
    if (err < 0) {
        return false;
    }

    return true;
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    FK_ASSERT(file_number_ == Storage::Data);
    FK_ASSERT(pdf_.is_open());

    auto err = pdf_.read(record, size, pool_);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t FileReader::read(void *record, pb_msgdesc_t const *fields) {
    FK_ASSERT(file_number_ == Storage::Data);
    FK_ASSERT(pdf_.is_open());

    auto err = pdf_.read(fields, record, pool_);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t FileReader::get_file_size(size_t &file_size) {
    FK_ASSERT(file_number_ == Storage::Data);

    if (!open_if_necessary(pool_)) {
        return false;
    }

    file_size = pdf_.attributes().size;

    return 0;
}

bool FileReader::open_if_necessary(Pool &pool) {
    if (pdf_.is_open()) {
        return true;
    }

    auto err = pdf_.open("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    return true;
}

} // namespace phylum_ops

} // namespace fk
