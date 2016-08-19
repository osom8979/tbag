/**
 * @file   TaskExecutorTest.cpp
 * @brief  TaskExecutor class tester.
 * @author zer0
 * @date   2016-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/thread/TaskExecutor.hpp>

#include <chrono>
#include <atomic>

using namespace libtbag;
using namespace libtbag::thread;

TEST(TaskExecutorTest, async1)
{
    TaskExecutor executor;

    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;

    ASSERT_TRUE(executor.push([&](){ t1 = TEST_NUMBER; }));
    ASSERT_TRUE(executor.push([&](){ t2 = TEST_NUMBER; }));

    ASSERT_EQ( 0, t1);
    ASSERT_EQ( 0, t2);
    ASSERT_EQ(2U, executor.size());

    executor.runAsync(1U);
    executor.exit();
    executor.join();

    ASSERT_FALSE(executor.push([&](){}));
    ASSERT_FALSE(executor.push([&](){}));

    ASSERT_EQ(TEST_NUMBER, t1);
    ASSERT_EQ(TEST_NUMBER, t2);
}

TEST(TaskExecutorTest, async2)
{
    TaskExecutor executor;

    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;

    ASSERT_TRUE(executor.push([&](){ t1 = TEST_NUMBER; }));
    ASSERT_TRUE(executor.push([&](){ t2 = TEST_NUMBER; }));

    executor.runAsync(2U);
    ASSERT_TRUE(executor.push([&](){ t3 = TEST_NUMBER; }));
    ASSERT_TRUE(executor.push([&](){ t4 = TEST_NUMBER; }));

    executor.exit();
    executor.join();

    ASSERT_EQ(TEST_NUMBER, t1);
    ASSERT_EQ(TEST_NUMBER, t2);
    ASSERT_EQ(TEST_NUMBER, t3);
    ASSERT_EQ(TEST_NUMBER, t4);
}

TEST(TaskExecutorTest, exception)
{
    TaskExecutor executor;
    ASSERT_THROW(executor.runAsync(0U), IllegalArgumentException);
}

TEST(TaskExecutorTest, reset)
{
    TaskExecutor executor;

    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;

    executor.push([&](){ t1 = TEST_NUMBER; });
    executor.push([&](){ t2 = TEST_NUMBER; });
    executor.runAsync(2U);
    ASSERT_EQ(2U, executor.getThreadCount());

    executor.exit();
    executor.join();
    ASSERT_TRUE(executor.empty());
    ASSERT_EQ(TEST_NUMBER, t1);
    ASSERT_EQ(TEST_NUMBER, t2);

    executor.reset();
    ASSERT_EQ(0U, executor.size());
    ASSERT_EQ(0U, executor.getThreadCount());

    executor.runAsync(1U);
    ASSERT_TRUE(executor.push([&](){ t3 = TEST_NUMBER; }));
    ASSERT_TRUE(executor.push([&](){ t4 = TEST_NUMBER; }));
    ASSERT_EQ(1U, executor.getThreadCount());

    executor.exit();
    executor.join();
    ASSERT_TRUE(executor.empty()); // ERROR? [2016/08/07 BUG FIX] Require testing.
    ASSERT_EQ(TEST_NUMBER, t3);
    ASSERT_EQ(TEST_NUMBER, t4);
}

TEST(TaskExecutorTest, joinTask)
{
    TaskExecutor executor(2U);
    int test = 0;

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 1; }));
    ASSERT_EQ(1, test);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 20; }));
    ASSERT_EQ(21, test);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 300; }));
    ASSERT_EQ(321, test);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 4000; std::this_thread::sleep_for(std::chrono::milliseconds(1)); }));
    ASSERT_EQ(4321, test);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 50000; std::this_thread::sleep_for(std::chrono::nanoseconds(1)); }));
    ASSERT_EQ(54321, test);

    executor.exit();
    ASSERT_FALSE(joinTask(executor, [&](){ test += 600000; }));
    ASSERT_EQ(54321, test);
}

