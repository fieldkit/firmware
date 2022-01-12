#include <gtest/gtest.h>

#include <os.h>
#include <internal.h>

#include "utilities.h"

class ScheduleSuite : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();
};

void ScheduleSuite::SetUp() {
    tests_platform_time(0);
}

void ScheduleSuite::TearDown() {
    ASSERT_EQ(os_teardown(), OSS_SUCCESS);
}

TEST_F(ScheduleSuite, OneTask_Initialize) {
    os_task_t tasks[1];
    uint32_t stacks[1][OS_STACK_MINIMUM_SIZE_WORDS];

    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);

    ASSERT_EQ(osg.ntasks, 1);
    ASSERT_EQ(osg.idle, &tasks[0]);
    ASSERT_EQ(osg.runqueue, &tasks[0]);
    ASSERT_EQ(osg.running, &tasks[0]);
    ASSERT_EQ(osg.scheduled, nullptr);

    ASSERT_EQ(tasks[0].status, OS_TASK_STATUS_ACTIVE);
    ASSERT_EQ(tasks[0].stack_size, sizeof(stacks[0]));
    ASSERT_EQ(tasks[0].stack, &stacks[0][0]);
    ASSERT_EQ(tasks[0].sp, &stacks[0][OS_STACK_MINIMUM_SIZE_WORDS - OS_STACK_BASIC_FRAME_SIZE]);
    ASSERT_STREQ(tasks[0].name, "idle");
    ASSERT_EQ(tasks[0].priority, OS_PRIORITY_IDLE);
    ASSERT_EQ(tasks[0].handler, &task_handler_idle);
    ASSERT_EQ(tasks[0].flags, 0);
    ASSERT_EQ(tasks[0].delay, 0);
    ASSERT_EQ(tasks[0].queue, nullptr);
    ASSERT_EQ(tasks[0].mutex, nullptr);
    ASSERT_EQ(tasks[0].c.message, nullptr);
    ASSERT_EQ(tasks[0].nblocked, nullptr);
    ASSERT_EQ(tasks[0].nrp, nullptr);
    ASSERT_EQ(tasks[0].np, nullptr);

    auto sp = (os_our_sframe_t *)tasks[0].sp;
    ASSERT_EQ(sp->basic.xpsr, 0x01000000);
    ASSERT_EQ(sp->basic.pc, ((uint32_t)(uintptr_t)task_handler_idle) & ~0x01UL);
    ASSERT_NE(sp->basic.lr, 0x0);
    ASSERT_EQ(sp->basic.r0, 0x0);
    ASSERT_EQ(stacks[0][0], OSH_STACK_MAGIC_WORD);
}

TEST_F(ScheduleSuite, OneTask_Schedule) {
    os_task_t tasks[1];
    uint32_t stacks[1][OS_STACK_MINIMUM_SIZE_WORDS];

    ASSERT_EQ(os_initialize(), OSS_SUCCESS);
    ASSERT_EQ(os_task_initialize(&tasks[0], "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, stacks[0], sizeof(stacks[0])),
              OSS_SUCCESS);
    ASSERT_EQ(os_start(), OSS_SUCCESS);

    ASSERT_EQ(osg.scheduled, nullptr);

    ASSERT_EQ(osi_schedule(), OSS_SUCCESS);
    /* No other tasks to schedule. */
    ASSERT_EQ(osg.scheduled, nullptr);
}

TEST_F(ScheduleSuite, TwoTasks_Initialize) {
    os_task_t tasks[2];
    uint32_t stacks[2][OS_STACK_MINIMUM_SIZE_WORDS];

    two_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.ntasks, 2);
    ASSERT_EQ(osg.idle, &tasks[0]);
    ASSERT_EQ(osg.runqueue, &tasks[1]);
    ASSERT_EQ(osg.runqueue->nrp, &tasks[0]);
    ASSERT_EQ(osg.running, &tasks[1]);
    ASSERT_EQ(osg.scheduled, nullptr);
    ASSERT_EQ(osg.tasks, &tasks[1]);
    ASSERT_EQ(osg.tasks->np, &tasks[0]);

    ASSERT_EQ(tasks[0].status, OS_TASK_STATUS_IDLE);
    ASSERT_EQ(tasks[1].status, OS_TASK_STATUS_ACTIVE);

    ASSERT_EQ(tasks[0].priority, OS_PRIORITY_IDLE);
    ASSERT_EQ(tasks[1].priority, OS_PRIORITY_NORMAL);
}

