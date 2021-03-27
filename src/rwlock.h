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
#ifndef OS_RWLOCK_H
#define OS_RWLOCK_H

#if defined(__cplusplus)
extern "C" {
#endif

os_status_t osi_rwlock_create(os_rwlock_t *rwlock, os_rwlock_definition_t *def);
os_status_t osi_rwlock_acquire_read(os_rwlock_t *rwlock, uint32_t to);
os_status_t osi_rwlock_acquire_write(os_rwlock_t *rwlock, uint32_t to);
os_status_t osi_rwlock_release(os_rwlock_t *rwlock);

#if defined(__cplusplus)
}
#endif

#endif
