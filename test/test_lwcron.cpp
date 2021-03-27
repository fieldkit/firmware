#include <gtest/gtest.h>
#include <vector>

#include <lwcron/lwcron.h>

using namespace lwcron;

static DateTime JacobsBirth{ 1982, 4, 23, 7, 30, 00 };

class SchedulerSuite : public ::testing::Test {
protected:

};

TEST_F(SchedulerSuite, DateTime) {
    ASSERT_EQ(JacobsBirth.unix_time(), 388395000);
    ASSERT_EQ(DateTime{ 388395000 }, JacobsBirth);
}

TEST_F(SchedulerSuite, Empty) {
    Scheduler scheduler;

    scheduler.begin(JacobsBirth);

    ASSERT_FALSE(scheduler.nextTask(JacobsBirth));
}

TEST_F(SchedulerSuite, Interval10s) {
    PeriodicTask task{ 10 };
    Task *tasks[1] = { &task };
    Scheduler scheduler{ tasks };

    scheduler.begin(JacobsBirth);

    auto n1 = scheduler.nextTask(JacobsBirth);
    ASSERT_EQ(n1.time, JacobsBirth.unix_time());

    auto n2 = scheduler.nextTask(JacobsBirth + 1);
    ASSERT_EQ(n2.time, JacobsBirth.unix_time() + 10);

    auto n3 = scheduler.nextTask(JacobsBirth + 5);
    ASSERT_EQ(n3.time, JacobsBirth.unix_time() + 10);

    auto n4 = scheduler.nextTask(JacobsBirth + 15);
    ASSERT_EQ(n4.time, JacobsBirth.unix_time() + 20);
}

TEST_F(SchedulerSuite, Interval60s) {
    PeriodicTask task{ 60 };
    Task *tasks[1] = { &task };
    Scheduler scheduler{ tasks };

    scheduler.begin(JacobsBirth);

    auto n1 = scheduler.nextTask(JacobsBirth + 1);
    ASSERT_EQ(n1.time, JacobsBirth.unix_time() + 60);

    auto n2 = scheduler.nextTask(JacobsBirth + 30);
    ASSERT_EQ(n2.time, JacobsBirth.unix_time() + 60);

    auto n3 = scheduler.nextTask(JacobsBirth + 90);
    ASSERT_EQ(n3.time, JacobsBirth.unix_time() + 120);
}

TEST_F(SchedulerSuite, Interval10m) {
    PeriodicTask task{ 60 * 10 };
    Task *tasks[1] = { &task };
    Scheduler scheduler{ tasks };

    scheduler.begin(JacobsBirth);

    auto n1 = scheduler.nextTask(JacobsBirth);
    ASSERT_EQ(n1.time, JacobsBirth.unix_time());

    auto n2 = scheduler.nextTask(JacobsBirth + 30);
    ASSERT_EQ(n2.time, JacobsBirth.unix_time() + (60 * 10));
}

TEST_F(SchedulerSuite, MultipleIntervals) {
    PeriodicTask task1{ 60 * 2 };
    PeriodicTask task2{ 60 * 5 };
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    scheduler.begin(JacobsBirth);

    auto n1 = scheduler.nextTask(JacobsBirth);
    ASSERT_EQ(n1.time, JacobsBirth.unix_time());
    ASSERT_EQ(n1.task, &task1);

    auto n2 = scheduler.nextTask(JacobsBirth + 5);
    ASSERT_EQ(n2.time, JacobsBirth.unix_time() + 60 * 2);
    ASSERT_EQ(n2.task, &task1);

    auto n3 = scheduler.nextTask(JacobsBirth + 60 * 3);
    ASSERT_EQ(n3.time, JacobsBirth.unix_time() + 60 * 4);
    ASSERT_EQ(n3.task, &task1);

    auto n4 = scheduler.nextTask(JacobsBirth + 60 * 4);
    ASSERT_EQ(n4.time, JacobsBirth.unix_time() + 60 * 4);
    ASSERT_EQ(n4.task, &task1);

    auto n5 = scheduler.nextTask(JacobsBirth + 60 * 4 + 5);
    ASSERT_EQ(n5.time, JacobsBirth.unix_time() + 60 * 5);
    ASSERT_EQ(n5.task, &task2);
}

class ExampleTask : public PeriodicTask {
private:
    bool ran_{ false };

public:
    ExampleTask(uint32_t interval) : PeriodicTask(interval) {
    }

public:
    bool ran() {
        return ran_;
    };

public:
    void run() override {
        ran_ = true;
    }
};

