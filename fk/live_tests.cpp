#include "live_tests.h"

#include "common.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
#include "hal/random.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static size_t write_reading(File &file) {
    fk_data_SensorAndValue readings[] = {
        { 0, (float)fk_random_i32(0, 1000) },
        { 1, (float)fk_random_i32(0, 1000) },
        { 2, (float)fk_random_i32(0, 1000) },
        { 3, (float)fk_random_i32(0, 1000) },
        { 4, (float)fk_random_i32(0, 1000) },
        { 5, (float)fk_random_i32(0, 1000) },
        { 6, (float)fk_random_i32(0, 1000) },
        { 7, (float)fk_random_i32(0, 1000) },
        { 8, (float)fk_random_i32(0, 1000) },
        { 9, (float)fk_random_i32(0, 1000) },
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


static void try_and_reproduce_weird_block_issue() {
    auto gs = get_global_state_rw();

    auto counter = 0;
    auto done = false;

    while (!done) {
        Storage storage{ MemoryFactory::get_data_memory(), false };
        if (counter == 0) {
            FK_ASSERT(storage.clear());
        }
        else {
            FK_ASSERT(storage.begin());
        }

        if ((counter % 50) == 0) {
            for (auto i = 0; i < 10; ++i) {
                StaticPool<1024> pool{ "signed-log" };

                auto meta_file = storage.file(Storage::Meta);
                if (!meta_file.seek_end()) {
                    FK_ASSERT(meta_file.create());
                }

                gs.get()->general.recording++;

                MetaOps meta_ops{ storage };
                FK_ASSERT(meta_ops.write_state(gs.get(), pool));

                FK_ASSERT(meta_file.seek_end());

                auto g = storage.geometry();
                if (meta_file.size() > g.block_size + 2048) {
                    done = true;
                    break;
                }
            }
        }

        auto data_file = storage.file(Storage::Data);
        if (!data_file.seek_end()) {
            FK_ASSERT(data_file.create());
        }
        write_reading(data_file);

        counter++;
    }

    {
        NoopProgressCallbacks progress;
        Storage storage{ MemoryFactory::get_data_memory(), false };
        FK_ASSERT(storage.begin());
        FK_ASSERT(storage.fsck(&progress));
    }
}

void fk_live_tests() {
    if (false) {
        try_and_reproduce_weird_block_issue();
    }
}

}
