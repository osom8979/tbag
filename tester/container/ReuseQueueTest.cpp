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
    ASSERT_EQ(q1.size(), 0U);
    ASSERT_EQ(q2.size(), 1U);

    q3 = q2;
    ASSERT_EQ(q1.size(), 0U);
    ASSERT_EQ(q2.size(), 1U);
    ASSERT_EQ(q3.size(), 1U);
}

TEST(ReuseQueueTest, Default)
{
    ReuseQueue<int> queue;

    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());

    int * test1_value = queue.push();
    int * test2_value = queue.push();
    ASSERT_TRUE(test1_value != nullptr);
    ASSERT_TRUE(test2_value != nullptr);
    ASSERT_EQ(queue.size(), 2U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    *test1_value = 100;
    *test2_value = 200;
    ASSERT_EQ(*queue.front(), 100);

    queue.pop();
    ASSERT_EQ(*queue.front(), 200);
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 1U);

    int * test3_value = queue.push();
    ASSERT_TRUE(test3_value != nullptr);
    ASSERT_EQ(queue.size(), 2U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    queue.push();
    ASSERT_EQ(queue.size(), 3U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfRemoveQueue());
}

TEST(ReuseQueueTest, ReusePtrQueue)
{
    ReusePtrQueue<int> queue;

    int const TEST_NUMBER = 100;

    auto item = queue.push();
    ASSERT_TRUE(item != nullptr);
    item->reset(new int(TEST_NUMBER));
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    queue.pop();
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 1U);

    queue.push();
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    auto find_item = queue.front();
    ASSERT_TRUE(find_item != nullptr);
    ASSERT_EQ(*(find_item->get()), TEST_NUMBER);
}

