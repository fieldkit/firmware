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

PartitionedReader::~PartitionedReader() {
    if (opened_) {
        lfs_file_close(lfs(), &file_);
        opened_ = false;
    }
}

tl::expected<record_file_search_t, Error> PartitionedReader::seek_and_open(uint32_t desired_record, Pool &pool) {
    auto search = map_->find(desired_record, pool);
    if (!search) {
        return tl::unexpected<Error>(search.error());
    }

    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory(), search->start_record_of_last_file);

    loginfo("opening %s", path_);

    close();

    file_cfg_ = lfs_->make_meta_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file_, path_, LFS_O_RDONLY, &file_cfg_) == 0);

    lfs_debug_attributes(file_cfg_);

    return search;
}

tl::expected<record_seek_t, Error> PartitionedReader::seek(uint32_t desired_record, Pool &pool) {
    loginfo("seeking R-%" PRIu32 " in %s", desired_record, directory());

    auto search = seek_and_open(desired_record, pool);
    if (!search) {
        return tl::unexpected<Error>(search.error());
    }

    Attributes attributes{ file_cfg_ };

    auto record_number = attributes.first_record();
    // auto last_record = attributes.first_record() + attributes.nrecords() - 1;
    auto file_position = 0u;
    auto success = false;
    record_seek_t seek;
    bzero(&seek, sizeof(record_seek_t));

    auto total_records = attributes.first_record() + attributes.nrecords();
    if (desired_record < attributes.first_record() || desired_record > total_records) {
        loginfo("desired record outside of file");
        return tl::unexpected<Error>(Error::General);
    }

    // Special case the end of the file.
    if (desired_record == total_records) {
        auto file_position = lfs_file_seek(lfs(), &file_, 0, LFS_SEEK_END);
        if (file_position < 0) {
            loginfo("seeking to end");
            return tl::unexpected<Error>(Error::IO);
        }

        seek = {
            .record = total_records,
            .first_record_of_containing_file = search->start_record_of_last_file,
            .absolute_position = file_position + search->bytes_before_start_of_last_file,
            .file_position = (uint32_t)file_position,
            .readable = false,
        };

        loginfo("seek: eof R-%" PRIu32 " position=%" PRIu32 "", desired_record, file_position);

        success = true;
    }

    // Special case the start of the tail record.
    if (desired_record + 1 == total_records) {
        file_position = attributes.position_of_last_record();

        if (lfs_file_seek(lfs(), &file_, file_position, LFS_SEEK_SET) < 0) {
            loginfo("seeking to tail record");
            return tl::unexpected<Error>(Error::IO);
        }

        seek = {
            .record = total_records - 1,
            .first_record_of_containing_file = search->start_record_of_last_file,
            .absolute_position = file_position + search->bytes_before_start_of_last_file,
            .file_position = file_position,
            .readable = true,
        };

        loginfo("seek: fast R-%" PRIu32 " position=%" PRIu32 "", desired_record, file_position);

        success = true;
    }

    while (!success) {
        if (record_number == desired_record) {
            auto position = lfs_file_tell(lfs(), &file_);
            loginfo("seek: have R-%" PRIu32 " found at %" PRIu32, record_number, position);
            success = true;
            seek = {
                .record = desired_record,
                .first_record_of_containing_file = search->start_record_of_last_file,
                .absolute_position = file_position + search->bytes_before_start_of_last_file,
                .file_position = file_position,
                .readable = true,
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

        loginfo("seek: skip R-%" PRIu32 " size=%" PRIu32 " seeking=%d nread=%d", record_number, total_record_size,
                sibling_offset, nread);

        file_position = lfs_file_seek(lfs(), &file_, sibling_offset, LFS_SEEK_CUR);

        record_number++;
    }

    if (!success) {
        return tl::unexpected<Error>(Error::EoF);
    }

    opened_ = true;

    return seek;
}

tl::expected<record_seek_t, Error> PartitionedReader::seek_fixed(Pool &pool) {
    return seek_via_attr(LFS_DRIVER_FILE_ATTR_RECORD_LAST, pool);
}

tl::expected<record_seek_t, Error> PartitionedReader::seek_via_attr(uint8_t index, Pool &pool) {
    loginfo("seeking via attribute: %d in %s", index, directory());

    auto search = seek_and_open(UINT32_MAX, pool);
    if (!search) {
        return tl::unexpected<Error>(search.error());
    }

    Attributes attributes{ file_cfg_ };

    auto file_position = attributes.get(index);
    if (file_position == UINT32_MAX) {
        loginfo("empty attribute");
        return tl::unexpected<Error>(Error::General);
    }

    loginfo("seeking position: %" PRIu32, file_position);

    if (lfs_file_seek(lfs(), &file_, file_position, LFS_SEEK_SET) < 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    opened_ = true;

    return record_seek_t{
        .record = (uint32_t)(attributes.first_record() + attributes.nrecords() - 1),
        .first_record_of_containing_file = search->start_record_of_last_file,
        .absolute_position = file_position + search->bytes_before_start_of_last_file,
        .file_position = file_position,
        .readable = true,
    };
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

    auto record_after = attributes.first_record() + attributes.nrecords();
    auto resuming = seek(record_after, *reader_pool_);
    if (!resuming) {
        logerror("seeking resume");
        return -1;
    }
    if (!resuming->readable) {
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
