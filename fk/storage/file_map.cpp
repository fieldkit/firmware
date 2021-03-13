#include <tiny_printf.h>

#include "storage/file_map.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

FileMap::FileMap(LfsDriver *lfs, const char *directory, int32_t number_of_files_to_keep, Pool &pool) : lfs_(lfs), directory_(directory), number_of_files_to_keep_(number_of_files_to_keep) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
    cache_pool_ = pool.subpool("file-map");
}

FileMap::~FileMap() {
}

bool FileMap::refresh() {
    int32_t expected_number_of_files = lfs_->get_number_of_files(directory_);
    if (expected_number_of_files < 0) {
        if (expected_number_of_files != LFS_ERR_NOATTR) {
            logerror("error number of files (%d)", expected_number_of_files);
        }
        expected_number_of_files = 0;
    }

    // Clear the pool and invalidate cache.
    cache_pool_->clear();
    cache_ = nullptr;

    lfs_dir_t dir;

    auto err = lfs_dir_open(lfs(), &dir, directory_);
    if (err < 0) {
        logerror("empty opening dir (%d)", err);
        return false;
    }

    number_files_ = 0;

    // We're building a new cache.
    cache_entry_t *head = nullptr;
    cache_entry_t *tail = nullptr;

    struct lfs_info info;
    while (lfs_dir_read(lfs(), &dir, &info)) {
        if (info.name[0] == '.') {
            continue;
        }

        tiny_snprintf(path_, LFS_NAME_MAX, "%s/%s", directory_, info.name);

        uint32_t first_record = 0;
        err = lfs_getattr(lfs(), path_, LFS_DRIVER_FILE_ATTR_RECORD_FIRST, &first_record, sizeof(first_record));
        if (err < 0) {
            logerror("error reading attr (%d)", err);
            return false;
        }

        uint32_t nrecords = 0;
        err = lfs_getattr(lfs(), path_, LFS_DRIVER_FILE_ATTR_NRECORDS, &nrecords, sizeof(nrecords));
        if (err < 0) {
            logerror("error reading attr (%d)", err);
            return false;
        }

        loginfo("[% 4d] ls: '%s' type=%d size=%d attrs: first-record=%" PRIu32 " nrecords=%" PRIu32, number_files_,
                info.name, info.type, info.size, first_record, nrecords);

        auto entry = (cache_entry_t *)cache_pool_->malloc(sizeof(cache_entry_t));
        entry->first_record = first_record;
        entry->nrecords = nrecords;
        entry->size = info.size;
        entry->np = nullptr;

        // TODO We be verifying the sort order.
        if (head == nullptr) {
            head = entry;
            tail = entry;
        }
        else {
            tail->np = entry;
            tail = entry;
        }

        number_files_++;
    }

    lfs_dir_close(lfs(), &dir);

    // Only update the number of files when they change.
    if (number_files_ != expected_number_of_files) {
        lfs_->set_number_of_files(directory_, number_files_);
    }

    cache_ = head;

    return true;
}

tl::expected<record_file_search_t, Error> FileMap::find(uint32_t desired_record, Pool &pool) {
    if (!initialized_) {
        if (!refresh()) {
            return tl::unexpected<Error>(Error::IO);
        }

        initialized_ = true;
    }

    uint32_t start_record_of_first_file{ UINT32_MAX };
    uint32_t start_record_of_last_file{ 0 };
    uint32_t bytes_before_start_of_last_file{ 0 };
    uint32_t last_record{ 0 };
    uint32_t total_bytes{ 0 };

    for (auto iter = cache_; iter != nullptr; iter = iter->np) {
        if (iter->first_record > start_record_of_last_file) {
            start_record_of_last_file = iter->first_record;
        }

        if (iter->first_record < start_record_of_first_file) {
            start_record_of_first_file = iter->first_record;
        }

        auto last_record_in_file = iter->first_record + iter->nrecords;
        if (last_record < last_record_in_file) {
            last_record = last_record_in_file;
        }

        if (desired_record >= iter->first_record && desired_record < iter->first_record + iter->nrecords) {
            break;
        }

        // If we're here and this is the final file then definitely
        // don't count that one.
        if (iter->np != nullptr) {
            bytes_before_start_of_last_file += iter->size;
        }

        total_bytes += iter->size;
    }

    return record_file_search_t{
        .start_record_of_first_file = start_record_of_first_file,
        .start_record_of_last_file = start_record_of_last_file,
        .bytes_before_start_of_last_file = bytes_before_start_of_last_file,
        .last_record = last_record,
        .total_bytes = total_bytes,
    };
}

bool FileMap::prune() {
    if (cache_ == nullptr) {
        return true;
    }

    int32_t keeping = number_of_files_to_keep_;
    for (auto iter = cache_; iter != nullptr; iter = iter->np) {
        if (--keeping < 0) {
            // Notice that we're pruning the head of the cache here.
            auto pruning = cache_;

            tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory_, pruning->first_record);

            loginfo("removing: '%s' first-record=%" PRIu32 " nrecords=%" PRIu32 "",
                    path_, pruning->first_record, pruning->nrecords);

            auto err = lfs_remove(lfs(), path_);
            if (err < 0) {
                logwarn("error removing '%s' (%d)", path_, err);
            }

            cache_ = cache_->np;
        }
    }

    return true;
}

tl::expected<partition_attributes_t, Error> FileMap::attributes(Pool &pool) {
    auto end_of_file = find(UINT32_MAX, pool);
    if (!end_of_file) {
        return tl::unexpected<Error>(end_of_file.error());
    }

    return partition_attributes_t{
        .size = end_of_file->total_bytes,
        .records = end_of_file->last_record,
    };
}

void FileMap::invalidate() {
    initialized_ = false;
}

} // namespace fk