TEST_F(ScheduleSuite, TwoTasks_Schedule) {
    os_task_t tasks[2];
    uint32_t stacks[2][OS_STACK_MINIMUM_SIZE_WORDS];

    two_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.scheduled, nullptr);

    /* Run scheduler, no scheduled task though, no lower priority tasks. */
    ASSERT_EQ(osi_schedule(), OSS_SUCCESS);
    ASSERT_EQ(osg.scheduled, nullptr);

    ASSERT_EQ(&tasks[1], osg.running);

    /* Current task is waiting, idle can be scheduled now. */
    tests_sleep_task(tasks[1]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[0]);
}

TEST_F(ScheduleSuite, TwoTasks_SleepsThenWakes) {
    os_task_t tasks[2];
    uint32_t stacks[2][OS_STACK_MINIMUM_SIZE_WORDS];

    two_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.scheduled, nullptr);

    /* Run scheduler, no scheduled task though, no lower priority tasks. */
    ASSERT_EQ(osi_schedule(), OSS_SUCCESS);
    ASSERT_EQ(osg.scheduled, nullptr);

    tests_platform_time(1000);

    /* Current task is waiting, idle can be scheduled now. */
    ASSERT_EQ(tests_sleep_running_task(), &tasks[0]);

    /* Just before the time the other task is supposed to awake. */
    tests_platform_time(1999);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[0]);

    ASSERT_EQ(tasks[1].delay, 2000);

    /* Task should awake now. */
    tests_platform_time(2000);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tasks[1].delay, 0);
}

TEST_F(ScheduleSuite, TwoCompetingTasks_Schedule) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.scheduled, nullptr);

    /* task-b will get a chance. */
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    /* task-a will get a chance. */
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
}

TEST_F(ScheduleSuite, ManyTasks_ScheduleAsTasksGraduallySleep) {
    os_task_t tasks[5];
    uint32_t stacks[5][OS_STACK_MINIMUM_SIZE_WORDS];

    five_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.scheduled, nullptr);

    /* Tasks get a chance in order. */
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[3]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[4]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    /* Gradually sleep tasks... */
    ASSERT_EQ(tests_sleep_running_task(), &tasks[1]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[3]);
    ;

    /* Gradually sleep tasks... */
    ASSERT_EQ(tests_sleep_running_task(), &tasks[1]);

    /* Gradually sleep tasks... */
    ASSERT_EQ(tests_sleep_running_task(), &tasks[4]);

    /* Gradually sleep tasks... */
    ASSERT_EQ(tests_sleep_running_task(), &tasks[0]);
}

TEST_F(ScheduleSuite, ThreeTasks_HigherPrioritySuspended) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    // Give task-1 higher priority, this works because it's already the first
    // one in the runqueue, otherwise we'd need to resort.
    tasks[1].priority += 0x10;

    // Really drive the point home.
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(os_task_suspend(&tasks[1]), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    ASSERT_EQ(os_task_resume(&tasks[1]), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
}

TEST_F(ScheduleSuite, ThreeTasks_HigherPrioritySleeps) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    // Give task-1 higher priority, this works because it's already the first
    // one in the runqueue, otherwise we'd need to resort.
    tasks[1].priority += 0x10;

    // Really drive the point home.
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    // Alright, sleep the task.
    ASSERT_EQ(tests_sleep_running_task(), &tasks[2]);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    tests_platform_time(999);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    tests_platform_time(1000);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);
}