TEST_F(SchedulerSuite, CheckRunsTasks) {
    ExampleTask task1{ 60 * 2 };
    Task *tasks[1] = { &task1 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    auto n1 = scheduler.nextTask(JacobsBirth);
    ASSERT_EQ(n1.time, JacobsBirth.unix_time());
    ASSERT_EQ(n1.task, &task1);

    ASSERT_FALSE(scheduler.check(now + 5));

    ASSERT_FALSE(task1.ran());
    auto to2 = scheduler.check(now + 60 * 2);
    ASSERT_EQ(to2.task, &task1);
    ASSERT_TRUE(task1.ran());
}

TEST_F(SchedulerSuite, RunningTasksMultipleIntervals) {
    PeriodicTask task1{ 60 * 2 };
    PeriodicTask task2{ 60 * 5 };
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task1);
    ASSERT_EQ(n1.time, now.unix_time() + 60 * 2);

    auto to2 = scheduler.check(now + 60 * 2);
    ASSERT_EQ(to2.task, &task1);
    ASSERT_FALSE(scheduler.check(now + 60 * 2));
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.task, &task1);
    ASSERT_EQ(n2.time, now.unix_time() + 60 * 4);

    auto to3 = scheduler.check(now + 60 * 4);
    ASSERT_EQ(to3.task, &task1);
    ASSERT_FALSE(scheduler.check(now + 60 * 4));
    auto n3 = scheduler.nextTask();
    ASSERT_EQ(n3.task, &task2);
}

TEST_F(SchedulerSuite, RunningTasksMultipleIntervalsDoesntMissTask1) {
    PeriodicTask task1{ 60 * 2 };
    PeriodicTask task2{ 60 * 2 };
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task1);
    ASSERT_EQ(n1.time, now.unix_time() + 60 * 2);

    auto o1 = scheduler.check(now + 60 * 2);
    ASSERT_EQ(o1.task, &task1);
    auto o2 = scheduler.check(now + 60 * 2);
    ASSERT_EQ(o2.task, &task2);
    ASSERT_FALSE(scheduler.check(now + 60 * 2));
}

TEST_F(SchedulerSuite, RunningTasksMultipleIntervalsDoesntMissTask2) {
    PeriodicTask task1{ 60 * 2 };
    PeriodicTask task2{ 60 * 2 };
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task1);
    ASSERT_EQ(n1.time, now.unix_time() + 60 * 2);

    auto o1 = scheduler.check(now + 60 * 2);
    ASSERT_EQ(o1.task, &task1);

    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.task, &task2);
    ASSERT_EQ(n2.time, now.unix_time() + 60 * 2);

    auto o2 = scheduler.check(now + 60 * 3);
    ASSERT_EQ(o2.task, &task2);
    ASSERT_FALSE(scheduler.check(now + 60 * 3));
}

TEST_F(SchedulerSuite, CronSpec) {
    DateTime now = JacobsBirth;
    CronSpec spec{ now };

    ASSERT_TRUE(bitarray_test(spec.seconds, 0));
    ASSERT_TRUE(bitarray_test(spec.minutes, 30));
    ASSERT_TRUE(bitarray_test(spec.hours, 7));

    CronSpec halfHourMark = CronSpec::specific(0, 30);
    ASSERT_TRUE(bitarray_test(halfHourMark.seconds, 0));
    ASSERT_TRUE(bitarray_test(halfHourMark.minutes, 30));

    ASSERT_EQ(halfHourMark.getNextTime(now + 0), now.unix_time());
    ASSERT_EQ(halfHourMark.getNextTime(now + 1), now.unix_time() + 60 * 60);

    CronSpec sixFifteenAm = CronSpec::specific(0, 15, 6);
    ASSERT_TRUE(bitarray_test(sixFifteenAm.seconds, 0));
    ASSERT_TRUE(bitarray_test(sixFifteenAm.minutes, 15));
    ASSERT_TRUE(bitarray_test(sixFifteenAm.hours, 6));

    DateTime time1(1982, 4, 24, 6, 15, 0);
    ASSERT_EQ(sixFifteenAm.getNextTime(now + 0), time1.unix_time());
}

TEST_F(SchedulerSuite, RunningTaskOnceADay) {
    CronTask task1{ CronSpec::interval(86400) };
    Task *tasks[1] = { &task1 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task1);
    DateTime time1(1982, 4, 24, 0, 0, 0);
    ASSERT_EQ(n1.time, time1.unix_time());

    ASSERT_FALSE(scheduler.check(now + 10));
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.task, &task1);
    DateTime time2(1982, 4, 24, 0, 0, 0);
    ASSERT_EQ(n2.time, time2.unix_time());

    ASSERT_TRUE(scheduler.check(now + (60 * 60 * 17)));
    auto n3 = scheduler.nextTask();
    ASSERT_EQ(n3.task, &task1);
    DateTime time3(1982, 4, 25, 0, 0, 0);
    ASSERT_EQ(n3.time, time3.unix_time());
}

