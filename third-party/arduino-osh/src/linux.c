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
#if !defined(ARDUINO)

#include <stdio.h>
#include <unistd.h>

#include "os.h"
#include "printf.h"
#include "internal.h"

uint32_t osi_printf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    uint32_t i = vfprintf(stderr, f, args);
    va_end(args);
    return i;
}

uint32_t osi_vprintf(const char *f, va_list args) {
    return vfprintf(stderr, f, args);
}

os_status_t osi_platform_setup() {
    return OSS_SUCCESS;
}

static uint32_t linux_uptime = 0;

uint32_t tests_platform_time(uint32_t time) {
    return linux_uptime = time;
}

uint32_t osi_platform_uptime() {
    return linux_uptime;
}

uint32_t osi_platform_delay(uint32_t ms) {
    usleep(ms * 1000);
    return ms;
}

uint32_t osi_platform_micros() {
    return 0;
}

void __disable_irq() {
}

void __enable_irq() {
}

#endif /* ARDUINO */
