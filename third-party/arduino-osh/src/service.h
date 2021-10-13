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
#ifndef OS_SERVICE_H
#define OS_SERVICE_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *
 */
uint32_t os_delay(uint32_t ms);

/**
 *
 */
uint32_t os_block(uint32_t ms, uint32_t flags);

/**
 *
 */
uint32_t os_panic(uint32_t code);

/**
 *
 */
uint32_t os_abort(uint32_t code);

/**
 *
 */
uint32_t os_printf(const char *str, ...);

/**
 *
 */
uint32_t os_pstr(const char *str);

/**
 *
 */
os_status_t os_queue_create(os_queue_t *queue, os_queue_definition_t *def);

/**
 *
 */
os_tuple_t os_queue_enqueue(os_queue_t *queue, void *message, uint32_t to);

/**
 *
 */
os_tuple_t os_queue_dequeue(os_queue_t *queue, uint32_t to);

/**
 *
 */
os_status_t os_mutex_create(os_mutex_t *mutex, os_mutex_definition_t *def);

/**
 *
 */
os_status_t os_mutex_acquire(os_mutex_t *mutex, uint32_t to);

/**
 *
 */
os_status_t os_mutex_is_owner(os_mutex_t *mutex);

/**
 *
 */
os_status_t os_mutex_release(os_mutex_t *mutex);

os_status_t os_semaphore_create(os_semaphore_t *semaphore, os_semaphore_definition_t *def);

os_status_t os_semaphore_acquire(os_semaphore_t *semaphore, uint32_t to);

os_status_t os_semaphore_release(os_semaphore_t *semaphore);

os_status_t os_rwlock_create(os_rwlock_t *rwlock, os_rwlock_definition_t *def);
os_status_t os_rwlock_acquire_read(os_rwlock_t *rwlock, uint32_t to);
os_status_t os_rwlock_acquire_write(os_rwlock_t *rwlock, uint32_t to);
os_status_t os_rwlock_release(os_rwlock_t *rwlock);

os_status_t os_signal(os_task_t *task, uint32_t signal);
os_status_t os_signal_check(uint32_t *signal);

/**
 *
 */
#define os_word_size(n) (((sizeof(n) + sizeof(uint32_t) - 1) & ~(sizeof(uint32_t) - 1)) / sizeof(uint32_t))

/**
 *
 */
#define os_queue_define(name, size, flags)                                                                                                 \
    os_queue_definition_t _os_queue_def_##name = { #name, size, flags };                                                                   \
    uint32_t _os_queue_##name[os_word_size(os_queue_t) + (size)];

/**
 *
 */
#define os_queue(name) ((os_queue_t *)_os_queue_##name)

/**
 *
 */
#define os_queue_external(name)                                                                                                            \
    extern os_queue_definition_t _os_queue_def_##name;                                                                                     \
    extern uint32_t _os_queue_##name[];

/**
 *
 */
#define os_queue_def(name) (os_queue_definition_t *)&_os_queue_def_##name

/**
 *
 */
#define os_mutex_define(name, flags)                                                                                                       \
    os_mutex_definition_t _os_mutex_def_##name = { #name, flags };                                                                         \
    os_mutex_t _os_mutex_##name;

/**
 *
 */
#define os_mutex(name) ((os_mutex_t *)&_os_mutex_##name)

/**
 *
 */
#define os_mutex_def(name) ((os_mutex_definition_t *)&_os_mutex_def_##name)

/**
 *
 */
#define os_semaphore_define(name, size, flags)                                                                                             \
    os_semaphore_definition_t _os_semaphore_def_##name = { #name, size, flags };                                                           \
    os_semaphore_t _os_semaphore_##name;

/**
 *
 */
#define os_semaphore(name) ((os_semaphore_t *)&_os_semaphore_##name)

/**
 *
 */
#define os_semaphore_def(name) ((os_semaphore_definition_t *)&_os_semaphore_def_##name)

/**
 *
 */
#define os_rwlock_define(name, flags)                                                                                                      \
    os_rwlock_definition_t _os_rwlock_def_##name = { #name, flags };                                                                       \
    os_rwlock_t _os_rwlock_##name;

/**
 *
 */
#define os_rwlock(name) ((os_rwlock_t *)&_os_rwlock_##name)

/**
 *
 */
#define os_rwlock_def(name) ((os_rwlock_definition_t *)&_os_rwlock_def_##name)

#if defined(__cplusplus)
}
#endif

#endif
