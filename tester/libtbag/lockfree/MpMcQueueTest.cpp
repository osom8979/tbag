/**
 * @file   MpMcQueueTest.cpp
 * @brief  MpMcQueue class tester.
 * @author zer0
 * @date   2018-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/lockfree/MpMcQueue.hpp>

using namespace libtbag;
using namespace libtbag::lockfree;

TEST(MpMcQueueTest, Default)
{
    MpMcQueue queue(4);
}


