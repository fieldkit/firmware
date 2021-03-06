#include <tiny_printf.h>
#include <samd51_common.h>

#include "live_tests.h"
#include "common.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "hal/random.h"
#include "hal/display.h"
#include "hal/battery_gauge.h"
#include "state_ref.h"
#include "clock.h"

#include "../modules/weather/main/weather.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static void scan_i2c_radio_bus() __attribute__((unused));

static void scan_i2c_module_bus() __attribute__((unused));

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
    loginfo("%s", __PRETTY_FUNCTION__);

    StandardPool pool{ "live-tests" };

    auto gs = get_global_state_rw();
    auto counter = 0;
    auto done = false;

    while (!done) {
        Storage storage{ MemoryFactory::get_data_memory(), pool, false };
        if (counter == 0) {
            FK_ASSERT(storage.clear());
        }
        else {
            FK_ASSERT(storage.begin());
        }

        if ((counter % 50) == 0) {
            for (auto i = 0; i < 10; ++i) {
                StandardPool pool{ "signed-log" };

                auto meta_file = storage.file(Storage::Meta);
                if (!meta_file.seek_end()) {
                    FK_ASSERT(meta_file.create());
                }

                gs.get()->general.recording++;

                MetaOps meta_ops{ storage };
                FK_ASSERT(meta_ops.write_state(gs.get(), &fkb_header, pool));

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
        Storage storage{ MemoryFactory::get_data_memory(), pool, false };
        FK_ASSERT(storage.begin());
        FK_ASSERT(storage.fsck(&progress));
    }
}

static void fsck_and_stop() {
    loginfo("%s", __PRETTY_FUNCTION__);

    FK_ASSERT(MemoryFactory::get_data_memory()->begin());

    StandardPool pool{ "live-tests" };
    NoopProgressCallbacks progress;
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    FK_ASSERT(storage.begin());
    FK_ASSERT(storage.fsck(&progress));

    while (true) {
        fk_delay(100);
    }
}

template<typename T>
static uint32_t calculate_crc(uint32_t seed, T &object) {
    return crc32_checksum(seed, (uint8_t *)&object, sizeof(T) - sizeof(uint32_t));
}

static bool read_weather_eeprom(uint32_t &last_address, fk_weather_t &last_record) {
    auto module_bus = get_board()->i2c_module();
    auto eeprom = ModuleEeprom{ module_bus };

    get_modmux()->choose((ModulePosition)6);

    uint32_t iter = EEPROM_ADDRESS_READINGS;
    while (true) {
        if (iter + sizeof(fk_weather_t) >= EEPROM_ADDRESS_READINGS_END) {
            break;
        }

        fk_weather_t temp;
        if (!eeprom.read_data(iter, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return false;
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            loginfo("record 0x%04" PRIx32 " startups=%" PRIu32 " seconds=%" PRIu32, iter, temp.startups, temp.seconds);
            last_address = iter;
            last_record = temp;
        }

        break;

        iter += sizeof(fk_weather_t);
    }

    return true;
}

/**
 * This may be a red herring because of the disable everything calls screwing
 * with the state of things. Saved anyway for some future analysis.
 */
static void try_and_break_module_bus() {
    loginfo("%s", __PRETTY_FUNCTION__);

    get_board()->disable_everything();

    fk_delay(500);

    uint32_t last_address = 0;
    fk_weather_t last_record;
    bzero(&last_record, sizeof(fk_weather_t));

    while (true) {
        if (true) {
            auto lock = get_modmux()->lock();

            get_board()->enable_everything();

            fk_delay(100);

            get_modmux()->choose((ModulePosition)6);

            fk_delay(100);

            loginfo("reading eeprom");

            uint32_t address = 0;
            fk_weather_t record;
            bzero(&record, sizeof(fk_weather_t));

            if (read_weather_eeprom(address, record)) {
                loginfo("found 0x%04" PRIx32 " 0x%04" PRIx32 " startups=%" PRIu32, last_address, address, record.startups);
                if (record.startups > last_record.startups) {
                    last_record = record;
                    last_address = address;
                }
            }

            loginfo("done");

            fk_delay(100);
        }

        auto delay = fk_random_i32(1000, 6000);
        loginfo("waiting %" PRIu32 "ms", delay);
        fk_delay(delay);

        {
            auto lock = get_modmux()->lock();
            fk_delay(100);

            loginfo("disabling");
            get_board()->disable_everything();

            loginfo("waiting 1s");
            fk_delay(1000);
        }
    }
}

static void try_and_break_weather_sensor_bus() {
    loginfo("%s", __PRETTY_FUNCTION__);

    auto display = get_display();

    display->off();

    get_board()->enable_everything();

    auto mm = get_modmux();

    mm->disable_all_modules();

    fk_delay(500);

    uint32_t last_address = 0;
    fk_weather_t last_record;
    bzero(&last_record, sizeof(fk_weather_t));

    char message[64] = { 0 };
    display->simple(SimpleScreen{ message });

    while (true) {
        bool reproduced = false;

        if (true) {
            auto lock = get_modmux()->lock();

            if (!mm->enable_all_modules()) {
                logerror("error enabling modules");
            }

            fk_delay(100);

            if (!get_modmux()->choose((ModulePosition)6)) {
                logerror("error choosing module bay");
            }

            fk_delay(100);

            loginfo("reading eeprom");

            uint32_t address = 0;
            fk_weather_t record;
            bzero(&record, sizeof(fk_weather_t));

            if (read_weather_eeprom(address, record)) {
                loginfo("found 0x%04" PRIx32 " 0x%04" PRIx32 " startups=%" PRIu32 " reading-failures=%" PRIu32, last_address, address, record.startups, record.reading_failures);

                if (record.reading_failures == 6) {
                    reproduced = true;
                }
                last_record = record;
                last_address = address;

                tiny_snprintf(message, sizeof(message), "%" PRIu32, record.startups);
                display->simple(SimpleScreen{ message });
            }

            loginfo("done");

            fk_delay(100);
        }

        if (reproduced) {
            loginfo("bingo!");
            display->company_logo();
            while (true) {
                fk_delay(1000);
            }
        }

        auto delay = fk_random_i32(1000, 6000);
        loginfo("waiting %" PRIu32 "ms", delay);
        fk_delay(delay);

        // NOTE Right now this is the only way to reliably get the modules to restart.
        fk_restart();

        {
            auto lock = get_modmux()->lock();
            fk_delay(100);

            loginfo("disabling and waiting 10s");
            if (!mm->disable_all_modules()) {
                logerror("error disabling modules");
            }
            fk_delay(10000);
        }
    }
}

static void watch_battery() {
    get_board()->enable_everything();

    while (true) {
        auto gauge = get_battery_gauge();

        if (!gauge->begin()) {
            logerror("battery gauge failed");
        }
        else {
            gauge->get();
        }

        fk_delay(1000);
    }
}

static void scan_i2c_radio_bus() {
    auto bus = get_board()->i2c_radio();

    bus.begin();

    get_board()->enable_lora();

    fk_delay(1000);

    while (true) {
        loginfo("scanning");

        for (auto i = 0u; i < 128u; ++i) {
            auto rv = bus.write(i, nullptr, 0);
            if (I2C_CHECK(rv)) {
                loginfo("  found 0x%x", i);
            }
        }

        fk_delay(1000);
    }
}

static void scan_i2c_module_bus() {
    auto mm = get_modmux();

    mm->disable_all_modules();

    fk_delay(1000);

    mm->enable_all_modules();

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    while (true) {
        for (auto i : { ModulePosition::from(0) }) {
            if (!mm->choose(i)) {
                loginfo("unable to choose %d", i.integer());
                continue;
            }

            loginfo("position: %d", i.integer());

            fk_delay(100);

            for (auto i = 0u; i < 128u; ++i) {
                auto rv = bus.write(i, nullptr, 0);
                if (I2C_CHECK(rv)) {
                    loginfo("  found 0x%x", i);
                }
            }
        }

        fk_delay(10000);
    }
}

static void test_i2c_weather_samd09() {
    auto mm = get_modmux();

    mm->disable_all_modules();

    fk_delay(1000);

    mm->enable_all_modules();

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    while (true) {
        for (auto i : { ModulePosition::from(3) }) {
            if (!mm->choose(i)) {
                loginfo("unable to choose %d", i.integer());
                continue;
            }

            loginfo("position: %d", i.integer());

            fk_delay(100);

            if (!I2C_CHECK(bus.write_register_u32(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, 0xaabbccdd))) {
                logwarn("error setting time");
            }

            fk_weather_aggregated_t aw;
            bzero(&aw, sizeof(aw));
            if (!I2C_CHECK(bus.read_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_READ, (uint8_t *)&aw, sizeof(aw)))) {
                logwarn("error reading");
            }
            else {
                loginfo("weather: %ds %dm (%d, %d)", aw.second, aw.minute, aw.counter_120s, aw.counter_10m);
                loginfo("weather: humidity = %" PRIu32, aw.humidity);
                loginfo("weather: temperature_1 = %" PRIu32, aw.temperature_1);
                loginfo("weather: pressure = %" PRIu32, aw.pressure);
                loginfo("weather: temperature_2 = %" PRIu32, aw.temperature_2);
                loginfo("weather: skipping wind");
                loginfo("weather: skipping rain");
                loginfo("weather: crc = %" PRIu32, aw.crc);

                loginfo("weather: humidity = %f", 100.0f * ((float)aw.humidity / (0xffff)));
                loginfo("weather: temperature_1 = %f", -45.0f + 175.0f * ((float)aw.temperature_1 / (0xffff)));
                loginfo("weather: pressure = %f", aw.pressure / 64.0f / 1000.0f);
                loginfo("weather: temperature_2 = %f", aw.temperature_2 / 16.0f);
            }
        }

        fk_delay(10000);
    }
}

static void https_test() {
    StandardPool main_pool{ "https-test" };

    get_board()->enable_everything();

    auto network = get_network();

    network->begin({
        .valid = true,
        .create = false,
        .ssid = "Conservify",
        .password = "Okavang0",
        }, &main_pool);

    while (network->status() != NetworkStatus::Connected) {
        fk_delay(1000);
        loginfo("connecting...");
    }

    loginfo("connected");

    while (true) {
        StandardPool pool{ "test" };
        auto url = "https://api.fkdev.org/status";
        auto http = open_http_connection("GET", url, "", true, pool);
        if (http != nullptr) {
            http->close();
        }

        fk_delay(1000);

        loginfo("ping");
    }
}

static void chase_upload_truncation() {
    loginfo("%s", __PRETTY_FUNCTION__);

    FK_ASSERT(MemoryFactory::get_data_memory()->begin());

    log_configure_level(LogLevels::TRACE);

    StandardPool pool{ "live-tests" };
    NoopProgressCallbacks progress;
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    FK_ASSERT(storage.begin());

    loginfo("storage ready");

    // 389383 -> 390151 41682 bytes
    // Start position is the same for both, it's the ending position.
    // that ends up wrong. Which means we definitely seem to be
    // truncating during the read.

    if (true) {
        uint32_t first_block = 389383;
        uint32_t last_block = 390151;
        auto file = storage.file(Storage::Data);
        auto size_info = file.get_size(first_block, last_block, pool);
        loginfo("size-info(one beyond): size = %" PRIu32 " %" PRIu32, size_info.size, size_info.last_block);
    }

    if (true) {
        uint32_t first_block = 389383;
        auto file = storage.file(Storage::Data);
        auto size_info = file.get_size(first_block, UINT32_MAX, pool);
        loginfo("size-info(max): size = %" PRIu32 " %" PRIu32, size_info.size, size_info.last_block);
    }

    if (false) {
        auto file = storage.file(Storage::Data);
        file.walk(389383, 390151, pool);
    }

    while (true) {
        fk_delay(100);
    }
}

static void sleep_test() {
    loginfo("sleep test");

    auto display = get_display();

    display->simple(SimpleScreen{ "Start" });

    while (true) {
        display->simple(SimpleScreen{ "2s" });

        fk_delay(2000);

        display->simple(SimpleScreen{ "Sleep" });

        auto duration = fk_deep_sleep(4000);
        loginfo("awake: %" PRIu32, duration);
    }
}

void fk_live_tests() {
    if (false) {
        chase_upload_truncation();
    }
    if (false) {
        fsck_and_stop();
    }
    if (false) {
        try_and_reproduce_weird_block_issue();
    }
    if (false) {
        try_and_break_module_bus();
    }
    if (false) {
        try_and_break_weather_sensor_bus();
    }
    if (false) {
        watch_battery();
    }
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
    if (false) {
        test_i2c_weather_samd09();
    }
    if (false) {
        https_test();
    }
    if (false) {
        sleep_test();
    }
}

}
