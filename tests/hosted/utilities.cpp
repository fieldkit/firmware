#include <fk-data-protocol.h>

#include "tests.h"
#include "utilities.h"
#include "protobuf.h"
#include "platform.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

size_t write_reading(File &file) {
    fk_data_SensorAndValue readings[] = {
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

    pb_array_t readings_array = {
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_SensorGroup groups[] = {
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

    pb_array_t sensor_groups_array = {
        .length = (size_t)1,
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = &groups,
        .fields = fk_data_SensorGroup_fields,
    };

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.time = fk_uptime();
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.location.fix = 0;
    record.readings.location.time = fk_uptime();
    record.readings.location.longitude = -118.2709223;
    record.readings.location.latitude = 34.0318047;
    record.readings.location.altitude = 100.0f;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.readings.sensorGroups.arg = &sensor_groups_array;

    auto wrote = file.write(&record, fk_data_DataRecord_fields);
    if (wrote == 0) {
        logerror("error saving readings");
        return 0;
    }

    return wrote;
}

void append_metadata_always(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

void append_metadata_always(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, size_t &appended, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;

    auto ar = srl.append_always(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(ar);

    appended = (*ar).size;
}

void append_metadata(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

void append_metadata(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, size_t &appended, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;

    auto ar = srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(ar);

    appended = (*ar).size;
}

void append_other_always(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

void append_other(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

extern "C" {

void osi_assert(const char *assertion, const char *file, int line) {
    FAIL() << "Assertion \"" << assertion << "\" failed. File: " << file << " Line: " << line;
}

void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...) {
    FAIL() << "Assertion \"" << assertion << "\" failed. File: " << file << " Line: " << line;
}

}
