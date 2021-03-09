#include "lfs_driver.h"

#include "hal/memory.h"
#include "pool.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

typedef DataMemory*(*DataMemoryFn)(void);

// Read a region in a block. Negative error codes are propogated
// to the user.
int lfs_block_device_read(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    auto memory = reinterpret_cast<DataMemoryFn>(c->context)();
    auto g = memory->geometry();
    auto address = block * g.block_size + off;
    if (memory->read(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

int lfs_block_device_prog(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    auto memory = reinterpret_cast<DataMemoryFn>(c->context)();
    auto g = memory->geometry();
    auto address = block * g.block_size + off;
    if (memory->write(address, (uint8_t *)buffer, size) != (int32_t)size) {
        return -1;
    }

    return 0;
}

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
int lfs_block_device_erase(struct lfs_config const *c, lfs_block_t block) {
    auto memory = reinterpret_cast<DataMemoryFn>(c->context)();
    auto g = memory->geometry();
    auto address = block * g.block_size;
    if (!memory->erase(address, g.block_size)) {
        return -1;
    }

    return 0;
}

// Sync the state of the underlying block device. Negative error codes
// are propogated to the user.
int lfs_block_device_sync(struct lfs_config const *c) {
    return 0;
}

struct lfs_config cfg = {
    .context = (void *)MemoryFactory::get_data_memory,
    .read = lfs_block_device_read,
    .prog = lfs_block_device_prog,
    .erase = lfs_block_device_erase,
    .sync = lfs_block_device_sync,
    .read_size = 1024,
    .prog_size = 1024,
    .block_size = 128 * 1024,
    .block_count = 128,
    .block_cycles = 500,
    .cache_size = 1024,
    .lookahead_size = 16,
    .read_buffer = nullptr,
    .prog_buffer = nullptr,
    .lookahead_buffer = nullptr,
    .name_max = 0,
    .file_max = 0,
    .attr_max = 0,
    .metadata_max = 0,
};

int32_t lfs_test() {
    StandardPool pool{ "lfs" };

    lfs_t lfs;

    FK_ASSERT(MemoryFactory::get_data_memory()->begin());

    cfg.read_buffer = pool.malloc(1024);
    cfg.prog_buffer = pool.malloc(1024);
    cfg.lookahead_buffer = pool.malloc(16);

    int32_t err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

    lfs_file_config file_cfg = {
        .buffer = pool.malloc(1024),
        .attrs = nullptr,
        .attr_count = 0,
    };

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

    loginfo("test done: %" PRIu32 " size=%" PRIu32, boot_count, fs_blocks * cfg.block_size);

    while (1) {
    }

    return 0;
}

} // namespace fk
