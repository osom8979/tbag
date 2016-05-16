/**
 * @file   TaskExecutorTest.cpp
 * @brief  TaskExecutor class tester.
 * @author zer0
 * @date   2016-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/thread/TaskExecutor.hpp>

using namespace libtbag;
using namespace libtbag::thread;

TEST(TaskExecutorTest, async1)
{
    TaskExecutor executor;

    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;

    executor.push([&]() -> bool { t1 = TEST_NUMBER; return true; });
    executor.push([&]() -> bool { t2 = TEST_NUMBER; return false; });

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

    executor.push([&]() -> bool { t1 = TEST_NUMBER; return true; });
    executor.push([&]() -> bool { t2 = TEST_NUMBER; return true; });

    executor.runAsync(2U);
    executor.push([&]() -> bool { t3 = TEST_NUMBER; return true; });
    executor.push([&]() -> bool { t4 = TEST_NUMBER; return true; });

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

    executor.push([&]() -> bool { t1 = TEST_NUMBER; return true; });
    executor.push([&]() -> bool { t2 = TEST_NUMBER; return true; });
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
    executor.push([&]() -> bool { t3 = TEST_NUMBER; return true; });
    executor.push([&]() -> bool { t4 = TEST_NUMBER; return true; });
    ASSERT_EQ(executor.getThreadCount(), 1U);

    executor.exit();
    executor.join();
    ASSERT_EQ(executor.sizeOfQueue(), 0U);
    ASSERT_EQ(t3, TEST_NUMBER);
    ASSERT_EQ(t4, TEST_NUMBER);
}

