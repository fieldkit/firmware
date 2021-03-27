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
#ifndef OS_EXTERNAL_H
#define OS_EXTERNAL_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *
 */
os_status_t osi_platform_setup();

/**
 *
 */
uint32_t osi_platform_uptime();

/**
 * Delay to use when the operating system can't handle the request, either
 * because we haven't been started yet or because we're not inside a task.
 */
uint32_t osi_platform_delay(uint32_t ms);

uint32_t osi_platform_micros();

#if defined(__cplusplus)
}
#endif

#endif /* OS_EXTERNAL_H */
