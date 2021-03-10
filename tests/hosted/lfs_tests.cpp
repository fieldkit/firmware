#include <tiny_printf.h>

#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/lfs_driver.h"

using namespace fk;

FK_DECLARE_LOGGER("lfs");

class ReadingRecord {
public:
    fk_data_SensorAndValue readings[10]{
        { 0, (float)random() },
        { 1, (float)random() },
        { 2, (float)random() },
        { 3, (float)random() },
        { 4, (float)random() },
        { 5, (float)random() },
        { 6, (float)random() },
        { 7, (float)random() },
        { 8, (float)random() },
        { 9, (float)random() },
    };

    pb_array_t readings_array{
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_SensorGroup groups[1]{
        {
            .module = 0,
            .readings = {
                .funcs = {
                    .encode = pb_encode_array,
                },
                .arg = &readings_array,
            },
        }
    };

    pb_array_t sensor_groups_array{
        .length = (size_t)1,
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = &groups,
        .fields = fk_data_SensorGroup_fields,
    };

    fk_data_DataRecord record;

public:
    ReadingRecord(uint32_t uptime, uint32_t reading_number) {
        record = fk_data_DataRecord_init_default;
        record.has_readings = true;
        record.readings.time = uptime;
        record.readings.reading = reading_number;
        record.readings.flags = 0;
        record.readings.has_location = true;
        record.readings.location.fix = 0;
        record.readings.location.time = fk_uptime();
        record.readings.location.longitude = -118.2709223;
        record.readings.location.latitude = 34.0318047;
        record.readings.location.altitude = 100.0f;
        record.readings.sensorGroups.funcs.encode = pb_encode_array;
        record.readings.sensorGroups.arg = &sensor_groups_array;
    }
};

class LfsSuite : public ::testing::Test {
protected:
    void SetUp() override {
        log_configure_level(LogLevels::INFO);
    }

    void TearDown() override {
    }

};

class Attributes {
private:
    lfs_file_config &cfg_;

public:
    Attributes(lfs_file_config &cfg) : cfg_(cfg) {
    }

public:
    uint32_t nblocks() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_NBLOCKS].buffer;
    }

    uint32_t first_block() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_FIRST_BLOCK].buffer;
    }

    uint32_t increase_nblocks() {
        auto &nblocks = *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_NBLOCKS].buffer;
        nblocks++;
        return nblocks;
    }

    void set(uint8_t index, uint32_t value) {
        *(uint32_t *)cfg_.attrs[index].buffer = value;
    }

    void first_block(uint32_t first_block) {
        set(LFS_DRIVER_ATTR_FIRST_BLOCK, first_block);
    }

    void nblocks(uint32_t nblocks) {
        set(LFS_DRIVER_ATTR_NBLOCKS, nblocks);
    }

};

class LfsReader : public Reader {
private:
    LfsDriver *lfs_{ nullptr };
    lfs_file_t *file_{ nullptr };

public:
    LfsReader(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
    }

public:
    int32_t read(uint8_t *buffer, size_t size) override {
        if (lfs_file_read(lfs_->lfs(), file_, buffer, size) >= 0) {
            return size;
        }
        return -1;
    }

};

class LfsWriter : public Writer {
private:
    LfsDriver *lfs_{ nullptr };
    lfs_file_t *file_{ nullptr };

public:
    LfsWriter(LfsDriver *lfs, lfs_file_t *file) : lfs_(lfs), file_(file) {
    }

public:
    int32_t write(uint8_t const *buffer, size_t size) override {
        if (lfs_file_write(lfs_->lfs(), file_, buffer, size) >= 0) {
            return size;
        }
        return -1;
    }

};

class FileMap {
private:
    LfsDriver *lfs_{ nullptr };
    uint32_t first_file_{ 0 };
    uint32_t start_of_last_file_{ 0 };
    uint32_t last_block_{ 0 };
    uint32_t bytes_traversed_{ 0 };
    bool initialized_{ false };

public:
    FileMap(LfsDriver *lfs, Pool &pool) : lfs_(lfs) {
    }

public:
    uint32_t first_file() {
        return first_file_;
    }

    uint32_t start_of_last_file() {
        return start_of_last_file_;
    }

    uint32_t last_block() {
        return last_block_;
    }

