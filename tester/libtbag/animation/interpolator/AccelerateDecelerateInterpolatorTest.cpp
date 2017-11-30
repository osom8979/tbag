/**
 * @file   AccelerateDecelerateInterpolatorTest.cpp
 * @brief  AccelerateDecelerateInterpolator class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/interpolator/AccelerateDecelerateInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(AccelerateDecelerateInterpolatorTest, Default)
{
    AccelerateDecelerateInterpolator object;
    ASSERT_TRUE(true);
}

