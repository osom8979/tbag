/**
 * @file   AverageTest.cpp
 * @brief  Average class tester.
 * @author zer0
 * @date   2017-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/math/Average.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(AverageTest, Default)
{
    float average = static_cast<float>(10 + 20 + 30 + 40 + 50) / 5.0;
    float prev_average = static_cast<float>(10 + 20 + 30 + 40) / 4.0;
    ASSERT_EQ(average, averageStep(prev_average, 50.0f, 5));
}

