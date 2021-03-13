#include <tiny_printf.h>
#include <inttypes.h>
#include <phylum/blake2b.h>

#include "storage/record_appender.h"
#include "storage/lfs_io.h"

// NOTE Consider passing time into write methods.
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

class SignedRecordBuilder {
private:
    uint8_t hash_[Hash::Length];
    pb_data_t data_ref_;
    pb_data_t hash_ref_;
    fk_data_SignedRecord record_;

public:
    SignedRecordBuilder(uint32_t record_number, void *record, pb_msgdesc_t const *fields, Pool &pool);

public:
    fk_data_SignedRecord *record();

public:
    uint8_t *hash() {
        return (uint8_t *)hash_ref_.buffer;
    }

};

SignedRecordBuilder::SignedRecordBuilder(uint32_t record_number, void *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto encoded = pool.encode(fields, record);

    BLAKE2b b2b;
    b2b.reset(Hash::Length);
    b2b.update(encoded->buffer, encoded->size);
    b2b.finalize(&hash_, Hash::Length);

    data_ref_ = {
        .length = encoded->size,
        .buffer = encoded->buffer,
    };

    hash_ref_ = {
        .length = Hash::Length,
        .buffer = hash_,
    };

    record_ = fk_data_SignedRecord_init_default;
    record_.kind = (fk_data_SignedRecordKind)0;
    record_.data.funcs.encode = pb_encode_data;
    record_.data.arg = (void *)&data_ref_;
    record_.hash.funcs.encode = pb_encode_data;
    record_.hash.arg = (void *)&hash_ref_;
    record_.record = record_number;
    record_.time = get_clock_now(); // Consider passing in.
}

fk_data_SignedRecord *SignedRecordBuilder::record() {
    return &record_;
};

RecordAppender::RecordAppender(LfsDriver *lfs, FileMap *map, lfs_size_t rollover_size, Pool &pool) : lfs_(lfs), map_(map), strategy_{ rollover_size } {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
}

AppendedRecordOrError RecordAppender::append_data_record(fk_data_DataRecord *record, Pool &pool) {
    auto maybe_err = locate_tail(pool);
    if (maybe_err) {
        return tl::unexpected<Error>(*maybe_err);
    }

    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory(), start_record_of_last_file_);

    loginfo("opening %s", path_);

    lfs_file_t file;
    lfs_file_config file_cfg = lfs_->make_data_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &file_cfg) == 0);

    Attributes attributes{ file_cfg };

    // Check to see if this file is full and we can rollover to a new
    // file. We close the old one and then open a new one, being
    // careful to initialize first_record and nrecords properly.
    if (should_rollover(&file)) {
        FK_ASSERT(lfs_file_close(lfs(), &file) == 0);

        auto first_record = attributes.first_record() + attributes.nrecords();

        tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory(), first_record);

        loginfo("rollover! creating=%s first-record=%" PRIu32, path_, first_record);
        FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &file_cfg) == 0);
        attributes.first_record(first_record);
        attributes.nrecords(0);

        lfs_->add_number_of_files(directory(), 1);

        start_record_of_last_file_ = first_record;
    }

    auto record_number = attributes.first_record() + attributes.nrecords();
    record->readings.reading = record_number;

    return write_record(file, attributes, record, fk_data_DataRecord_fields, pool);
}

