/**
 * @file   swapTest.cpp
 * @brief  swap algorithm tester.
 * @author zer0
 * @date   2016-04-19
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/swap.hpp>

using namespace libtbag;

TEST(swapTest, Default)
{
    int v1 = 10;
    int v2 = 20;

    algorithm::swapWithPod(v1, v2);

    ASSERT_EQ(v1, 20);
    ASSERT_EQ(v2, 10);
}

