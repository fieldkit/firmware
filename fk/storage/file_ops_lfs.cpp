#include "storage/storage.h"
#include "storage/file_ops_lfs.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

namespace lfs {

FK_DECLARE_LOGGER("lfsops");

MetaOps::MetaOps(LfsDriver &lfs) : lfs_(lfs) {
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
    return 0u;
}

tl::expected<FileAttributes, Error> MetaOps::atttributes() {
    return FileAttributes{ 0, 0 };
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    return false;
}

DataOps::DataOps(LfsDriver &lfs) : lfs_(lfs) {
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    return 0u;
}

tl::expected<FileAttributes, Error> DataOps::atttributes() {
    return FileAttributes{ 0, 0 };
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    return false;
}

FileReader::FileReader(LfsDriver &lfs, FileNumber file_number, Pool &pool) : lfs_(lfs), file_number_(file_number), pool_(pool) {
}

FileReader::SizeInfo FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    return SizeInfo{
        .size = 0,
        .last_block = 0,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    return false;
}

bool FileReader::seek_record(RecordNumber record) {
    return false;
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    return 0;
}

int32_t FileReader::read(void *record, pb_msgdesc_t const *fields) {
    return 0;
}

} // namespace darwin

} // namespace fk