AppendedRecordOrError RecordAppender::append_changes(uint8_t kind, void *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto maybe_err = locate_tail(pool);
    if (maybe_err) {
        return tl::unexpected<Error>(*maybe_err);
    }

    // Open the file, getting access to the custom attributes.
    tiny_snprintf(path_, LFS_NAME_MAX, "%s/%08" PRIx32 ".fkpb", directory(), start_record_of_last_file_);

    loginfo("opening %s", path_);

    lfs_file_t file;
    lfs_file_config file_cfg = lfs_->make_meta_cfg(pool);
    FK_ASSERT(lfs_file_opencfg(lfs(), &file, path_, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, &file_cfg) == 0);
    Attributes attributes{ file_cfg };

    auto record_number = attributes.first_record() + attributes.nrecords();
    SignedRecordBuilder signed_record{ record_number, record, fields, pool };

    // Do we have a record of this kind yet? The default value for
    // these attributes is a 0xff fill just to avoid ambiguity over
    // the start of the file at 0.
    auto position = attributes.get(kind);
    if (position != UINT32_MAX) {
        loginfo("found existing kind=%d @ %" PRIu32 "", kind, position);

        // Seek to the position in the file, this returns the new
        // position in the file.
        FK_ASSERT(lfs_file_seek(lfs(), &file, position, LFS_SEEK_SET) >= 0);

        // TODO Move to class
        fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
        sr.hash.funcs.decode = pb_decode_data;
        sr.hash.arg = (void *)&pool;

        LfsReader lfs_reader{ lfs_, &file };
        auto istream = pb_istream_from_readable(&lfs_reader);
        if (!pb_decode_delimited(&istream, fk_data_SignedRecord_fields, &sr)) {
            return tl::unexpected<Error>(Error::IO);
        }

        // Compare the hashes here, if this is different we'll be
        // appending this new record.
        auto hash_ref = (pb_data_t *)sr.hash.arg;
        FK_ASSERT(hash_ref->length == Hash::Length);
        if (memcmp(signed_record.hash(), hash_ref->buffer, Hash::Length) == 0) {
            auto absolute_position = bytes_before_start_of_last_file_ + position;
            return appended_record_t{
                .record = (uint32_t)sr.record,
                .first_record_of_containing_file = start_record_of_last_file_,
                .absolute_position = (lfs_size_t)absolute_position,
                .file_position = (lfs_size_t)position,
                .record_size = (lfs_size_t)0,
            };
        }

        loginfo("modified, appending");
    }
    else {
        loginfo("first of its kind=%d, appending", kind);
    }

    // Set the attribute for this kind to the position of this new record.
    attributes.set(kind, lfs_file_tell(lfs(), &file));

    return write_record(file, attributes, signed_record.record(), fk_data_SignedRecord_fields, pool);
}

AppendedRecordOrError RecordAppender::write_record(lfs_file_t &file, Attributes &attributes, void *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto record_number = attributes.first_record() + attributes.nrecords();
    auto file_size_before = lfs_file_size(lfs(), &file);

    attributes.mark_last_record(file_size_before);

    logdebug("writing record: R-%" PRIu32, record_number);

    // Spin up a writer and drop the record into the directly file.
    LfsWriter lfs_writer{ lfs_, &file };
    BufferedWriter buffered{ &lfs_writer, (uint8_t *)pool.malloc(1024), 1024 };
    auto ostream = pb_ostream_from_writable(&buffered);
    FK_ASSERT(pb_encode_delimited(&ostream, fields, record));
    if (buffered.flush() <= 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    // Ensure the updated nrecords attributes gets written with this
    // appended record.
    attributes.increase_nrecords();

    auto file_size_after = lfs_file_size(lfs(), &file);

    // Commit our changes to the file system.
    logdebug("closing");
    lfs_file_close(lfs(), &file);

    // If this file is 0 bytes in length then we need to refresh our
    // map because this is a new file, so we invalidate to ensure a
    // future rescan.
    if (file_size_before == 0) {
        if (!map_->refresh()) {
            return tl::unexpected<Error>(Error::IO);
        }
    }

    auto absolute_position = bytes_before_start_of_last_file_ + file_size_before;
    auto record_size = file_size_after - file_size_before;
    loginfo("wrote: R-%" PRIu32 " file-size=%" PRIu32 " record-size=%d",
            record_number, file_size_after, record_size);

    return appended_record_t{
        .record = record_number,
        .first_record_of_containing_file = start_record_of_last_file_,
        .absolute_position = absolute_position,
        .file_position = (lfs_size_t)file_size_before,
        .record_size = (lfs_size_t)record_size,
    };
}

bool RecordAppender::create_directory_if_necessary() {
    struct lfs_info info;
    if (lfs_stat(lfs(), directory(), &info) == LFS_ERR_NOENT) {
        FK_ASSERT(lfs_mkdir(lfs(), directory()) == 0);
    }

    return true;
}

optional<Error> RecordAppender::locate_tail(Pool &pool) {
    if (!initialized_) {
        FK_ASSERT(create_directory_if_necessary());

        // In order to append we only need to know the very last file
        // in the sequence, so refresh the map and get the start record
        // of that last file.
        auto search = map_->find(UINT32_MAX, pool);
        if (!search) {
            logerror("append error finding tail");
            return search.error();
        }

        start_record_of_last_file_ = search->start_record_of_last_file;
        bytes_before_start_of_last_file_ = search->bytes_before_start_of_last_file;

        initialized_ = true;
    }

    return nullopt;
}

FileSizeRollover::FileSizeRollover(lfs_size_t size) : size_(size) {
}

bool FileSizeRollover::should_rollover(lfs_t *lfs, lfs_file_t *file) {
    if (size_ == 0) {
        return false;
    }
    int32_t file_size = lfs_file_size(lfs, file);
    if (file_size < 0) {
        // TODO Why not?
        return true;
    }
    return (lfs_size_t)file_size > size_;
}

} // namespace fk
