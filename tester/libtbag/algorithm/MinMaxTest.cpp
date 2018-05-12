/**
 * @file   MinMaxTest.cpp
 * @brief  MinMax class tester.
 * @author zer0
 * @date   2018-05-12
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/MinMax.hpp>

using namespace libtbag;
using namespace libtbag::algorithm;

TEST(MinMaxTest, Default)
{
    ASSERT_EQ(1, getMin<int>({5, 9, 1, 2, 3, 8, 8}));
    ASSERT_EQ(9, getMax<int>({5, 9, 1, 2, 3, 8, 8}));

    ASSERT_EQ(5, getMin(5, 9));
    ASSERT_EQ(5, getMin(9, 5));

    ASSERT_EQ(9, getMax(5, 9));
    ASSERT_EQ(9, getMax(9, 5));
}

