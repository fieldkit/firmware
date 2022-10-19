#include <tiny_printf.h>
#include <samd51_common.h>

#include "live_tests.h"
#include "common.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "hal/random.h"
#include "hal/display.h"
#include "hal/battery_gauge.h"
#include "hal/sd_card.h"
#include "hal/hal.h"
#include "state_ref.h"
#include "state_manager.h"
#include "core_dump.h"

#include "../modules/weather/main/weather.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static void scan_i2c_radio_bus() __attribute__((unused));

static void scan_i2c_module_bus() __attribute__((unused));

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

static void scan_bus(TwoWireWrapper &bus) {
    for (auto i = 0u; i < 128u; ++i) {
        auto rv = bus.write(i, nullptr, 0);
        if (I2C_CHECK(rv)) {
            loginfo("  found 0x%x", i);
        }
    }
}

static void scan_i2c_module_bus() {
    auto mm = get_modmux();

    mm->begin();

    mm->disable_all_modules();

    fk_delay(1000);

    mm->enable_all_modules();

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    while (true) {
        for (auto i : { ModulePosition::from(1), ModulePosition::from(2), ModulePosition::from(3), ModulePosition::from(4) }) {
            if (!mm->choose(i)) {
                loginfo("unable to choose %d", i.integer());
                continue;
            }

            loginfo("position: %d", i.integer());

            fk_delay(100);

            scan_bus(bus);
        }

        fk_delay(10000);
    }
}

static bool write_header_file(const char *name, ModuleHeader &header, Pool &pool) {
    auto sd = get_sd_card();

    auto file = sd->open(name, OpenFlags::Write, pool);
    if (file == nullptr || !*file) {
        return false;
    }

    if (file->write((uint8_t *)&header, sizeof(ModuleHeader)) != sizeof(ModuleHeader)) {
        return false;
    }

    file->close();

    return true;
}

static void write_headers() {
    StandardPool pool{ "write-headers " };

    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    ModuleHeader header_weather = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WEATHER,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_ph = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_PH,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_ec = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_EC,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_do = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_DO,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_temp = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_TEMP,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_orp = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_ORP,
        .version = 0x01,
        .id = { 0 },
    };

    write_header_file("weather.hdr", header_weather, pool);
    write_header_file("w-ph.hdr", header_water_ph, pool);
    write_header_file("w-ec.hdr", header_water_ec, pool);
    write_header_file("w-do.hdr", header_water_do, pool);
    write_header_file("w-temp.hdr", header_water_temp, pool);
    write_header_file("w-orp.hdr", header_water_orp, pool);
    if (false) {
        write_header_file("fk-program.cfg", header_weather, pool);
    }
}

static void test_core_dump() {
    // fk_core_dump("livetest");
    // fk_core_dump(nullptr);

    fk_core_dump_tasks();

    while (true) {
        fk_delay(1000);
    }
}

void fk_live_tests() {
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
    if (false) {
        write_headers();
    }
    if (false) {
        test_core_dump();

        while (true) {
            fk_delay(1000);
        }
    }
}

} // namespace fk
