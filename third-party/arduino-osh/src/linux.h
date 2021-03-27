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
#ifndef OS_LINUX_H
#define OS_LINUX_H

#if !defined(ARDUINO)

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *
 */
uint32_t tests_platform_time(uint32_t time);

void __disable_irq();

void __enable_irq();

#if defined(__cplusplus)
}
#endif

#endif /* !ARDUINO */

#endif /* OS_LINUX_H */
