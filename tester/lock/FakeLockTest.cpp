/**
 * @file   FakeLockTest.cpp
 * @brief  FakeLock class tester.
 * @author zer0
 * @date   2016-05-22
 */

#include <gtest/gtest.h>
#include <libtbag/lock/FakeLock.hpp>

#include <mutex>

using namespace libtbag;
using namespace libtbag::lock;

TEST(FakeLockTest, Default)
{
    FakeLock lock;
    lock.lock();
    lock.unlock();
    ASSERT_FALSE(lock.try_lock());
}

TEST(FakeLockTest, LockGuard)
{
    FakeLock lock;
    std::lock_guard<FakeLock> guard(lock);
}

