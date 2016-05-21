/**
 * @file   StateQueueTest.cpp
 * @brief  StateQueue class tester.
 * @author zer0
 * @date   2016-05-21
 */

#include <gtest/gtest.h>
#include <libtbag/container/StateQueue.hpp>

using namespace libtbag;
using namespace libtbag::container;

using QueueDemo = StateQueue<int, 3>;

TEST(StateQueueTest, Default)
{
    QueueDemo queue;

    QueueDemo q0;
    QueueDemo q1 = QueueDemo();
    QueueDemo q2 = q1;
    QueueDemo q3_move = QueueDemo();
    QueueDemo q4 = std::move(q3_move);

    ASSERT_EQ(QueueDemo::QUEUE_COUNT, 3);
}

