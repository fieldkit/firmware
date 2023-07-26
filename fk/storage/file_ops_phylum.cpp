#include "storage/storage.h"
#include "storage/file_ops_phylum.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

#include "hal/flash.h"
#include "storage/phylum_data_file.h"

namespace fk {

namespace phylum_ops {

FK_DECLARE_LOGGER("phyops");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
}

static RecordType get_record_type(SignedRecordKind kind) {
    switch (kind) {
    case SignedRecordKind::State:
        return RecordType::State;
    case SignedRecordKind::Modules:
        return RecordType::Modules;
    case SignedRecordKind::Schedule:
        return RecordType::Schedule;
    default:
        break;
    }

    FK_ASSERT(0);
    return RecordType::Unknown;
}

tl::expected<uint32_t, Error> MetaOps::write_record(SignedRecordKind kind, fk_data_DataRecord *record, Pool &pool) {
    return write_kind(get_record_type(kind), record, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_kind(RecordType record_type, fk_data_DataRecord *record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    auto appended = file.append_immutable(record_type, fk_data_DataRecord_fields, record, pool);
    if (appended.bytes < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    return appended.record;
}

tl::expected<FileAttributes, Error> MetaOps::attributes(Pool &pool) {
    return FileAttributes{
        0 /* size */, 0 /* records */
    };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        logerror("opening file");
        return false;
    }

    file_size_t position = UINT32_MAX;
    err = file.seek_record_type(get_record_type(kind), position);
    if (err < 0) {
        logerror("seeking record by type");
        return false;
    }

    if (position == UINT32_MAX) {
        loginfo("lookup %d: invalid position", (int32_t)kind);
        return false;
    }

    auto bytes_read = file.read(fk_data_DataRecord_fields, record.for_decoding(), pool);
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

tl::expected<uint32_t, Error> DataOps::write_readings(fk_data_DataRecord *record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    record_number_t record_number = 0;

#if defined(FK_PHYLUM_AMPLIFICATION)
    auto amplification = FK_PHYLUM_AMPLIFICATION;
#else
    auto amplification = 1;
#endif

    for (auto i = 0; i < amplification; ++i) {
        auto attributes = file.attributes();

        record_number = attributes.nrecords;
        record->readings.reading = record_number;
        loginfo("writing record=#%" PRIu32, record_number);

        auto appended = file.append_always(RecordType::Data, fk_data_DataRecord_fields, record, nullptr, pool);
        if (appended.bytes <= 0) {
            logerror("error saving readings");
            return tl::unexpected<Error>(Error::IO);
        }

        loginfo("wrote %zd bytes record=#%" PRIu32 "", (size_t)appended.bytes, appended.record);
    }

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
        file_attributes.nrecords,
        file_attributes.nreadings,
    };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    PhylumDataFile file{ storage_.phylum(), pool };
    auto err = file.open("d/00000000", pool);
    if (err < 0) {
        return false;
    }

    file_size_t position = UINT32_MAX;
    err = file.seek_record_type(RecordType::Location, position);
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

FileReader::FileReader(Storage &storage, FileNumber file_number, Pool &pool)
    : storage_(storage), file_number_(file_number), pdf_{ storage.phylum(), pool }, pool_(pool) {
}

tl::expected<FileReader::SizeInfo, Error> FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    if (file_number_ == Storage::Meta) {
        logerror("get-size(fail): Storage::Meta");
        return SizeInfo{
            .size = 0,
            .last_block = 0,
        };
    }

    if (!open_if_necessary()) {
        logerror("get-size(fail): unable to open");
        return tl::unexpected<Error>(Error::IO);
    }

    auto attributes = pdf_.attributes();
    auto position_of_last = attributes.size;

    if (last_block != UINT32_MAX) {
        position_of_last = pdf_.seek_record(last_block);
        if (position_of_last < 0) {
            logerror("get-size(fail): position-of-last < 0");
            return tl::unexpected<Error>(Error::IO);
        }
    }

    auto position_of_first = pdf_.seek_record(first_block);
    if (position_of_first < 0) {
        logerror("get-size(fail): position-of-first < 0");
        return tl::unexpected<Error>(Error::IO);
    }

    if (last_block >= attributes.nrecords) {
        last_block = attributes.nrecords;
    }

    return SizeInfo{
        .size = position_of_last - position_of_first,
        .last_block = last_block,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto nread = pdf_.read(fields, record, pool);
    if (nread <= 0) {
        return false;
    }

    return true;
}

bool FileReader::seek_record(RecordNumber record, Pool & /*pool*/) {
    FK_ASSERT(file_number_ == Storage::Data);

    if (!open_if_necessary()) {
        return false;
    }

    auto err = pdf_.seek_record(record);
    if (err < 0) {
        return false;
    }

    return true;
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    FK_ASSERT(file_number_ == Storage::Data);
    FK_ASSERT(pdf_.is_open());

    auto err = pdf_.read(record, size);
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

    if (!open_if_necessary()) {
        return false;
    }

    file_size = pdf_.attributes().size;

    return 0;
}

bool FileReader::open_if_necessary() {
    if (pdf_.is_open()) {
        return true;
    }

    loginfo("opening file");

    auto err = pdf_.open("d/00000000", pool_);
    if (err < 0) {
        return false;
    }

    return true;
}

EncodedMessage *FileReader::read_signed_record_bytes(SignedRecordKind kind, Pool &pool) {
    FK_ASSERT(file_number_ == Storage::Data);

    if (!open_if_necessary()) {
        return nullptr;
    }

    file_size_t position = UINT32_MAX;
    auto err = pdf_.seek_record_type(get_record_type(kind), position);
    if (err < 0) {
        logerror("seeking record by type");
        return nullptr;
    }

    if (position == UINT32_MAX) {
        loginfo("lookup %d: invalid position", (int32_t)kind);
        return nullptr;
    }

    EncodedMessage *bytes = nullptr;
    if (pdf_.read_delimited_bytes_into_message(&bytes, StandardPageSize, pool) < 0) {
        loginfo("read-bytes error");
        return nullptr;
    }

    return bytes;
}

} // namespace phylum_ops

} // namespace fk
