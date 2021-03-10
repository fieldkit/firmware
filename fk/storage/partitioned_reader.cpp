#include <tiny_printf.h>

#include "storage/partitioned_reader.h"
#include "storage/lfs_attributes.h"
#include "storage/file_map.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

PartitionedReader::PartitionedReader(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
    buffer_ = (uint8_t *)pool.malloc(buffer_size_);
}

bool PartitionedReader::seek(uint32_t desired_block, Pool &pool) {
    FileMap map{ lfs_, pool };

    loginfo("seeking R-%" PRIu32 " in %s", desired_block, directory_);

    if (!map.refresh(directory_, desired_block, pool)) {
        return false;
    }

    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory_, map.start_of_last_file());

    loginfo("opening %s", path_);

    lfs_file_t file;
    lfs_file_config file_cfg = lfs_->make_data_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDONLY, &file_cfg) == 0);

    Attributes attributes{ file_cfg };

    auto success = false;

    auto block_number = attributes.first_block();

    while (true) {
        if (block_number == desired_block) {
            auto position = lfs_file_tell(lfs(), &file);
            loginfo("seek: have R-%" PRIu32 " found at %" PRIu32, block_number, position);
            success = true;
            break;
        }

        auto nread = lfs_file_read(lfs(), &file, buffer_, buffer_size_);
        if (nread < 0) {
            logerror("seek error reading file");
            return false;
        } else if (nread == 0) {
            break;
        }

        uint32_t record_size = 0;
        pb_istream_t stream = pb_istream_from_buffer((pb_byte_t *)buffer_, nread);
        if (!pb_decode_varint32(&stream, &record_size)) {
            logerror("seek error decoding record size");
            return false;
        }

        uint32_t total_record_size = record_size + pb_varint_size(record_size);

        int32_t sibling_offset = total_record_size - nread;

        loginfo("seek: skip R-%" PRIu32 " size=%" PRIu32 " seeking=%d nread=%d", block_number, total_record_size,
                sibling_offset, nread);

        lfs_file_seek(lfs(), &file, sibling_offset, LFS_SEEK_CUR);

        block_number++;
    }

    FK_ASSERT(lfs_file_close(lfs(), &file) == 0);

    return success;
}

} // namespace fk