    uint32_t bytes_traversed() {
        return bytes_traversed_;
    }

public:
    bool refresh(const char *directory, uint32_t desired_block, Pool &pool) {
        auto path = (char *)pool.malloc(LFS_NAME_MAX);

        lfs_dir_t dir;
        lfs_dir_open(lfs(), &dir, directory);

        bytes_traversed_ = 0;
        start_of_last_file_ = 0;
        first_file_ = UINT32_MAX;

        /**
         * This is basically an array of indices/block numbers and
         * their lengths, which for full memory consumes
         *
         * (4 + 4) * (NBLOCKS / BLOCKS_PER_FILE) bytes
         *
         * You can even get this down to 4 bytes if you assume
         * contiguous ranges, and verify before assuming.
         *
         * Which will easily sit inside a single page of memory:
         *
         * 8192 / 4 = 2048 files, or one file per block on two chip.
         *
         */
        struct lfs_info info;
        while (lfs_dir_read(lfs(), &dir, &info)) {
            if (info.name[0] == '.') {
                continue;
            }

            tiny_snprintf(path, LFS_NAME_MAX, "%s/%s", directory, info.name);

            uint32_t first_block = 0;
            lfs_getattr(lfs(), path, LFS_DRIVER_ATTR_FIRST_BLOCK, &first_block, sizeof(first_block));

            uint32_t nblocks = 0;
            lfs_getattr(lfs(), path, LFS_DRIVER_ATTR_NBLOCKS, &nblocks, sizeof(nblocks));

            loginfo("ls: '%s' type=%d size=%d attrs: first-block=%" PRIu32 " nblocks=%" PRIu32,
                    info.name, info.type, info.size, first_block, nblocks);

            if (first_block > start_of_last_file_) {
                start_of_last_file_ = first_block;
            }

            if (first_block < first_file_) {
                first_file_ = first_block;
            }

            bytes_traversed_ += info.size;

            if (desired_block >= first_block && desired_block < first_block + nblocks) {
                break;
            }
        }

        lfs_dir_close(lfs(), &dir);

        return true;
    }

    bool refresh(const char *directory, Pool &pool) {
        return refresh(directory, UINT32_MAX, pool);
    }

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

};

constexpr size_t MaximumRecordSizeEncodedSize = 4;

class AppendDataRecord {
private:
    LfsDriver *lfs_{ nullptr };
    const char *directory_{ nullptr };
    uint32_t start_of_last_file_{ 0 };
    bool initialized_{ false };
    char *path_{ nullptr };

public:
    AppendDataRecord(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
	    path_ = (char *)pool.malloc(LFS_NAME_MAX);
    }

public:
    bool create_directory_if_necessary() {
        struct lfs_info info;
        if (lfs_stat(lfs(), directory_, &info) == LFS_ERR_NOENT) {
            FK_ASSERT(lfs_mkdir(lfs(), directory_) == 0);
        }
        return true;
    }

    bool append(fk_data_DataRecord *record, Pool &pool) {
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

	    loginfo("wrote: R-%" PRIu32 " file-size=%" PRIu32 " record-size=%d",
                block, file_size_after, file_size_after - file_size_before);

	    return true;
    }

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

    bool should_rollover(lfs_file_t *file) {
	    auto file_size= lfs_file_size(lfs(), file);
	    return file_size > 1024;
    }

};

class PartitionedReader {
private:
    LfsDriver *lfs_{ nullptr };
    const char *directory_{ nullptr };
    bool initialized_{ false };
    char *path_{ nullptr };
    uint8_t *buffer_{ nullptr };
    lfs_size_t buffer_size_{ MaximumRecordSizeEncodedSize };

public:
    PartitionedReader(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
	    path_ = (char *)pool.malloc(LFS_NAME_MAX);
        buffer_ = (uint8_t *)pool.malloc(buffer_size_);
    }

public:
    bool seek(uint32_t desired_block, Pool &pool) {
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
            }
            else if (nread == 0) {
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

            loginfo("seek: skip R-%" PRIu32 " size=%" PRIu32 " seeking=%d nread=%d",
                    block_number, total_record_size, sibling_offset, nread);

            lfs_file_seek(lfs(), &file, sibling_offset, LFS_SEEK_CUR);

            block_number++;
        }

        FK_ASSERT(lfs_file_close(lfs(), &file) == 0);

        return success;
    }

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

};

TEST_F(LfsSuite, Create) {
    StandardPool pool{ "lfs" };
    auto memory = MemoryFactory::get_data_memory();

    FK_ASSERT(memory->begin());

    LfsDriver lfs_driver{ memory, pool };
    FK_ASSERT(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    log_configure_level(LogLevels::NONE);

    AppendDataRecord adr{ &lfs_driver, "data", pool };

    ReadingRecord rr{ 10000, 0 };
    for (auto i = 0u; i < 4 * 20; ++i) {
        StandardPool iter{ "iter" };
        FK_ASSERT(adr.append(&rr.record, iter));
    }

    log_configure_level(LogLevels::INFO);

    PartitionedReader reader{ &lfs_driver, "data", pool };
    FK_ASSERT(reader.seek(17, pool));
    FK_ASSERT(reader.seek(65, pool));

    loginfo("unmounting");
    lfs_unmount(lfs);
}
