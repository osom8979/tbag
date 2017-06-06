/**
 * @file   MessageQueueTest.cpp
 * @brief  MessageQueue class tester.
 * @author zer0
 * @date   2017-06-06
 */

#include <gtest/gtest.h>
#include <libtbag/ipc/MessageQueue.hpp>

using namespace libtbag;
using namespace libtbag::ipc;

TEST(MessageQueueTest, Default)
{
    MessageQueue object;
    ASSERT_TRUE(true);
}

