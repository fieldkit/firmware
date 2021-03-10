#include "lfs_driver.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

constexpr lfs_size_t number_data_attributes = 2;

fklfs_attribute_template_t data_attributes[number_data_attributes] = {
    { LFS_DRIVER_ATTR_NBLOCKS,     sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_FIRST_BLOCK, sizeof(uint32_t) },
};

constexpr lfs_size_t number_meta_attributes = 6;

fklfs_attribute_template_t meta_attributes[number_meta_attributes] = {
    { LFS_DRIVER_ATTR_FIRST_BLOCK,     sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_NBLOCKS,         sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_CONFIG_MODULES,  sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_CONFIG_SCHEDULE, sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_CONFIG_STATE,    sizeof(uint32_t) },
    { LFS_DRIVER_ATTR_CONFIG_OTHER,    sizeof(uint32_t) },
};

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


LfsDriver::LfsDriver(DataMemory *memory, Pool &pool) : memory_(memory), pool_(&pool) {
    auto g = memory->geometry();

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
        .read_buffer = pool_->malloc(LFS_DRIVER_CACHE_SIZE),
        .prog_buffer = pool_->malloc(LFS_DRIVER_CACHE_SIZE),
        .lookahead_buffer = pool_->malloc(LFS_DRIVER_LOOKAHEAD_SIZE),
        .name_max = 0,
        .file_max = 0,
        .attr_max = 0,
        .metadata_max = 0,
    };
}

LfsDriver::~LfsDriver() {
}

bool LfsDriver::begin(bool force_create) {
    if (force_create) {
        FK_ASSERT(!lfs_format(&lfs_, &cfg_));
        FK_ASSERT(!lfs_mount(&lfs_, &cfg_));
        return true;
    }

    if (lfs_mount(&lfs_, &cfg_) != 0) {
        FK_ASSERT(!lfs_format(&lfs_, &cfg_));
        FK_ASSERT(!lfs_mount(&lfs_, &cfg_));
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
    auto buffer = pool.malloc(LFS_DRIVER_CACHE_SIZE);

    for (auto i = 0u; i < nattributes; ++i) {
        auto size = attributes[i].size;
        attrs[i].type = attributes[i].type;
        attrs[i].buffer = pool.malloc(size);
        attrs[i].size = size;
        bzero(attrs[i].buffer, size);
    }

    return {
        .buffer = buffer,
        .attrs = attrs,
        .attr_count = nattributes,
    };
};

int32_t LfsDriver::read(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    logverbose("read: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    if (memory_->read(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

int32_t LfsDriver::prog(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    logdebug("prog: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    if (memory_->write(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

int32_t LfsDriver::erase(struct lfs_config const *c, lfs_block_t block) {
    logdebug("erase: %" PRIu32, block);

    auto g = memory_->geometry();
    auto address = block * g.block_size;
    if (!memory_->erase(address, g.block_size)) {
        return -1;
    }

    return 0;
}

} // namespace fk
