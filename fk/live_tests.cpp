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

void fk_live_tests() {
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
}

}
