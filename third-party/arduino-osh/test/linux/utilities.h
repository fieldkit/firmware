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
#ifndef OS_TESTS_UTILITIES_H
#define OS_TESTS_UTILITIES_H

#include <gtest/gtest.h>
#include <iostream>

#include <os.h>

os_task_t *tests_task_switch(void);

os_task_t *tests_schedule_task_and_switch();

os_task_t *tests_sleep_task(os_task_t &task);

os_task_t *tests_sleep_running_task();

void tests_dump_runqueue();

void tests_dump_waitqueue();

void task_handler_idle(void *p);

void task_handler_test(void *p);

template <size_t T, size_t NS, size_t SS> static void two_tasks_setup(os_task_t (&tasks)[T], uint32_t (&stacks)[NS][SS]) {
    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[1], "task-1", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[1], sizeof(stacks[1])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);
    tests_schedule_task_and_switch();
}

template <size_t T, size_t NS, size_t SS> void three_tasks_setup(os_task_t (&tasks)[T], uint32_t (&stacks)[NS][SS]) {
    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[1], "task-1", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[1], sizeof(stacks[1])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[2], "task-2", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[2], sizeof(stacks[2])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);
    tests_schedule_task_and_switch();
}

template <size_t T, size_t NS, size_t SS> void four_tasks_setup(os_task_t (&tasks)[T], uint32_t (&stacks)[NS][SS]) {
    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[1], "task-1", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[1], sizeof(stacks[1])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[2], "task-2", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[2], sizeof(stacks[2])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[3], "task-3", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[3], sizeof(stacks[3])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);
    tests_schedule_task_and_switch();
}

template <size_t T, size_t NS, size_t SS> void five_tasks_setup(os_task_t (&tasks)[T], uint32_t (&stacks)[NS][SS]) {
    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[1], "task-1", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[1], sizeof(stacks[1])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[2], "task-2", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[2], sizeof(stacks[2])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[3], "task-3", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[3], sizeof(stacks[3])),
              OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[4], "task-4", OS_TASK_START_RUNNING, &task_handler_test, NULL, stacks[4], sizeof(stacks[4])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);
    tests_schedule_task_and_switch();
}

void PrintTo(os_task_t *task, std::ostream *os);
void PrintTo(volatile os_task_t *task, std::ostream *os);

#endif // OS_TESTS_UTILITIES_H
