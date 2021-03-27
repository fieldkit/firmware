#include <gtest/gtest.h>

#include <os.h>
#include <internal.h>

#include "utilities.h"

class QueuesSuite : public ::testing::Test {
protected:
    virtual void SetUp();
    virtual void TearDown();

};

void QueuesSuite::SetUp() {
    tests_platform_time(0);
}

void QueuesSuite::TearDown() {
    ASSERT_EQ(os_teardown(), OSS_SUCCESS);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_SendReceiveOne) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];

    three_tasks_setup(tasks, stacks);

    os_queue_define(queue, 4, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    auto message = "message-0";
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_SUCCESS);

    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);

    void *received = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received, 500), OSS_SUCCESS);

    ASSERT_EQ(received, message);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_SendBlock) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];
    const char *messages[5] = {
        "message-0",
        "message-1",
        "message-2",
        "message-3",
        "message-4",
    };

    three_tasks_setup(tasks, stacks);

    os_queue_define(queue, 4, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[0], 500), OSS_SUCCESS);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[1], 500), OSS_SUCCESS);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[2], 500), OSS_SUCCESS);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[3], 500), OSS_SUCCESS);

    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[3], 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_SEND);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));

    void *received = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received, 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, nullptr);
    ASSERT_EQ(tasks[1].queue, nullptr);

    ASSERT_EQ(received, messages[0]);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_ReceiveBlock) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];
    const char *message = "message-0";

    three_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.running, &tasks[1]);

    os_queue_define(queue, 4, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    void *received = nullptr;
    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received, 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_RECEIVE);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, nullptr);
    ASSERT_EQ(tasks[1].queue, nullptr);

    auto tuple = osi_task_stacked_return_tuple(&tasks[1]);
    ASSERT_EQ(tuple->status, OSS_SUCCESS);
    ASSERT_EQ(tuple->value.ptr, message);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_TwoReceiveBlock) {
    os_task_t tasks[4];
    uint32_t stacks[4][OS_STACK_MINIMUM_SIZE_WORDS];
    const char *messages[2] = {
        "message-0",
        "message-1",
    };

    four_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.running, &tasks[1]);

    os_queue_define(queue, 4, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    void *received1 = nullptr;
    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received1, 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_RECEIVE);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));

    void *received2 = nullptr;
    osi_task_set_stacked_return(&tasks[2], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received1, 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[3]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->blocked.tasks->nblocked, &tasks[2]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_RECEIVE);
    ASSERT_EQ(tasks[2].queue, os_queue(queue));

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[0], 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[1]), OSS_SUCCESS);
    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[2]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_RECEIVE);
    ASSERT_EQ(tasks[1].queue, nullptr);
    ASSERT_EQ(tasks[2].queue, os_queue(queue));

    auto tuple1 = osi_task_stacked_return_tuple(&tasks[1]);
    ASSERT_EQ(tuple1->status, OSS_SUCCESS);
    ASSERT_EQ(tuple1->value.ptr, messages[0]);

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[1], 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[2]), OSS_SUCCESS);
    ASSERT_EQ(os_queue(queue)->blocked.tasks, nullptr);
    ASSERT_EQ(tasks[1].queue, nullptr);
    ASSERT_EQ(tasks[2].queue, nullptr);

    auto tuple2 = osi_task_stacked_return_tuple(&tasks[2]);
    ASSERT_EQ(tuple2->status, OSS_SUCCESS);
    ASSERT_EQ(tuple2->value.ptr, messages[1]);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_TwoSendBlock) {
    os_task_t tasks[4];
    uint32_t stacks[4][OS_STACK_MINIMUM_SIZE_WORDS];
    const char *messages[4] = {
        "message-0",
        "message-1",
        "message-2",
        "message-3",
    };

    four_tasks_setup(tasks, stacks);

    ASSERT_EQ(osg.running, &tasks[1]);

    os_queue_define(queue, 2, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[0], 500), OSS_SUCCESS);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[1], 500), OSS_SUCCESS);

    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[2], 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)messages[3], 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[3]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->blocked.tasks->nblocked, &tasks[2]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_SEND);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));
    ASSERT_EQ(tasks[2].queue, os_queue(queue));

    void *received1 = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received1, 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[1]);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[1]), OSS_SUCCESS);

    void *received2 = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received2, 500), OSS_SUCCESS);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(osi_task_get_stacked_return(&tasks[2]), OSS_SUCCESS);

    void *received3 = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received3, 500), OSS_SUCCESS);
    ASSERT_EQ(messages[2], received3);

    void *received4 = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received4, 500), OSS_SUCCESS);
    ASSERT_EQ(messages[3], received4);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_SendBlockTimeOut) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];
    auto message = "message-0";

    three_tasks_setup(tasks, stacks);

    os_queue_define(queue, 2, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_SUCCESS);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_SUCCESS);

    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->blocked.tasks->nblocked, nullptr);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_SEND);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));

    /* time passes and task-1 gives up */
    tests_platform_time(499);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    tests_platform_time(500);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, nullptr);
    ASSERT_EQ(tasks[1].queue, nullptr);

    void *received = nullptr;
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received, 500), OSS_SUCCESS);

    ASSERT_EQ(received, message);
}

TEST_F(QueuesSuite, ThreeTasks_Queue_ReceiveBlockTimeOut) {
    os_task_t tasks[3];
    uint32_t stacks[3][OS_STACK_MINIMUM_SIZE_WORDS];
    auto message = "message-0";

    three_tasks_setup(tasks, stacks);

    os_queue_define(queue, 2, OS_QUEUE_FLAGS_NONE);

    ASSERT_EQ(os_queue_create(os_queue(queue), os_queue_def(queue)), OSS_SUCCESS);

    void *received = nullptr;
    osi_task_set_stacked_return(&tasks[1], OSS_ERROR_TO);
    ASSERT_EQ(osi_queue_dequeue(os_queue(queue), &received, 500), OSS_ERROR_TO);
    ASSERT_EQ(tests_task_switch(), &tasks[2]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, &tasks[1]);
    ASSERT_EQ(os_queue(queue)->status, OS_QUEUE_BLOCKED_RECEIVE);
    ASSERT_EQ(tasks[1].queue, os_queue(queue));

    /* time passes and task-1 gives up */
    tests_platform_time(499);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[2]);
    tests_platform_time(500);
    ASSERT_EQ(tests_schedule_task_and_switch(), &tasks[1]);

    ASSERT_EQ(os_queue(queue)->blocked.tasks, nullptr);
    ASSERT_EQ(tasks[1].queue, nullptr);

    ASSERT_EQ(osi_queue_enqueue(os_queue(queue), (void *)message, 500), OSS_SUCCESS);

    // auto tuple = osi_task_stacked_return_tuple(&tasks[1]);
    // ASSERT_EQ(tuple->status, OSS_ERROR_TO);
    // ASSERT_EQ(tuple->value.ptr, nullptr);
}
