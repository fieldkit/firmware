#pragma once

#include "storage/lfs_driver.h"

namespace fk {

class Attributes {
private:
    lfs_file_config &cfg_;

public:
    Attributes(lfs_file_config &cfg) : cfg_(cfg) {
    }

public:
    uint32_t nrecords() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_FILE_ATTR_NRECORDS].buffer;
    }

    uint32_t first_record() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_FILE_ATTR_FIRST_RECORD].buffer;
    }

    uint32_t increase_nrecords() {
        auto &nrecords = *(uint32_t *)cfg_.attrs[LFS_DRIVER_FILE_ATTR_NRECORDS].buffer;
        nrecords++;
        return nrecords;
    }

    void set(uint8_t index, uint32_t value) {
        *(uint32_t *)cfg_.attrs[index].buffer = value;
    }

    uint32_t get(uint8_t index) {
        return *(uint32_t *)cfg_.attrs[index].buffer;
    }

    void first_record(uint32_t first_record) {
        set(LFS_DRIVER_FILE_ATTR_FIRST_RECORD, first_record);
    }

    void nrecords(uint32_t nrecords) {
        set(LFS_DRIVER_FILE_ATTR_NRECORDS, nrecords);
    }
};

} // namespace fk
