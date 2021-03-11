#include "io.h"
#include "storage/lfs_driver.h"

namespace fk {

class LfsReader : public Reader {
private:
    LfsDriver *lfs_{ nullptr };
    lfs_file_t *file_{ nullptr };

public:
    LfsReader(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
    }

public:
    int32_t read(uint8_t *buffer, size_t size) override {
        return lfs_file_read(lfs_->lfs(), file_, buffer, size);
    }
};

class LfsWriter : public Writer {
private:
    LfsDriver *lfs_{ nullptr };
    lfs_file_t *file_{ nullptr };

public:
    LfsWriter(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
    }

public:
    int32_t write(uint8_t const *buffer, size_t size) override {
        return lfs_file_write(lfs_->lfs(), file_, buffer, size);
    }
};

} // namespace fk
