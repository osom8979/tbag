/**
 * @file   AspectRatioTest.cpp
 * @brief  AspectRatio class tester.
 * @author zer0
 * @date   2018-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/math/AspectRatio.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(AspectRatioTest, Default)
{
    auto test = calcAspectRatio(800, 600);
    ASSERT_EQ(4, test.first);
    ASSERT_EQ(3, test.second);

    test = calcAspectRatio(1920, 1080);
    ASSERT_EQ(16, test.first);
    ASSERT_EQ(9, test.second);
}

