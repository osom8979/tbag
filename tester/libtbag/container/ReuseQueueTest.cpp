/**
 * @file   ReuseQueueTest.cpp
 * @brief  ReuseQueue class tester.
 * @author zer0
 * @date   2016-08-03
 */

#include <gtest/gtest.h>
#include <libtbag/container/ReuseQueue.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(ReuseQueueTest, Constructor)
{
    ReuseQueue<int> q1;
    ReuseQueue<int> q2;

    q1.push();
    ASSERT_EQ(1U, q1.size());

    q2 = q1;
    ASSERT_EQ(1U, q1.size());
    ASSERT_EQ(1U, q2.size());
}

TEST(ReuseQueueTest, MoveConstructor)
{
    ReuseQueue<int> q1;
    ReuseQueue<int> q2;

    q1.push();
    q2 = std::move(q1);

    ASSERT_EQ(0U, q1.size());
    ASSERT_EQ(1U, q2.size());
}

TEST(ReuseQueueTest, Default)
{
    ReuseQueue<int> queue;

    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());

    int & test1_value = queue.push();
    int & test2_value = queue.push();
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    test1_value = 100;
    test2_value = 200;

    int result;
    ASSERT_EQ(Err::E_SUCCESS, queue.front(result));
    ASSERT_EQ(100, result);

    queue.pop();
    ASSERT_EQ(Err::E_SUCCESS, queue.front(result));
    ASSERT_EQ(200, result);
    ASSERT_EQ(1U,  queue.size());
    ASSERT_EQ(1U,  queue.sizeOfRemoveQueue());

    int & test3_value = queue.push();
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    queue.push();
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());
}

