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
#ifndef OS_SEMAPHORE_H
#define OS_SEMAPHORE_H

#if defined(__cplusplus)
extern "C" {
#endif

os_status_t osi_semaphore_create(os_semaphore_t *semaphore, os_semaphore_definition_t *def);
os_status_t osi_semaphore_acquire(os_semaphore_t *semaphore, uint32_t to);
os_status_t osi_semaphore_release(os_semaphore_t *semaphore);

#if defined(__cplusplus)
}
#endif

#endif
