/**
 * @file   FindTest.cpp
 * @brief  Find class tester.
 * @author zer0
 * @date   2017-07-20
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/Find.hpp>
#include <vector>

using namespace libtbag;
using namespace libtbag::algorithm;

TEST(FindTest, Default)
{
    std::vector<int> temp = {20, 10, 90, 80};
    int min(0), max(0);

    algorithm::findMinMax(temp.begin(), temp.end(), &min, &max);
    ASSERT_EQ(10, min);
    ASSERT_EQ(90, max);

    algorithm::findMinMax(temp.begin(), temp.begin(), &min, &max);
    ASSERT_EQ(20, min);
    ASSERT_EQ(20, max);
}

