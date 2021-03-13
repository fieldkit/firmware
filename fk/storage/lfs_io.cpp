#include "storage/lfs_io.h"
#include "utilities.h"

namespace fk {

LfsReader::LfsReader(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
}

int32_t LfsReader::read(uint8_t *buffer, size_t size) {
    return lfs_file_read(lfs_->lfs(), file_, buffer, size);
}

LfsWriter::LfsWriter(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
}

int32_t LfsWriter::write(uint8_t const *buffer, size_t size) {
    return lfs_file_write(lfs_->lfs(), file_, buffer, size);
}

} // namespace fk
