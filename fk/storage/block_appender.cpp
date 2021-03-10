#include <tiny_printf.h>

#include "storage/block_appender.h"
#include "storage/file_map.h"
#include "storage/lfs_attributes.h"
#include "storage/lfs_io.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

BlockAppender::BlockAppender(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
}

bool BlockAppender::create_directory_if_necessary() {
    struct lfs_info info;
    if (lfs_stat(lfs(), directory_, &info) == LFS_ERR_NOENT) {
        FK_ASSERT(lfs_mkdir(lfs(), directory_) == 0);
    }
    return true;
}

bool BlockAppender::append(fk_data_DataRecord *record, Pool &pool) {
    if (!initialized_) {
        FK_ASSERT(create_directory_if_necessary());

        FileMap map{ lfs_, pool };

        if (!map.refresh(directory_, pool)) {
            return false;
        }

        start_of_last_file_ = map.start_of_last_file();

        initialized_ = true;
    }

    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory_, start_of_last_file_);

    loginfo("opening %s", path_);

    lfs_file_t file;
    lfs_file_config file_cfg = lfs_->make_data_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &file_cfg) == 0);

    Attributes attributes{ file_cfg };

    if (should_rollover(&file)) {
        FK_ASSERT(lfs_file_close(lfs(), &file) == 0);

        auto first_block = attributes.first_block() + attributes.nblocks();
        tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory_, first_block);

        loginfo("rollover! creating=%s first-block=%" PRIu32, path_, first_block);
        FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &file_cfg) == 0);
        attributes.first_block(first_block);
        attributes.nblocks(0);

        start_of_last_file_ = first_block;
    }

    auto block = attributes.first_block() + attributes.nblocks();
    auto file_size_before = lfs_file_size(lfs(), &file);

    loginfo("writing block: R-%" PRIu32, block);

    LfsWriter lfs_writer{ lfs_, &file };
    auto ostream = pb_ostream_from_writable(&lfs_writer);
    FK_ASSERT(pb_encode_delimited(&ostream, fk_data_DataRecord_fields, record));
    attributes.increase_nblocks();

    auto file_size_after = lfs_file_size(lfs(), &file);

    logdebug("closing");
    lfs_file_close(lfs(), &file);

    loginfo("wrote: R-%" PRIu32 " file-size=%" PRIu32 " record-size=%d", block, file_size_after,
            file_size_after - file_size_before);

    return true;
}

bool BlockAppender::should_rollover(lfs_file_t *file) {
    auto file_size = lfs_file_size(lfs(), file);
    return file_size > 1024;
}

} // namespace fk
