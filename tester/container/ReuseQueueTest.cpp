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
    ReuseQueue<int> q3;

    q1.push();
    q2 = std::move(q1);
    ASSERT_EQ(0U, q1.size());
    ASSERT_EQ(1U, q2.size());

    q3 = q2;
    ASSERT_EQ(0U, q1.size());
    ASSERT_EQ(1U, q2.size());
    ASSERT_EQ(1U, q3.size());
}

TEST(ReuseQueueTest, Default)
{
    ReuseQueue<int> queue;

    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());

    int * test1_value = queue.push();
    int * test2_value = queue.push();
    ASSERT_NE(nullptr, test1_value);
    ASSERT_NE(nullptr, test2_value);
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    *test1_value = 100;
    *test2_value = 200;
    ASSERT_EQ(100, *queue.front());

    queue.pop();
    ASSERT_EQ(200, *queue.front());
    ASSERT_EQ(1U,   queue.size());
    ASSERT_EQ(1U,   queue.sizeOfRemoveQueue());

    int * test3_value = queue.push();
    ASSERT_NE(nullptr, test3_value);
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    queue.push();
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());
}

TEST(ReuseQueueTest, ReusePtrQueue)
{
    ReusePtrQueue<int> queue;

    int const TEST_NUMBER = 100;

    auto item = queue.push();
    ASSERT_NE(nullptr, item);
    item->reset(new int(TEST_NUMBER));
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    queue.pop();
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(1U, queue.sizeOfRemoveQueue());

    queue.push();
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    auto find_item = queue.front();
    ASSERT_NE(nullptr, find_item);
    ASSERT_EQ(TEST_NUMBER, *(find_item->get()));
}

