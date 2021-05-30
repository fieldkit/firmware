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

    auto appended = file.append_immutable(record_type, fk_data_DataRecord_fields, &record.record(), pool);
    if (appended.bytes < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    auto attributes = file.attributes();

    gs->update_data_stream(attributes.size, attributes.nrecords);

    return appended.record;
}

tl::expected<FileAttributes, Error> MetaOps::attributes(Pool &pool) {
    return FileAttributes{
        0 /* size */,
        0 /* records */
    };
}

static RecordType get_record_type(SignedRecordKind kind) {
    switch (kind) {
    case SignedRecordKind::State: return RecordType::State;
    case SignedRecordKind::Modules: return RecordType::Modules;
    case SignedRecordKind::Schedule: return RecordType::Schedule;
    default:
        break;
    }

    FK_ASSERT(0);
    return RecordType::Unknown;
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        logerror("opening file");
        return false;
    }

    file_size_t position = UINT32_MAX;
    err = file.seek_record_type(get_record_type(kind), position, pool);
    if (err < 0) {
        logerror("seeking record by type");
        return false;
    }

    if (position == UINT32_MAX) {
        loginfo("lookup %d: invalid position", (int32_t)kind);
        return false;
    }

    auto bytes_read = file.read(fk_data_DataRecord_fields, &record.for_decoding(pool), pool);
    if (bytes_read <= 0) {
        logwarn("reading record by type");
        return false;
    }

    return true;
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

    #if defined(FK_PHYLUM_AMPLIFICATION)
    auto amplification = FK_PHYLUM_AMPLIFICATION;
    #else
    auto amplification = 1;
    #endif
    auto record_number = 0;

    for (auto i = 0; i < amplification; ++i) {
        record_number = file.attributes().record_number;

        record->readings.reading = record_number;

        auto appended = file.append_always(RecordType::Data, fk_data_DataRecord_fields, record, pool);
        if (appended.bytes <= 0) {
            logerror("error saving readings");
            return tl::unexpected<Error>(Error::IO);
        }

        loginfo("wrote %zd bytes rec=(#%" PRIu32 ") (%" PRIu32 " bytes)",
                (size_t)appended.bytes, appended.record, 0);
    }

    auto attributes = file.attributes();

    gs->update_data_stream(attributes.size, attributes.nrecords);

    return record_number;
}

tl::expected<FileAttributes, Error> DataOps::attributes(Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return FileAttributes{ 0, 0 };
    }

    auto file_attributes = file.attributes();

    return FileAttributes{
        file_attributes.size,
        file_attributes.nrecords
    };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    file_size_t position = UINT32_MAX;
    err = file.seek_record_type(RecordType::Location, position, pool);
    if (err < 0) {
        logerror("seeking record by type");
        return false;
    }

    if (position == UINT32_MAX) {
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
    auto position_of_last = attributes.size;

    if (last_block != UINT32_MAX) {
        position_of_last = pdf_.seek_record(last_block, pool);
        if (position_of_last < 0) {
            return tl::unexpected<Error>(Error::IO);
        }
    }

    auto position_of_first = pdf_.seek_record(first_block, pool);
    if (position_of_first < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    if (last_block >= attributes.record_number) {
        last_block = attributes.record_number;
    }

    return SizeInfo{
        .size = position_of_last - position_of_first,
        .last_block = last_block,
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
