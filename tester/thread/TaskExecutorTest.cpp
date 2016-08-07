/**
 * @file   TaskExecutorTest.cpp
 * @brief  TaskExecutor class tester.
 * @author zer0
 * @date   2016-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/thread/TaskExecutor.hpp>

#include <chrono>

using namespace libtbag;
using namespace libtbag::thread;

TEST(TaskExecutorTest, async1)
{
    TaskExecutor executor;

    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;

    executor.push([&](){ t1 = TEST_NUMBER; });
    executor.push([&](){ t2 = TEST_NUMBER; });
    executor.exit();

    ASSERT_EQ(t1, 0);
    ASSERT_EQ(t2, 0);
    ASSERT_EQ(executor.sizeOfQueue(), 2U);

    executor.runAsync(1U);
    executor.join();
    ASSERT_EQ(t1, TEST_NUMBER);
    ASSERT_EQ(t2, TEST_NUMBER);
}

TEST(TaskExecutorTest, async2)
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
    executor.push([&](){ t3 = TEST_NUMBER; });
    executor.push([&](){ t4 = TEST_NUMBER; });

    executor.exit();
    executor.join();

    ASSERT_EQ(t1, TEST_NUMBER);
    ASSERT_EQ(t2, TEST_NUMBER);
    ASSERT_EQ(t3, TEST_NUMBER);
    ASSERT_EQ(t4, TEST_NUMBER);
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
    ASSERT_EQ(executor.getThreadCount(), 2U);

    executor.exit();
    executor.join();
    ASSERT_EQ(executor.sizeOfQueue(), 0U);
    ASSERT_EQ(t1, TEST_NUMBER);
    ASSERT_EQ(t2, TEST_NUMBER);

    executor.reset();
    ASSERT_EQ(executor.sizeOfQueue(), 0U);
    ASSERT_EQ(executor.getThreadCount(), 0U);

    executor.runAsync(1U);
    executor.push([&](){ t3 = TEST_NUMBER; });
    executor.push([&](){ t4 = TEST_NUMBER; });
    ASSERT_EQ(executor.getThreadCount(), 1U);

    executor.exit();
    executor.join();
    ASSERT_EQ(executor.sizeOfQueue(), 0U); // ERROR? [2016/08/07 BUG FIX] Require testing.
    ASSERT_EQ(t3, TEST_NUMBER);
    ASSERT_EQ(t4, TEST_NUMBER);
}

TEST(TaskExecutorTest, joinTask)
{
    TaskExecutor executor(2U);
    int test = 0;

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 1; }));
    ASSERT_EQ(test, 1);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 20; }));
    ASSERT_EQ(test, 21);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 300; }));
    ASSERT_EQ(test, 321);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 4000; std::this_thread::sleep_for(std::chrono::milliseconds(1)); }));
    ASSERT_EQ(test, 4321);

    ASSERT_TRUE(executor.push([](){}));
    ASSERT_TRUE(joinTask(executor, [&](){ test += 50000; std::this_thread::sleep_for(std::chrono::nanoseconds(1)); }));
    ASSERT_EQ(test, 54321);

    executor.exit();
    ASSERT_FALSE(joinTask(executor, [&](){ test += 600000; }));
    ASSERT_EQ(test, 54321);
}

