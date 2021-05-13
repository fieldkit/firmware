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
#include "state_ref.h"
#include "state_manager.h"
#include "clock.h"

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

            scan_bus(bus);
        }

        fk_delay(10000);
    }
}

static GpsState const *get_gps_from_global_state(Pool &pool) {
    return get_global_state_ro().get()->location(pool);
}

static void test_water_module() {
    StandardPool pool{ "water-test" };

    get_board()->i2c_core().begin();

    get_board()->disable_gps();

    auto mm = get_modmux();

    mm->enable_all_modules();

    GlobalStateManager gsm;
    FK_ASSERT(gsm.initialize(pool));

    ModuleRegistry registry;
    registry.initialize();

    auto memory = MemoryFactory::get_data_memory();
    if (!memory->begin()) {
        logerror("memory");
        FK_ASSERT(0);
    }

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    auto gps = get_gps_from_global_state(pool);
    StatisticsMemory statistics{ memory };
    Storage storage{ &statistics, pool, true };
    if (!storage.begin()) {
        logerror("opening storage");
        FK_ASSERT(0);
    }

    ModuleScanning scanning{ mm };
    auto modules = scanning.scan(pool);
    if (modules) {
        loginfo("have modules");
    }

    ScanningContext ctx{ mm, gps, bus, ModulePowerState::Unknown, pool };
    auto mf = get_module_factory();
    if (!mf.initialize(ctx, pool)) {
        logerror("module factory");
        FK_ASSERT(0);
    }

    if (!mf.rescan(scanning, pool)) {
        logerror("module factory");
        FK_ASSERT(0);
    }

    if (true) {
        ModuleRegistry registry;
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_PH,
            .version = 0x01,
        };

        auto meta = registry.resolve(header);
        auto constructed = meta->ctor(pool);
        auto first_pass = true;

        while (true) {
            StandardPool loop_pool{ "water-loop" };

            for (auto position : { ModulePosition::from(1), ModulePosition::from(2), ModulePosition::from(3), ModulePosition::from(4) }) {
                loginfo("position: %d", position.integer());

                auto module_ctx = ctx.open_module(position, loop_pool);
                if (!mm->choose(position)) {
                    logerror("choose %d", position.integer());
                    while (true) {
                        fk_delay(100);
                    }
                }

                if (first_pass) {
                    scan_bus(module_ctx.module_bus());
                }

                if (constructed->initialize(module_ctx, loop_pool)) {
                    loginfo("ready!");

                    ModuleReadingsCollection all_readings{ loop_pool };
                    auto readingsCtx = ctx.open_readings(position, all_readings, loop_pool);
                    if (!readingsCtx.open()) {
                        logerror("[%d] error choosing module", position.integer());
                        continue;
                    }

                    constructed->take_readings(readingsCtx, loop_pool);
                }
            }

            fk_delay(1000);

            first_pass = false;
        }
    } else {
        for (auto &m : *modules) {
            if (true || m.header.kind == FK_MODULES_KIND_WATER_PH) {
                loginfo("module!");

                auto constructed = mf.get(m.position);
                if (!constructed) {
                    logerror("ctor");
                    continue;
                }

                auto module_ctx = ctx.open_module(m.position, pool);

                if (!mm->choose(m.position)) {
                    logerror("choose %d", m.position.integer());
                    continue;
                }

                scan_bus(module_ctx.module_bus());

                if ((*constructed)->module_instance->initialize(module_ctx, pool)) {
                    loginfo("ready!");
                }
            }
        }
    }

    while (true) {
        fk_delay(1000);
    }
}

void fk_live_tests() {
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        test_water_module();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
}

}
