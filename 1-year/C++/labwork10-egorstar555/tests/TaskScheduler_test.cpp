#include <lib/TaskScheduler.h>

#include <gtest/gtest.h>
#include <valarray>
#include <random>

TEST(TTaskSchedulerTest, AddAndGetResultInt) {
    TTaskScheduler scheduler;
    int result = scheduler.add([]() -> int { return 42; });
    scheduler.ExecuteAll();

    ASSERT_EQ(scheduler.GetResult<int>(result), 42);
}

TEST(TTaskSchedulerTest, GetFutureResultInt) {
    TTaskScheduler scheduler;
    int result = scheduler.add([]() { return 100; });
    scheduler.ExecuteAll();

    ASSERT_EQ(scheduler.GetFutureResult<int>(result), 100);
}

TEST(TTaskSchedulerTest, AddAndGetResultFloat) {
    TTaskScheduler scheduler;
    float result = scheduler.add([]() { return 42.0; });
    scheduler.ExecuteAll();

    ASSERT_EQ(scheduler.GetResult<double>(result), 42.0);
}

TEST(TTaskSchedulerTest, GetFutureResult2Float) {
    TTaskScheduler scheduler;
    float result = scheduler.add([]() { return 100.0; });
    scheduler.ExecuteAll();

    ASSERT_EQ(scheduler.GetFutureResult<double>(result),
              100);
}

TEST(TTaskSchedulerTest, RootsQuadraticEquation) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto id0 = scheduler.add([](float a, float c) -> float { return -4 * a * c; }, a, c);
    auto id1 = scheduler.add([](float b, float* v) -> float { return b * b + *v; }, b,
                             &(scheduler.GetFutureResult<float>(id0)));
    auto id2 = scheduler.add([](float b, float* d) -> float { return -b + std::sqrt(*d); }, b,
                             &(scheduler.GetFutureResult<float>(id1)));
    auto id3 = scheduler.add([](float b, float* d) -> float { return -b - std::sqrt(*d); }, b,
                             &(scheduler.GetFutureResult<float>(id1)));
    auto id4 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a,
                             &(scheduler.GetFutureResult<float>(id2)));
    auto id5 = scheduler.add([](float a, float* v) -> float { return *v / (2 * a); }, a,
                             &(scheduler.GetFutureResult<float>(id3)));


    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<float>(id0), 0);
    EXPECT_EQ(scheduler.GetResult<float>(id1), 4);
    EXPECT_EQ(scheduler.GetResult<float>(id2), 4);
    EXPECT_EQ(scheduler.GetResult<float>(id3), 0);
    EXPECT_EQ(scheduler.GetResult<float>(id4), 2);
    EXPECT_EQ(scheduler.GetResult<float>(id5), 0);
}

TEST(TTaskSchedulerTest, DifferentReturnTypes) {

    int a = 1;
    int b = -2;
    int c = 0;

    TTaskScheduler scheduler;

    auto id0 = scheduler.add([](int a, int c) -> int { return -4 * a * c; }, a, c);
    auto id1 = scheduler.add([](int b, int* v) -> int { return b * b + *v; }, b,
                             &(scheduler.GetFutureResult<int>(id0)));
    auto id2 = scheduler.add([](int b, int* d) -> float { return -b + std::sqrt(*d + 10); }, b,
                             &(scheduler.GetFutureResult<int>(id1)));


    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<int>(id0), 0);
    EXPECT_EQ(scheduler.GetResult<int>(id1), 4);
    EXPECT_NEAR(scheduler.GetResult<float>(id2), 5.74166, 0.0001);
}

TEST(TTaskSchedulerTest, DifferentSignature) {

    int a = 1;
    int b = -2;
    int c = 0;

    TTaskScheduler scheduler;

    auto id0 = scheduler.add([]() -> int { return 100; });
    auto id1 = scheduler.add([](int* v) -> int { return 50 + *v; }, &(scheduler.GetFutureResult<int>(id0)));
    auto id2 = scheduler.add([](int b, int* d) -> int { return b * (*d); }, b, &(scheduler.GetFutureResult<int>(id1)));


    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<int>(id0), 100);
    EXPECT_EQ(scheduler.GetResult<int>(id1), 150);
    EXPECT_EQ(scheduler.GetResult<int>(id2), -300);
}

TEST(TTaskSchedulerTest, TaskOrder) {

    int a = 1;
    int b = -2;
    int c = 0;

    TTaskScheduler scheduler;

    auto id0 = scheduler.add([]() -> int { return 100; });
    auto id1 = scheduler.add([](int* v) -> int { return 50 + *v; }, &(scheduler.GetFutureResult<int>(id0)));
    auto id2 = scheduler.add([](int b, int* d) -> int { return b * (*d); }, b, &(scheduler.GetFutureResult<int>(id1)));

    EXPECT_EQ(scheduler.GetResult<int>(id2), -300);
}


TEST(TTaskSchedulerTest, AddTaskAndGetResult) {
    TTaskScheduler scheduler;

    int result = 0;
    auto task_id = scheduler.add([&result](int a, int b) { return a + b; }, 5, 10);

    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<int>(task_id), 15);
}

TEST(TTaskSchedulerTest, AddTaskAndGetFutureResult) {
    TTaskScheduler scheduler;

    double result = 0.0;
    auto task_id = scheduler.add([&result](double a, double b) { return a * b; }, 2.5, 3.5);

    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetFutureResult<double>(task_id), 8.75);
}

TEST(TTaskSchedulerTest, ExecuteAllTasks) {
    TTaskScheduler scheduler;

    int result1 = 0;
    int result2 = 0;
    auto task_id1 = scheduler.add([&result1](int a, int b) { return a + b; }, 5, 10);
    auto task_id2 = scheduler.add([&result2](int a, int b) { return a * b; }, 3, 7);

    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<int>(task_id1), 15);
    EXPECT_EQ(scheduler.GetResult<int>(task_id2), 21);
}