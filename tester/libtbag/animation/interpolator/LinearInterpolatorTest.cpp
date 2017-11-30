/**
 * @file   LinearInterpolatorTest.cpp
 * @brief  LinearInterpolator class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/interpolator/LinearInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(LinearInterpolatorTest, Default)
{
    LinearInterpolator object;
    ASSERT_TRUE(true);
}

