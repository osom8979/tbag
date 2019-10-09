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

    q1.push(1);
    ASSERT_EQ(1U, q1.size());

    q2 = q1;
    ASSERT_EQ(1U, q1.size());
    ASSERT_EQ(1U, q2.size());
}

TEST(ReuseQueueTest, MoveConstructor)
{
    ReuseQueue<int> q1;
    ReuseQueue<int> q2;

    ASSERT_EQ(0U, q1.size());
    ASSERT_EQ(0U, q2.size());

    q1.push(1);
    q2 = std::move(q1);
    ASSERT_EQ(1U, q2.size());
}

TEST(ReuseQueueTest, Default)
{
    ReuseQueue<int> queue;

    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReady());

    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfReady());

    queue.push(100);
    queue.push(200);
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReady());

    int result;
    ASSERT_EQ(100, queue.front());

    queue.pop();
    ASSERT_EQ(200, queue.front());
    ASSERT_EQ(1U,  queue.size());
    ASSERT_EQ(1U,  queue.sizeOfReady());

    queue.push(300);
    ASSERT_EQ(2U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReady());

    queue.push(400);
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReady());

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfReady());
}

TEST(ReuseQueueTest, PushLambda)
{
    int const TEST_VALUE1 = 100;
    int const TEST_VALUE2 = 200;
    ReuseQueue<int> q1;

    ASSERT_EQ(0, q1.size());
    ASSERT_EQ(0, q1.sizeOfReady());
    ASSERT_EQ(0, q1.sizeOfTotal());
    ASSERT_TRUE(q1.empty());
    ASSERT_TRUE(q1.emptyOfReady());
    ASSERT_TRUE(q1.emptyOfTotal());

    q1.push([&](int & v){ v = TEST_VALUE1; });
    ASSERT_EQ(TEST_VALUE1, q1.front());
    ASSERT_EQ(TEST_VALUE1, q1.back());

    q1.push([&](int & v){ v = TEST_VALUE2; });
    ASSERT_EQ(TEST_VALUE1, q1.front());
    ASSERT_EQ(TEST_VALUE2, q1.back());

    ASSERT_EQ(2, q1.size());
    ASSERT_EQ(0, q1.sizeOfReady());
    ASSERT_EQ(2, q1.sizeOfTotal());
    ASSERT_FALSE(q1.empty());
    ASSERT_TRUE(q1.emptyOfReady());
    ASSERT_FALSE(q1.emptyOfTotal());

    q1.pop();
    ASSERT_EQ(TEST_VALUE2, q1.front());
    ASSERT_EQ(TEST_VALUE2, q1.back());

    ASSERT_EQ(1, q1.size());
    ASSERT_EQ(1, q1.sizeOfReady());
    ASSERT_EQ(2, q1.sizeOfTotal());
    ASSERT_FALSE(q1.empty());
    ASSERT_FALSE(q1.emptyOfReady());
    ASSERT_FALSE(q1.emptyOfTotal());

    int result;
    ASSERT_TRUE(q1.frontAndPop(&result));
    ASSERT_EQ(TEST_VALUE2, result);

    ASSERT_EQ(0, q1.size());
    ASSERT_EQ(2, q1.sizeOfReady());
    ASSERT_EQ(2, q1.sizeOfTotal());
    ASSERT_TRUE(q1.empty());
    ASSERT_FALSE(q1.emptyOfReady());
    ASSERT_FALSE(q1.emptyOfTotal());

    q1.clear();
    ASSERT_EQ(0, q1.size());
    ASSERT_EQ(0, q1.sizeOfReady());
    ASSERT_EQ(0, q1.sizeOfTotal());
    ASSERT_TRUE(q1.empty());
    ASSERT_TRUE(q1.emptyOfReady());
    ASSERT_TRUE(q1.emptyOfTotal());

    q1.push([&](int & v){ v = TEST_VALUE1; });
    q1.push([&](int & v){ v = TEST_VALUE2; });
    ASSERT_EQ(2, q1.size());

    q1.popAll();
    ASSERT_EQ(0, q1.size());
}

