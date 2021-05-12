#include "storage/storage.h"
#include "storage/file_ops_phylum.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

#include "hal/flash.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

PhylumFlashMemory::PhylumFlashMemory(DataMemory *target) : target_(target) {
}

size_t PhylumFlashMemory::block_size() {
    return target_->geometry().block_size;
}

size_t PhylumFlashMemory::number_blocks() {
    return target_->geometry().nblocks;
}

size_t PhylumFlashMemory::page_size() {
    return target_->geometry().page_size;
}

int32_t PhylumFlashMemory::erase(uint32_t address, uint32_t length) {
    return target_->erase(address, length);
}

int32_t PhylumFlashMemory::write(uint32_t address, uint8_t const *data, size_t size) {
    return target_->write(address, data, size, MemoryWriteFlags::None);
}

int32_t PhylumFlashMemory::read(uint32_t address, uint8_t *data, size_t size) {
    return target_->read(address, data, size, MemoryReadFlags::None);
}

int32_t PhylumFlashMemory::copy_page(uint32_t source, uint32_t destiny, size_t size) {
    return target_->copy_page(source, destiny, size);
}

namespace phylum_ops {

FK_DECLARE_LOGGER("phyops");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, Pool &pool) {
    return tl::unexpected<Error>(Error::IO);
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, fkb_header_t const *fkb, Pool &pool) {
    return tl::unexpected<Error>(Error::IO);
}

tl::expected<uint32_t, Error> MetaOps::write_modules(GlobalState *gs, fkb_header_t const *fkb, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    return tl::unexpected<Error>(Error::IO);
}

tl::expected<FileAttributes, Error> MetaOps::attributes() {
    return tl::unexpected<Error>(Error::IO);
}

bool MetaOps::read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) {
    return false;
}

DataOps::DataOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> DataOps::write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) {
    return tl::unexpected<Error>(Error::IO);
}

tl::expected<FileAttributes, Error> DataOps::attributes() {
    return tl::unexpected<Error>(Error::IO);
}

bool DataOps::read_fixed_record(DataRecord &record, Pool &pool) {
    return false;
}

FileReader::FileReader(Storage &storage, FileNumber file_number, Pool &pool) : storage_(storage), file_number_(file_number), pool_(pool) {
}

tl::expected<FileReader::SizeInfo, Error> FileReader::get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) {
    return SizeInfo{
        .size = 0,
        .last_block = 0,
    };
}

bool FileReader::decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    return false;
}

bool FileReader::seek_record(RecordNumber record, Pool &pool) {
    return false;
}

int32_t FileReader::read(uint8_t *record, size_t size) {
    return -1;
}

int32_t FileReader::read(void *record, pb_msgdesc_t const *fields) {
    return -1;
}

int32_t FileReader::get_file_size(size_t &file_size) {
    return -1;
}

} // namespace phylum_ops

} // namespace fk
