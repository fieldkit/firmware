#include "storage/meta_ops.h"
#include "storage/storage.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("sops");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
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
    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    auto srl = SignedRecordLog { meta };
    auto meta_record = srl.append_immutable(kind, &record.record(), fk_data_DataRecord_fields, pool);

    // Return this information and do outside.
    gs->update_meta_stream(meta);

    return (*meta_record).record;
}

tl::expected<FileAttributes, Error> MetaOps::atttributes() {
    auto file = storage_.file(Storage::Meta);
    if (!file.seek_end()) {
        return tl::unexpected<Error>(Error::IO);
    }

    return FileAttributes{ file.size(), file.record() };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    auto meta = storage_.file(Storage::Meta);
    auto srl = SignedRecordLog{ meta };
    if (!srl.seek_record(kind)) {
        return false;
    }

    if (!srl.decode(&record.for_decoding(pool), fk_data_DataRecord_fields, pool)) {
        return false;
    }

    return true;
}

DataOps::DataOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    auto file = storage_.file(Storage::Data);
    if (!file.seek_end()) {
        FK_ASSERT(file.create());
    }

    // With the file open we can assign the record number.
    auto record_number = file.record();
    record->readings.reading = record_number;

    // Write to the file.
    auto bytes_wrote = file.write(record, fk_data_DataRecord_fields);
    if (bytes_wrote == 0) {
        logerror("error saving readings");
        return tl::unexpected<Error>(Error::IO);
    }

    loginfo("wrote %zd bytes rec=(#%" PRIu32 ") (%" PRIu32 " bytes) (" PRADDRESS ") (%" PRIu32 " wasted)",
            (size_t)bytes_wrote, record_number, file.size(), file.tail(), file.wasted());

    gs->update_data_stream(file);

    return record->readings.reading;
}

tl::expected<FileAttributes, Error> DataOps::atttributes() {
    auto file = storage_.file(Storage::Data);
    if (!file.seek_end()) {
        return tl::unexpected<Error>(Error::IO);
    }

    return FileAttributes{ file.size(), file.record() };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    auto file = storage_.file(Storage::Data);
    if (!file.seek_end()) {
        FK_ASSERT(file.create());
    }

    if (!file.rewind()) {
        return false;
    }

    auto nread = file.read(&record.for_decoding(pool), fk_data_DataRecord_fields);
    if (nread <= 0) {
        return false;
    }

    return true;
}

FileReader::FileReader(Storage &storage, FileNumber file_number, Pool &pool) : storage_(storage), file_number_(file_number), pool_(pool), file_(&storage, file_number, pool) {
}

FileReader::SizeInfo FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    auto size = file_.get_size(first_block, last_block, pool);
    return SizeInfo{
        .size = size.size,
        .last_block = size.last_block,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
    sr.data.funcs.decode = pb_decode_data;
    sr.data.arg = (void *)&pool;
    sr.data.funcs.decode = pb_decode_data;
    sr.hash.arg = (void *)&pool;

    auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
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

bool FileReader::seek_record(RecordNumber record) {
    return file_.seek(record);
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    return file_.read(record, size);
}

int32_t FileReader::read(void *record, pb_msgdesc_t const *fields) {
    return file_.read(record, fields);
}

}
