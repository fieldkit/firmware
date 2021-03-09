#include "lfs_driver.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

typedef DataMemory*(*DataMemoryFn)(void);

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
        .read_buffer = pool_->malloc(LFS_DRIVER_READ_SIZE),
        .prog_buffer = pool_->malloc(LFS_DRIVER_PROG_SIZE),
        .lookahead_buffer = pool_->malloc(LFS_DRIVER_LOOKAHEAD_SIZE),
        .name_max = 0,
        .file_max = 0,
        .attr_max = 0,
        .metadata_max = 0,
    };
}

LfsDriver::~LfsDriver() {
}

bool LfsDriver::begin() {
    int32_t err = lfs_mount(&lfs_, &cfg_);
    if (err) {
        FK_ASSERT(!lfs_format(&lfs_, &cfg_));
        FK_ASSERT(!lfs_mount(&lfs_, &cfg_));
    }

    return true;
}

int32_t LfsDriver::read(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    // logdebug("read: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    if (memory_->read(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

int32_t LfsDriver::prog(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    // logdebug("prog: %" PRIu32 " off=%" PRIu32 " size=%" PRIu32, block, off, size);

    auto g = memory_->geometry();
    auto address = block * g.block_size + off;
    if (memory_->write(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

int32_t LfsDriver::erase(struct lfs_config const *c, lfs_block_t block) {
    // logdebug("erase: %" PRIu32, block);

    auto g = memory_->geometry();
    auto address = block * g.block_size;
    if (!memory_->erase(address, g.block_size)) {
        return -1;
    }

    return 0;
}

int32_t lfs_test() {
    StandardPool pool{ "lfs" };

    FK_ASSERT(MemoryFactory::get_data_memory()->begin());

    LfsDriver lfs_driver{ MemoryFactory::get_data_memory(), pool };

    FK_ASSERT(lfs_driver.begin());

    auto &lfs = lfs_driver.lfs();

    lfs_file_config file_cfg = lfs_driver.make_file_cfg();

    lfs_file_t file;
    uint32_t boot_count = 0;
    lfs_file_opencfg(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT, &file_cfg);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));
    boot_count += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));
    lfs_file_close(&lfs, &file);

    lfs_dir_t dir;
    struct lfs_info info;
    lfs_dir_open(&lfs, &dir, "/");
    while (lfs_dir_read(&lfs, &dir, &info)) {
        loginfo("ls: %d '%s' (%d)", info.type, info.name, info.size);
    }
    lfs_dir_close(&lfs, &dir);

    lfs_unmount(&lfs);

    auto fs_blocks = lfs_fs_size(&lfs);

    loginfo("test done: %" PRIu32 " fs-blocks=%" PRIu32, boot_count, fs_blocks);

    while (1) {
    }

    return 0;
}

} // namespace fk
