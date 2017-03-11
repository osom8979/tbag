/**
 * @file   UvLockTest.cpp
 * @brief  UvLock class tester.
 * @author zer0
 * @date   2016-10-12
 */

#include <gtest/gtest.h>
#include <libtbag/lock/UvLock.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::lock;

TEST(UvLockTest, Default)
{
    UvLock lock;

    int const TEST_NUMBER = 10;
    int  test   = 0;
    bool result = false;

    lock.lock();
    std::thread t0 = std::thread([&](){
        lock.lock();
        if (test == TEST_NUMBER) {
            result = true;
        }
        lock.unlock();
    });

    std::this_thread::sleep_for(std::chrono::microseconds(1));
    test = TEST_NUMBER;
    lock.unlock();

    t0.join();
    ASSERT_EQ(TEST_NUMBER, test);
    ASSERT_TRUE(result);
}

