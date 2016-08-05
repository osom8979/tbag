/**
 * @file   SafetyPrepareQueueTest.cpp
 * @brief  SafetyPrepareQueue class tester.
 * @author zer0
 * @date   2016-08-04
 */

#include <gtest/gtest.h>
#include <libtbag/container/SafetyPrepareQueue.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(SafetyPrepareQueueTest, Default)
{
    int const TEST_VALUE = 100;

    SafetyPrepareQueue<int> queue;

    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);

    auto & prepare = queue.prepare();
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);

    prepare.at() = TEST_VALUE;

    queue.push(prepare);
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);

    auto & pop = queue.pop();
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 1U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);

    ASSERT_EQ(pop.at(), TEST_VALUE);

    queue.readEnd(pop);
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfReadingSet());
    ASSERT_TRUE(queue.emptyOfRemoveSet());
}

TEST(SafetyPrepareQueueTest, popAndReadEnd)
{
    SafetyPrepareQueue<int> queue;

    queue.push(queue.prepare());
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);

    ASSERT_TRUE(queue.popAndReadEnd());
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);
}

TEST(SafetyPrepareQueueTest, autoOperator)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.autoPrepare();
        ASSERT_EQ(queue.size(), 0U);
        ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
        ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);
    }
    ASSERT_EQ(queue.size(), 1U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);

    {
        auto pop = queue.autoPop();
        ASSERT_EQ(queue.size(), 0U);
        ASSERT_EQ(queue.sizeOfReadingSet(), 1U);
        ASSERT_EQ(queue.sizeOfRemoveSet(), 0U);
    }
    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);
}

TEST(SafetyPrepareQueueTest, cancel)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.autoPrepare();
        ASSERT_EQ(queue.size(), 0U);
        ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
        ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);
        prepare->cancel();
    }

    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReadingSet(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveSet(), 1U);
}

