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

TEST(MinMaxTest, getMinMax_01)
{
    std::vector<int> temp = {20, 10, 90, 80};
    int min(0), max(0);

    getMinMax(temp.begin(), temp.end(), min, max);
    ASSERT_EQ(10, min);
    ASSERT_EQ(90, max);

    getMinMax(temp.begin(), temp.begin(), min, max);
    ASSERT_EQ(20, min);
    ASSERT_EQ(20, max);
}

TEST(MinMaxTest, getMinMax_02)
{
    std::vector<float> temp = {0.1f, -0.009f, 10.0f, -200.0f, 2000.0f};
    float min(0), max(0);

    getMinMax(temp.begin(), temp.end(), min, max);
    ASSERT_FLOAT_EQ(-200.0f, min);
    ASSERT_FLOAT_EQ(2000.0f, max);
}