TEST_F(SchedulerSuite, RunningTasksMultipleCrons) {
    CronTask task1{ CronSpec::specific( 0, 20,  6) }; //  6:20AM
    CronTask task2{ CronSpec::specific(30,  0, 12) }; // 12:00PM
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task2);
    DateTime time1(1982, 4, 23, 12, 0, 30);
    ASSERT_EQ(n1.time, time1.unix_time());

    auto o1 = scheduler.check(time1);
    ASSERT_EQ(o1.task, &task2);
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.task, &task1);
    DateTime time2(1982, 4, 24, 6, 20, 0);
    ASSERT_EQ(n2.time, time2.unix_time());
}

TEST_F(SchedulerSuite, RunningTasksMultipleCronsRecurringRuns) {
    CronTask task1{ CronSpec::specific( 0, 20,  6) }; //  6:20AM
    CronTask task2{ CronSpec::specific(30,  0, 12) }; // 12:00PM
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth;
    scheduler.begin(now + 5);

    ASSERT_FALSE(scheduler.check(now + 5));
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.task, &task2);
    DateTime time1(1982, 4, 23, 12, 0, 30);
    ASSERT_EQ(n1.time, time1.unix_time());

    auto o1 = scheduler.check(time1);
    ASSERT_EQ(o1.task, &task2);
    ASSERT_FALSE(scheduler.check(time1));
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.task, &task1);
    DateTime time2(1982, 4, 24, 6, 20, 0);
    ASSERT_EQ(n2.time, time2.unix_time());
}

TEST_F(SchedulerSuite, CronSpecIntervals) {
    auto everyMinute = CronSpec::interval(60);
    ASSERT_EQ(bitarray_nset(everyMinute.seconds), 1);
    ASSERT_EQ(bitarray_nset(everyMinute.minutes), 60);
    ASSERT_EQ(bitarray_nset(everyMinute.hours), 24);

    auto everyTwoMinutes = CronSpec::interval(60 * 2);
    ASSERT_EQ(bitarray_nset(everyTwoMinutes.seconds), 1);
    ASSERT_EQ(bitarray_nset(everyTwoMinutes.minutes), 30);
    ASSERT_EQ(bitarray_nset(everyTwoMinutes.hours), 24);

    auto everyHour = CronSpec::interval(60 * 60);
    ASSERT_EQ(bitarray_nset(everyHour.seconds), 1);
    ASSERT_EQ(bitarray_nset(everyHour.minutes), 1);
    ASSERT_EQ(bitarray_nset(everyHour.hours), 24);

    auto everyTwoHours = CronSpec::interval(60 * 60 * 2);
    ASSERT_EQ(bitarray_nset(everyTwoHours.seconds), 1);
    ASSERT_EQ(bitarray_nset(everyTwoHours.minutes), 1);
    ASSERT_EQ(bitarray_nset(everyTwoHours.hours), 12);

    auto everySixHours = CronSpec::interval(60 * 60 * 6);
    ASSERT_EQ(bitarray_nset(everySixHours.seconds), 1);
    ASSERT_EQ(bitarray_nset(everySixHours.minutes), 1);
    ASSERT_EQ(bitarray_nset(everySixHours.hours), 4);
}

TEST_F(SchedulerSuite, InvestigateMissedTaskProblem) {
    PeriodicTask task1{ 150 };
    PeriodicTask task2{ 300 };
    Task *tasks[2] = { &task1, &task2 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth + 3;
    scheduler.begin(now);

    ASSERT_FALSE(scheduler.check(now));

    ASSERT_FALSE(scheduler.check(now + 145));

    ASSERT_TRUE(scheduler.check(now + 147));
}

TEST_F(SchedulerSuite, ClockMovingBackwardsResets) {
    PeriodicTask task1{ 150 };
    Task *tasks[1] = { &task1 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth - 1;
    scheduler.begin(now);

    ASSERT_FALSE(scheduler.check(now));

    now += 1;
    ASSERT_TRUE(scheduler.check(now));

    // Next time is 150s after "now"
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.time, now.unix_time() + 150);

    // Go back two hours.
    now -= 60 * 60 * 2 + 1;

    ASSERT_FALSE(scheduler.check(now));

    // New next time is 1s after the new now.
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.time, now.unix_time() + 1);

    ASSERT_TRUE(scheduler.check(now + 1));
}

TEST_F(SchedulerSuite, ClockMovingBackwardsAFewSecondsDoesNotReRun) {
    PeriodicTask task1{ 150 };
    Task *tasks[1] = { &task1 };
    Scheduler scheduler{ tasks };

    auto now = JacobsBirth - 1;
    scheduler.begin(now);

    ASSERT_FALSE(scheduler.check(now));

    now += 1;
    ASSERT_TRUE(scheduler.check(now));

    // Next time is 150s after "now"
    auto n1 = scheduler.nextTask();
    ASSERT_EQ(n1.time, now.unix_time() + 150);

    // Go back 10 seconds
    now -= 10;

    ASSERT_FALSE(scheduler.check(now));

    // New next time should be the same as previously.
    auto n2 = scheduler.nextTask();
    ASSERT_EQ(n2.time, n1.time);
}
