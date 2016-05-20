/**
 * @file   SpinLockTest.cpp
 * @brief  SpinLock class tester.
 * @author zer0
 * @date   2016-05-20
 */

#include <gtest/gtest.h>

#define TEST_LIBTBAG_SPINLOCK
#include <libtbag/lock/SpinLock.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::lock;

class SpinLockTestDemo : public SpinLock
{
};

TEST(SpinLockTest, Default)
{
    bool const LOCKED   = SpinLock::LOCKED;
    bool const UNLOCKED = SpinLock::UNLOCKED;

    SpinLockTestDemo locker;
    std::atomic_int result(0);

    ASSERT_EQ(result.load(), 0);
    ASSERT_EQ(locker._state.load(), UNLOCKED);

    locker.lock();
    ASSERT_EQ(locker._state.load(), LOCKED);

    bool t1_result1 = false;
    bool t1_result2 = false;
    bool t1_result3 = false;

    std::thread t1([&](){
        t1_result1 = (locker._state.load() == LOCKED ? true : false);
        result.store(100);

        locker.lock();

        t1_result2 = (locker._state.load() == LOCKED ? true : false);
        result.store(200);

        locker.unlock();
        t1_result3 = (locker._state.load() == UNLOCKED ? true : false);
    });

    {
        bool t2_result = false;
        std::thread t2([&](){
            t2_result = locker.try_lock();
        });
        ASSERT_FALSE(locker.try_lock());

        t2.join();
        ASSERT_FALSE(t2_result);
    }

    ASSERT_EQ(result.load(), 100);
    ASSERT_EQ(locker._state.load(), LOCKED);

    locker.unlock();
    t1.join();

    ASSERT_EQ(locker._state.load(), UNLOCKED);
    ASSERT_EQ(result.load(), 200);
    ASSERT_TRUE(t1_result1);
    ASSERT_TRUE(t1_result2);
    ASSERT_TRUE(t1_result3);
}

