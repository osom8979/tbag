/**
 * @file   ThreadPoolTest.cpp
 * @brief  ThreadPool class tester.
 * @author zer0
 * @date   2016-10-13
 */

#include <gtest/gtest.h>
#include <libtbag/thread/ThreadPool.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::thread;

TEST(ThreadPoolTest, Default)
{
    int const TEST_NUMBER = 100;
    int t1 = 0;
    int t2 = 0;

    {
        ThreadPool pool(2U);

        ASSERT_TRUE(pool.push([&](){ t1 = TEST_NUMBER; }));
        ASSERT_TRUE(pool.push([&](){ t2 = TEST_NUMBER; }));

        pool.exit();

        ASSERT_FALSE(pool.push([&](){}));
        ASSERT_FALSE(pool.push([&](){}));
    }

    ASSERT_EQ(TEST_NUMBER, t1);
    ASSERT_EQ(TEST_NUMBER, t2);
}

TEST(ThreadPoolTest, Exception)
{
    ASSERT_THROW(ThreadPool(0U), IllegalArgumentException);
}

TEST(ThreadPoolTest, WaitTask)
{
    ThreadPool pool(2U);
    int test = 0;

    ASSERT_TRUE(pool.push([](){}));
    ASSERT_TRUE(ThreadPool::waitTask(pool, [&](){ test += 1; }));
    ASSERT_EQ(1, test);

    ASSERT_TRUE(pool.push([](){}));
    ASSERT_TRUE(ThreadPool::waitTask(pool, [&](){ test += 20; }));
    ASSERT_EQ(21, test);

    ASSERT_TRUE(pool.push([](){}));
    ASSERT_TRUE(ThreadPool::waitTask(pool, [&](){ test += 300; }));
    ASSERT_EQ(321, test);

    ASSERT_TRUE(pool.push([](){}));
    ASSERT_TRUE(ThreadPool::waitTask(pool, [&](){ test += 4000; std::this_thread::sleep_for(std::chrono::milliseconds(1)); }));
    ASSERT_EQ(4321, test);

    ASSERT_TRUE(pool.push([](){}));
    ASSERT_TRUE(ThreadPool::waitTask(pool, [&](){ test += 50000; std::this_thread::sleep_for(std::chrono::nanoseconds(1)); }));
    ASSERT_EQ(54321, test);

    pool.exit();
    ASSERT_FALSE(ThreadPool::waitTask(pool, [&](){ test += 600000; }));
    ASSERT_EQ(54321, test);
}

