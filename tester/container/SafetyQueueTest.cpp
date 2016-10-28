/**
 * @file   SafetyQueueTest.cpp
 * @brief  SafetyQueue class tester.
 * @author zer0
 * @date   2016-05-22
 */

#include <gtest/gtest.h>
#include <libtbag/container/SafetyQueue.hpp>
#include <libtbag/lock/SpinLock.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::container;

TEST(SafetyQueueTest, Default)
{
    SafetyQueue<int> queue;

    queue.push(10);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(10, queue.front());

    queue.push(20);
    queue.push(30);
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(10, queue.front());

    queue.pop();
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(20, queue.front());

    queue.clear();
    ASSERT_EQ(0U, queue.size());
}

TEST(SafetyQueueTest, PopUntil)
{
    SafetyQueue<int> queue;
    queue.push(0);
    queue.push(0);
    queue.push(0);
    queue.push(0);
    queue.push(0);
    ASSERT_EQ(5U, queue.size());

    queue.popUntil(1U);
    ASSERT_EQ(1U, queue.size());
}

TEST(SafetyQueueTest, SpinLock)
{
    SafetyQueue<int, lock::SpinLock> queue;
    queue.push(10);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(10, queue.frontAndPop());
    ASSERT_EQ(0U, queue.size());
}

