/**
 * @file   swapTest.cpp
 * @brief  swap algorithm tester.
 * @author zer0
 * @date   2016-04-19
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/Swap.hpp>

using namespace libtbag;

TEST(SwapTest, Default)
{
    int v1 = 100;
    int v2 = 200;

    algorithm::swapWithPod(v1, v2);
    ASSERT_EQ(200, v1);
    ASSERT_EQ(100, v2);
}

