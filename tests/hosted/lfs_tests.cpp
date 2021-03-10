#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/block_appender.h"
#include "storage/partitioned_reader.h"

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

TEST_F(LfsSuite, Create) {
    StandardPool pool{ "lfs" };
    auto memory = MemoryFactory::get_data_memory();

    FK_ASSERT(memory->begin());

    LfsDriver lfs_driver{ memory, pool };
    FK_ASSERT(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    log_configure_level(LogLevels::NONE);

    FileMap map{ &lfs_driver, "data", pool };
    BlockAppender appender{ &lfs_driver, &map, 1024, pool };

    ReadingRecord readings{ 10000, 0 };
    for (auto i = 0u; i < 4 * 20; ++i) {
        StandardPool iter{ "iter" };
        FK_ASSERT(appender.append(&readings.record, iter));
    }

    log_configure_level(LogLevels::INFO);

    PartitionedReader reader{ &lfs_driver, &map, pool };

    auto seek1 = reader.seek(17, pool);
    ASSERT_TRUE(seek1);
    ASSERT_EQ(seek1->block, 17u);
    ASSERT_EQ(seek1->first_block_of_containing_file, 10u);
    ASSERT_EQ(seek1->absolute_position, 3051u);
    ASSERT_EQ(seek1->file_position, 791u);

    auto seek2 = reader.seek(64, pool);
    ASSERT_TRUE(seek2);
    ASSERT_EQ(seek2->block, 64u);
    ASSERT_EQ(seek2->first_block_of_containing_file, 60u);
    ASSERT_EQ(seek2->absolute_position, 8362u);
    ASSERT_EQ(seek2->file_position, 452u);

    lfs_unmount(lfs);
}
