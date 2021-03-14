#include "lfs_driver.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

/**
 * These are intentional constructed so that the same attributes share
 * the same array indices for both data and meta files.
 */

constexpr lfs_size_t number_data_attributes = 5;

fklfs_attribute_template_t data_attributes[number_data_attributes] = {
    { LFS_DRIVER_FILE_ATTR_NRECORDS,          sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_FIRST,      sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_LAST,       sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_GPS,        sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_RECORD_UPLOADED,   sizeof(uint32_t), 0xff },
};

constexpr lfs_size_t number_meta_attributes = 9;

fklfs_attribute_template_t meta_attributes[number_meta_attributes] = {
    { LFS_DRIVER_FILE_ATTR_NRECORDS,          sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_FIRST,      sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_LAST,       sizeof(uint32_t), 0x00 },
    { LFS_DRIVER_FILE_ATTR_RECORD_GPS,        sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_RECORD_UPLOADED,   sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_CONFIG_MODULES,    sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_CONFIG_SCHEDULE,   sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_CONFIG_STATE,      sizeof(uint32_t), 0xff },
    { LFS_DRIVER_FILE_ATTR_CONFIG_OTHER,      sizeof(uint32_t), 0xff },
};

int32_t lfs_debug_attributes(lfs_file_config &config) {
    for (auto i = 0u; i < config.attr_count; ++i) {
        if (config.attrs[i].size == sizeof(uint32_t)) {
            logdebug("attr[%2x] = %" PRIu32, i, *(uint32_t *)config.attrs[i].buffer);
        }
        else {
            logdebug("attr[%2x] unknown size=%d", config.attrs[i].size);
        }
    }

    return 0;
}

// Read a region in a block. Negative error codes are propogated
// to the user.
int lfs_block_device_read(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    auto driver = reinterpret_cast<LfsDriver*>(c->context);
    return driver->read(c, block, off, buffer, size);
}

int lfs_block_device_prog(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    auto driver = reinterpret_cast<LfsDriver*>(c->context);
    return driver->prog(c, block, off, buffer, size);
}

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int lfs_block_device_erase(struct lfs_config const *c, lfs_block_t block) {
    auto driver = reinterpret_cast<LfsDriver*>(c->context);
    return driver->erase(c, block);
}

// Sync the state of the underlying block device. Negative error codes
// are propogated to the user.
int lfs_block_device_sync(struct lfs_config const *c) {
    return 0;
}

LfsDriver::LfsDriver() : memory_(nullptr), pool_(nullptr) {
}

LfsDriver::~LfsDriver() {
}

bool LfsDriver::begin(DataMemory *memory, Pool &pool, bool force_create) {
    memory_ = memory;
    pool_ = &pool;

    auto g = memory->geometry();

    if (!memory->begin()) {
        logerror("begin memory failed");
        return false;
    }

    cfg_ = {
        .context = this,
        .read = lfs_block_device_read,
        .prog = lfs_block_device_prog,
        .erase = lfs_block_device_erase,
        .sync = lfs_block_device_sync,
        .read_size = LFS_DRIVER_READ_SIZE,
        .prog_size = LFS_DRIVER_PROG_SIZE,
        .block_size = g.block_size,
        .block_count = g.nblocks,
        .block_cycles = 500,
        .cache_size = LFS_DRIVER_CACHE_SIZE,
        .lookahead_size = LFS_DRIVER_LOOKAHEAD_SIZE,
        // NOTE: Read/prog buffers are LFS_DRIVER_CACHE_SIZE bytes large!
        .read_buffer = pool.malloc(LFS_DRIVER_CACHE_SIZE),
        .prog_buffer = pool.malloc(LFS_DRIVER_CACHE_SIZE),
        .lookahead_buffer = pool.malloc(LFS_DRIVER_LOOKAHEAD_SIZE),
        .name_max = 0,
        .file_max = 0,
        .attr_max = 0,
        .metadata_max = 0,
    };

    if (force_create) {
        auto format_err = lfs_format(&lfs_, &cfg_);
        if (format_err < 0) {
            return false;
        }

        auto mount_err = lfs_mount(&lfs_, &cfg_);
        if (mount_err < 0) {
            return false;
        }
        return true;
    }

    if (lfs_mount(&lfs_, &cfg_) != 0) {
        auto format_err = lfs_format(&lfs_, &cfg_);
        if (format_err < 0) {
            return false;
        }

        auto mount_err = lfs_mount(&lfs_, &cfg_);
        if (mount_err < 0) {
            return false;
        }
    }

    return true;
}

