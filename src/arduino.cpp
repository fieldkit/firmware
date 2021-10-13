/**
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
#if defined(ARDUINO)

#include "os.h"
#include "printf.h"
#include "internal.h"

#include <Arduino.h>

extern "C" {

static void serial_putchar(char c, void *arg) {
    if (c != 0) {
#if defined(OS_CONFIG_DEBUG_RTT)
        SEGGER_RTT_PutChar(0, c);
#else
        Serial.print(c);
#endif
    }
}

uint32_t osi_printf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    auto i = os_vfctprintf(serial_putchar, NULL, f, args);
    va_end(args);
    return i;
}

uint32_t osi_vprintf(const char *f, va_list args) {
    auto i = os_vfctprintf(serial_putchar, NULL, f, args);
    return i;
}

os_status_t osi_platform_setup() {
    return OSS_SUCCESS;
}

uint32_t osi_platform_uptime() {
    return millis();
}

uint32_t osi_platform_delay(uint32_t ms) {
    delay(ms);
    return ms;
}

uint32_t osi_platform_micros() {
    return micros();
}

extern void SysTick_DefaultHandler(void);

int32_t sysTickHook(void) {
    SysTick_DefaultHandler();

    osi_irs_systick();

    return 1;
}
}

#endif
