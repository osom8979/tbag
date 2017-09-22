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
#include <atomic>

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

TEST(ThreadPoolTest, DoubleThreadCheck)
{
    std::atomic_bool thread1_running(false);
    std::atomic_bool thread2_running(false);
    std::atomic_bool thread3_running(false);

    std::atomic_bool thread1_exit(false);
    std::atomic_bool thread2_exit(false);
    std::atomic_bool thread3_exit(false);

    ThreadPool pool(2U);

    std::thread::id thread1_id;
    std::thread::id thread2_id;
    std::thread::id thread3_id;

    ASSERT_TRUE(pool.push([&](){
        thread1_id = std::this_thread::get_id();
        std::cout << "J(1)START:";
        thread1_running = true;
        while (thread1_exit == false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        thread1_running = false;
        std::cout << "J(1)END:";
    }));

    ASSERT_TRUE(pool.push([&](){
        thread2_id = std::this_thread::get_id();
        std::cout << "J(2)START:";
        thread2_running = true;
        while (thread2_exit == false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        thread2_running = false;
        std::cout << "J(2)END:";
    }));

    ASSERT_TRUE(pool.push([&](){
        thread3_id = std::this_thread::get_id();
        std::cout << "J(3)START:";
        thread3_running = true;
        while (thread3_exit == false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        thread3_running = false;
        std::cout << "J(3)END:";
    }));

    while (thread1_running == false || thread2_running == false || thread3_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_TRUE(thread1_running);
    ASSERT_TRUE(thread2_running);
    ASSERT_FALSE(thread3_running);

    std::cout << "J(1)EXIT:";
    thread1_exit = true;

    while (thread1_running || thread2_running == false || thread3_running == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_FALSE(thread1_running);
    ASSERT_TRUE(thread2_running);
    ASSERT_TRUE(thread3_running);

    std::cout << "J(2)EXIT:";
    thread2_exit = true;

    while (thread1_running || thread2_running || thread3_running == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_FALSE(thread1_running);
    ASSERT_FALSE(thread2_running);
    ASSERT_TRUE(thread3_running);

    std::cout << "J(3)EXIT:";
    thread3_exit = true;

    while (thread1_running || thread2_running || thread3_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_FALSE(thread1_running);
    ASSERT_FALSE(thread2_running);
    ASSERT_FALSE(thread3_running);
    std::cout << std::endl;

    ASSERT_NE(thread1_id, thread2_id);
    ASSERT_EQ(thread1_id, thread3_id);
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