lfs_file_config LfsDriver::make_meta_cfg(Pool &pool) {
    return make_file_cfg(meta_attributes, number_meta_attributes, pool);
}

lfs_file_config LfsDriver::make_data_cfg(Pool &pool) {
    return make_file_cfg(data_attributes, number_data_attributes, pool);
}

lfs_file_config LfsDriver::make_file_cfg(fklfs_attribute_template_t const *attributes, lfs_size_t nattributes, Pool &pool) {
    auto attrs = (struct lfs_attr *)pool.malloc<struct lfs_attr>(nattributes);

    for (auto i = 0u; i < nattributes; ++i) {
        auto size = attributes[i].size;
        auto buffer = pool.malloc(size);
        memset(buffer, attributes[i].fill, size);
        attrs[i].type = attributes[i].type;
        attrs[i].buffer = buffer;
        attrs[i].size = size;
    }

    auto buffer = pool.malloc(LFS_DRIVER_CACHE_SIZE);

    return {
        .buffer = buffer,
        .attrs = attrs,
        .attr_count = nattributes,
    };
};

int32_t LfsDriver::read(struct lfs_config const *cfg, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    LFS_ASSERT(off  % cfg->read_size == 0);
    LFS_ASSERT(size % cfg->read_size == 0);
    LFS_ASSERT(block < cfg->block_count);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    auto remaining = size;

    logverbose("read: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    while (remaining > 0) {
        if (memory_->read(address, (uint8_t *)buffer, cfg->read_size) != (int32_t)cfg->read_size) {
            return -1;
        }

        address += cfg->read_size;
        remaining -= cfg->read_size;
        buffer = ((uint8_t *)buffer) + cfg->read_size;
    }


    return 0;
}

int32_t LfsDriver::prog(struct lfs_config const *cfg, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    LFS_ASSERT(off  % cfg->prog_size == 0);
    LFS_ASSERT(size % cfg->prog_size == 0);
    LFS_ASSERT(block < cfg->block_count);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    auto remaining = size;

    logdebug("prog: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    while (remaining > 0) {
        if (memory_->write(address, (uint8_t *)buffer, cfg->prog_size) != (int32_t)cfg->prog_size) {
            return -1;
        }

        address += cfg->prog_size;
        remaining -= cfg->prog_size;
        buffer = ((uint8_t *)buffer) + cfg->prog_size;
    }

    return 0;
}

int32_t LfsDriver::erase(struct lfs_config const *cfg, lfs_block_t block) {
    logdebug("erase: %" PRIu32, block);

    auto g = memory_->geometry();
    auto address = block * g.block_size;
    auto err = memory_->erase(address, g.block_size);
    if (err < 0) {
        return err;
    }
    return 0;
}

int32_t LfsDriver::get_number_of_files(const char *path) {
    int32_t value = 0;
    auto err = lfs_getattr(lfs(), path, LFS_DRIVER_DIR_ATTR_NFILES, &value, sizeof(int32_t));
    if (err < 0) {
        return err;
    }
    return value;
}

int32_t LfsDriver::set_number_of_files(const char *path, int32_t value) {
    return lfs_setattr(lfs(), path, LFS_DRIVER_DIR_ATTR_NFILES, &value, sizeof(int32_t));
}

int32_t LfsDriver::add_number_of_files(const char *path, int32_t value) {
    auto nfiles = get_number_of_files(path);
    if (nfiles < 0) {
        return nfiles;
    }

    nfiles += value;

    auto err = set_number_of_files(path, nfiles);
    if (err < 0) {
        return err;
    }
    return nfiles;
}

} // namespace fk
