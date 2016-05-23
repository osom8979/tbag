/**
 * @file   SpinLockTest.cpp
 * @brief  SpinLock class tester.
 * @author zer0
 * @date   2016-05-20
 */

#include <gtest/gtest.h>
#include <libtbag/lock/SpinLock.hpp>

#include <mutex>
#include <thread>

using namespace libtbag;
using namespace libtbag::lock;

TEST(SpinLockTest, Default)
{
    SpinLock locker;

    int r1 = 0;
    int r2 = 0;

    std::thread t1([&](){
        std::lock_guard<SpinLock> guard(locker);
        if (r2 == 0) {
            r1 = 100;
        } else {
            r1 = r2;
        }
    });

    std::thread t2([&](){
        std::lock_guard<SpinLock> guard(locker);
        if (r1 == 0) {
            r2 = 200;
        } else {
            r2 = r1;
        }
    });

    t1.join();
    t2.join();

    ASSERT_EQ(r1, r2);
}

