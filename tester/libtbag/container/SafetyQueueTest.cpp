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
    using Queue = SafetyQueue<int>;
    Queue queue;
    int result = 0;

    queue.push(10);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(E_SUCCESS, queue.front(result));
    ASSERT_EQ(10, result);

    queue.push(20);
    queue.push(30);
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(E_SUCCESS, queue.front(result));
    ASSERT_EQ(10, result);

    queue.pop();
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(E_SUCCESS, queue.front(result));
    ASSERT_EQ(20, result);

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
    using Queue = SafetyQueue<int, lock::SpinLock>;
    Queue queue;
    int result = 0;

    queue.push(10);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(E_SUCCESS, queue.frontAndPop(result));
    ASSERT_EQ(10, result);
    ASSERT_EQ(0U, queue.size());
}

