#include <gtest/gtest.h>

#include <os.h>
#include <internal.h>

#include "utilities.h"

class MutexesSuite : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
};

void MutexesSuite::SetUp() {
    tests_platform_time(0);
}

void MutexesSuite::TearDown() {
    ASSERT_EQ(os_teardown(), OSS_SUCCESS);
}

TEST_F(MutexesSuite, ThreeTasks_Mutex_AcquireRelease) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    os_mutex_t mutex;
    os_mutex_definition_t def = { "mutex" };
    ASSERT_EQ(osi_mutex_create(&mutex, &def), OSS_SUCCESS);

    tests_schedule_task_and_switch();
    tests_schedule_task_and_switch();

    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_SUCCESS);
    ASSERT_EQ(mutex.owner, &tasks[1]);
    ASSERT_EQ(mutex.level, 1);
    ASSERT_EQ(tasks[1].mutex, nullptr);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(osi_mutex_release(&mutex), OSS_SUCCESS);

    ASSERT_EQ(mutex.owner, nullptr);
    ASSERT_EQ(mutex.level, 0);
    ASSERT_EQ(tasks[1].mutex, nullptr);
}

TEST_F(MutexesSuite, ThreeTasks_Mutex_AcquireBlockRelease) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    os_mutex_t mutex;
    os_mutex_definition_t def = { "mutex" };
    ASSERT_EQ(osi_mutex_create(&mutex, &def), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_SUCCESS);
    ASSERT_EQ(mutex.owner, &tasks[1]);
    ASSERT_EQ(mutex.level, 1);
    ASSERT_EQ(tasks[1].mutex, nullptr);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    /* task-2 tries to acquire and fails, unschedules itself so we just switch. */
    osi_task_set_stacked_return(&tasks[2], OSS_ERROR_TO);
    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_ERROR_TO);
    ASSERT_EQ(mutex.blocked.tasks, &tasks[2]);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    /* task-2 is blocked for now. */
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    /* task-1 releases the mutex and will dispatch task-2 */
    ASSERT_EQ(osi_mutex_release(&mutex), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(mutex.owner, &tasks[2]);
    ASSERT_EQ(mutex.level, 1);
    ASSERT_EQ(tasks[1].mutex, nullptr);
    ASSERT_EQ(tasks[2].mutex, nullptr);
    ASSERT_EQ(mutex.blocked.tasks, nullptr);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[2]), OSS_SUCCESS);
}

TEST_F(MutexesSuite, FourTasks_Mutex_AcquireBlockRelease) {
    os_task_t tasks[4];
    uint32_t stacks[4][OS_STACK_MINIMUM_SIZE_WORDS];

    four_tasks_setup(tasks, stacks);

    os_mutex_t mutex;
    os_mutex_definition_t def = { "mutex" };
    ASSERT_EQ(osi_mutex_create(&mutex, &def), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_SUCCESS);
    ASSERT_EQ(mutex.owner, &tasks[2]);
    ASSERT_EQ(mutex.level, 1);
    ASSERT_EQ(tasks[1].mutex, nullptr);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[3]);

    /* task-3 tries to acquire and fails, unschedules itself so we just switch. */
    osi_task_set_stacked_return(&tasks[3], OSS_ERROR_TO);
    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_ERROR_TO);
    ASSERT_EQ(mutex.blocked.tasks, &tasks[3]);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    /* task-1 tries to acquire and fails, unschedules itself so we just switch. */
    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_ERROR_TO);
    ASSERT_EQ(mutex.blocked.tasks, &tasks[3]);
    ASSERT_EQ(mutex.blocked.tasks->nblocked, &tasks[1]); /* Appended */
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(tasks[1].mutex, &mutex);
    ASSERT_EQ(tasks[3].mutex, &mutex);

    /* release */
    ASSERT_EQ(osi_mutex_release(&mutex), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[3]);
    ASSERT_EQ(mutex.blocked.tasks, &tasks[1]);

    ASSERT_EQ(tasks[1].mutex, &mutex);
    ASSERT_EQ(tasks[3].mutex, nullptr);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[3]), OSS_SUCCESS);
    ASSERT_EQ(osi_task_get_stacked_return(&tasks[1]), OSS_ERROR_TO);

    /* release */
    ASSERT_EQ(osi_mutex_release(&mutex), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);
    ASSERT_EQ(mutex.blocked.tasks, nullptr);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[1]), OSS_SUCCESS);
}

TEST_F(MutexesSuite, ThreeTasks_Mutex_AcquireBlockTimeOut) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    os_mutex_t mutex;
    os_mutex_definition_t def = { "mutex" };
    ASSERT_EQ(osi_mutex_create(&mutex, &def), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_SUCCESS);
    ASSERT_EQ(mutex.owner, &tasks[1]);
    ASSERT_EQ(mutex.level, 1);
    ASSERT_EQ(tasks[1].mutex, nullptr);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    /* task-2 tries to acquire and fails, unschedules itself so we just switch. */
    osi_task_set_stacked_return(&tasks[2], OSS_ERROR_TO);
    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_ERROR_TO);
    ASSERT_EQ(mutex.blocked.tasks, &tasks[2]);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    /* task-2 is blocked for now. */
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    /* time passes and task-2 gives up */
    tests_platform_time(499);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    tests_platform_time(500);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    /* task-1 releases the mutex */
    ASSERT_EQ(osi_mutex_release(&mutex), OSS_SUCCESS);

    ASSERT_EQ(mutex.owner, nullptr);
    ASSERT_EQ(mutex.level, 0);
    ASSERT_EQ(tasks[1].mutex, nullptr);
    ASSERT_EQ(tasks[2].mutex, nullptr);
    ASSERT_EQ(mutex.blocked.tasks, nullptr);
}

TEST_F(MutexesSuite, ThreeTasks_HigherPriorityTaskBlockedAcquiring) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    os_mutex_t mutex;
    os_mutex_definition_t def = { "mutex" };
    ASSERT_EQ(osi_mutex_create(&mutex, &def), OSS_SUCCESS);

    // Give task-1 higher priority, this works because it's already the first
    // one in the runqueue, otherwise we'd need to resort.
    tasks[1].priority += 0x10;
    // Sleep, so that a lower priority task can grab the mutex.
    ASSERT_EQ(tests_sleep_running_task(), &tasks[2]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_SUCCESS);
    ASSERT_EQ(mutex.owner, &tasks[2]);

    // Bring back the higher priority task.
    tests_platform_time(1000);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    // Try acquiring the mutex.
    ASSERT_EQ(osi_mutex_acquire(&mutex, 500), OSS_ERROR_TO);

    // TODO: DEADLOCK
}
