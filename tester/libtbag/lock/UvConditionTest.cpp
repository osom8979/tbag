/**
 * @file   UvConditionTest.cpp
 * @brief  UvCondition class tester.
 * @author zer0
 * @date   2016-10-12
 */

#include <gtest/gtest.h>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/lock/UvLock.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::lock;

TEST(UvConditionTest, Default)
{
    UvCondition condition;
    UvLock lock;

    int const TEST_NUMBER = 10;
    int test = 0;

    std::thread t0 = std::thread([&](){
        std::this_thread::sleep_for(std::chrono::microseconds(1));

        lock.lock();
        test = TEST_NUMBER;
        condition.signal();
        lock.unlock();
    });

    lock.lock();
    while (test != TEST_NUMBER) {
        condition.wait(lock);
    }
    lock.unlock();

    t0.join();
    ASSERT_EQ(TEST_NUMBER, test);
}

