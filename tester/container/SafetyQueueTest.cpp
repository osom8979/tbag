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
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.front(), 10);

    queue.push(20);
    queue.push(30);
    ASSERT_EQ(queue.size(), 3U);
    ASSERT_EQ(queue.front(), 10);

    queue.pop();
    ASSERT_EQ(queue.size(), 2U);
    ASSERT_EQ(queue.front(), 20);

    queue.clear();
    ASSERT_EQ(queue.size(), 0U);
}

TEST(SafetyQueueTest, popUntil)
{
    SafetyQueue<int> queue;
    queue.push(0);
    queue.push(0);
    queue.push(0);
    queue.push(0);
    queue.push(0);
    ASSERT_EQ(queue.size(), 5U);
    queue.popUntil(1U);
    ASSERT_EQ(queue.size(), 1U);
}

TEST(SafetyQueueTest, SpinLock)
{
    SafetyQueue<int, lock::SpinLock> queue;
    queue.push(10);
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.frontAndPop(), 10);
    ASSERT_EQ(queue.size(), 0U);
}

