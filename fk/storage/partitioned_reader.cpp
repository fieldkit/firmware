#include <tiny_printf.h>

#include "storage/partitioned_reader.h"
#include "storage/lfs_attributes.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

PartitionedReader::PartitionedReader(LfsDriver *lfs, FileMap *map, Pool &pool) : lfs_(lfs), map_(map) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
    buffer_ = (uint8_t *)pool.malloc(buffer_size_);
}

tl::expected<block_seek_t, Error> PartitionedReader::seek(uint32_t desired_block, Pool &pool) {
    loginfo("seeking R-%" PRIu32 " in %s", desired_block, directory());

    auto search = map_->find(desired_block, pool);
    if (!search) {
        return tl::unexpected<Error>(search.error());
    }

    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory(), search->start_block_of_last_file);

    loginfo("opening %s", path_);

    close();

    file_cfg_ = lfs_->make_data_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file_, path_, LFS_O_RDONLY, &file_cfg_) == 0);

    Attributes attributes{ file_cfg_ };

    auto block_number = attributes.first_block();

    if (!(desired_block >= block_number && desired_block < block_number + attributes.nblocks())) {
        loginfo("desired block outside of file");
        return tl::unexpected<Error>(Error::General);
    }

    auto file_position = 0u;

    auto success = false;
    block_seek_t seek;
    bzero(&seek, sizeof(block_seek_t));

    while (!success) {
        if (block_number == desired_block) {
            auto position = lfs_file_tell(lfs(), &file_);
            loginfo("seek: have R-%" PRIu32 " found at %" PRIu32, block_number, position);
            success = true;
            seek = {
                .block = desired_block,
                .first_block_of_containing_file = search->start_block_of_last_file,
                .absolute_position = file_position + search->bytes_before_start_of_last_file,
                .file_position = file_position,
            };
            break;
        }

        auto nread = lfs_file_read(lfs(), &file_, buffer_, buffer_size_);
        if (nread < 0) {
            logerror("seek error reading file");
            return tl::unexpected<Error>(Error::General);
        }
        else if (nread == 0) {
            break;
        }

        uint32_t record_size = 0;
        pb_istream_t stream = pb_istream_from_buffer((pb_byte_t *)buffer_, nread);
        if (!pb_decode_varint32(&stream, &record_size)) {
            logerror("seek error decoding record size");
            return tl::unexpected<Error>(Error::General);
        }

        uint32_t total_record_size = record_size + pb_varint_size(record_size);

        int32_t sibling_offset = total_record_size - nread;

        loginfo("seek: skip R-%" PRIu32 " size=%" PRIu32 " seeking=%d nread=%d", block_number, total_record_size,
                sibling_offset, nread);

        file_position = lfs_file_seek(lfs(), &file_, sibling_offset, LFS_SEEK_CUR);

        block_number++;
    }

    if (!success) {
        return tl::unexpected<Error>(Error::EoF);
    }

    opened_ = true;

    return seek;
}

Reader *PartitionedReader::open_reader(Pool &pool) {
    FK_ASSERT(opened_);

    reader_pool_ = &pool;

    return this;
}

int32_t PartitionedReader::read(uint8_t *buffer, size_t size) {
    FK_ASSERT(reader_pool_ != nullptr);

    auto bytes_or_err = lfs_file_read(lfs(), &file_, buffer, size);
    if (bytes_or_err < 0) {
        return bytes_or_err;
    }
    if (bytes_or_err > 0) {
        return bytes_or_err;
    }

    // We need to move to the following file, now.
    loginfo("end of file, seeking");

    Attributes attributes{ file_cfg_ };

    auto block_after = attributes.first_block() + attributes.nblocks();

    if (!seek(block_after, *reader_pool_)) {
        loginfo("end of partitioned file");
        return 0;
    }

    bytes_or_err = lfs_file_read(lfs(), &file_, buffer, size);
    if (bytes_or_err < 0) {
        return bytes_or_err;
    }
    if (bytes_or_err > 0) {
        return bytes_or_err;
    }

    return -1;
}

void PartitionedReader::close() {
    if (opened_) {
        lfs_file_close(lfs(), &file_);
        opened_ = false;
    }
}

} // namespace fk
