/**
 * @file   BoundMpMcQueueTest.cpp
 * @brief  BoundMpMcQueue class tester.
 * @author zer0
 * @date   2018-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/BoundMpMcQueue.hpp>

using namespace libtbag;
using namespace libtbag::lockfree;

TEST(BoundMpMcQueueTest, Default)
{
    BoundMpMcQueue object;
    ASSERT_TRUE(true);
}

