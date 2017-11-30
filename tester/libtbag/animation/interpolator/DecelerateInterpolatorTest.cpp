/**
 * @file   DecelerateInterpolatorTest.cpp
 * @brief  DecelerateInterpolator class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/interpolator/DecelerateInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(DecelerateInterpolatorTest, Default)
{
    DecelerateInterpolator object;
    ASSERT_TRUE(true);
}

