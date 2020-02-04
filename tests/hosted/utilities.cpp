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
    record.has_readings = true;
    record.readings.time = fk_uptime();
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.has_location = true;
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
    record.has_metadata = true;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

void append_metadata_always(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, size_t &appended, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_metadata = true;
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
    record.has_metadata = true;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

void append_metadata(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, size_t &appended, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_metadata = true;
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
    record.has_metadata = true;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

void append_other(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_metadata = true;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

void write_meta_records(DataMemory *memory, size_t total) {
    auto size = 0u;
    auto counter = 0u;
    while (size < total) {
        StandardPool pool{ __func__ };
        Storage storage{ memory, pool, false };
        if (!storage.begin()) {
            ASSERT_TRUE(storage.clear());
        }

        auto meta_file = storage.file(1);
        if (!meta_file.seek_end()) {
            FK_ASSERT(meta_file.create());
        }
        auto srl = SignedRecordLog{ meta_file };
        size_t appended = 0;
        append_metadata(srl, counter++, "our-build-1", "our-git-1", appended, pool);

        size += appended + sizeof(RecordHeader) + sizeof(RecordTail);
    }
}

void write_number_of_readings(DataMemory *memory, size_t n) {
    StandardPool pool{ __func__ };
    Storage storage{ memory, pool, false };
    if (!storage.begin()) {
        ASSERT_TRUE(storage.clear());
    }
    auto file = storage.file(0);
    if (!file.seek_end()) {
        ASSERT_TRUE(file.create());
    }
    for (size_t i = 0; i < n; ++i) {
        write_reading(file);
    }
}

void write_readings(DataMemory *memory, size_t total) {
    StandardPool pool{ __func__ };
    Storage storage{ memory, pool, false };
    if (!storage.begin()) {
        ASSERT_TRUE(storage.clear());
    }
    auto file = storage.file(0);
    if (!file.seek_end()) {
        ASSERT_TRUE(file.create());
    }
    auto size = 0u;
    while (size < total) {
        auto wrote = write_reading(file);
        size += wrote + sizeof(RecordHeader) + sizeof(RecordTail);
    }
}

void write_alternating(DataMemory *memory, size_t total) {
    auto size = 0u;
    auto counter = 0u;
    while (size < total) {
        StandardPool pool{ __func__ };
        Storage storage{ memory, pool, false };
        if (!storage.begin()) {
            ASSERT_TRUE(storage.clear());
        }

        auto meta_file = storage.file(1);
        if (!meta_file.seek_end()) {
            FK_ASSERT(meta_file.create());
        }
        auto srl = SignedRecordLog{ meta_file };
        size_t appended = 0;
        append_metadata(srl, counter++, "our-build-1", "our-git-1", appended, pool);

        auto data_file = storage.file(0);
        if (!data_file.seek_end()) {
            ASSERT_TRUE(data_file.create());
        }

        write_reading(data_file);

        size += appended + sizeof(RecordHeader) + sizeof(RecordTail);
    }
}
