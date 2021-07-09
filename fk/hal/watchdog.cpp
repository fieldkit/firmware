#include <tiny_printf.h>

#if defined(__SAMD51__)
#include <hal_wdt.h>
#endif
#if defined(linux)
#endif

#include "hal/hal.h"

namespace fk {

EnableWatchdog::EnableWatchdog() {
    fk_wdt_enable();
}

EnableWatchdog::~EnableWatchdog() {
    fk_wdt_disable();
}

} // namespace fk

extern "C" {

#if defined(__SAMD51__)

struct wdt_descriptor WDT_0;

int32_t fk_wdt_initialize() {
	hri_mclk_set_APBAMASK_WDT_bit(MCLK);
	wdt_init(&WDT_0, WDT);
    return 0;
}

int32_t fk_wdt_enable() {
    uint32_t clk_rate = 1000;
    uint16_t to_period = 4096;
    wdt_set_timeout_period(&WDT_0, clk_rate, to_period);
    wdt_enable(&WDT_0);
    alogf(LogLevels::INFO, "watchdog", "wdt:enabled");
    return 0;
}

int32_t fk_wdt_disable() {
    wdt_disable(&WDT_0);
    alogf(LogLevels::INFO, "watchdog", "wdt:disabled");
    return 0;
}

int32_t fk_wdt_feed() {
    wdt_feed(&WDT_0);
    return 0;
}

#endif

#if defined(linux)

int32_t fk_wdt_initialize() {
    return 0;
}

int32_t fk_wdt_enable() {
    return 0;
}

int32_t fk_wdt_disable() {
    return 0;
}

int32_t fk_wdt_feed() {
    return 0;
}

#endif

}
