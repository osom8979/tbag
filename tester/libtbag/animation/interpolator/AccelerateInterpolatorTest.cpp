/**
 * @file   AccelerateInterpolatorTest.cpp
 * @brief  AccelerateInterpolator class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/interpolator/AccelerateInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(AccelerateInterpolatorTest, Default)
{
    AccelerateInterpolator object;
    ASSERT_TRUE(true);
}

