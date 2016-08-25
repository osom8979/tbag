/**
 * @file   FrameInterpolatorTest.cpp
 * @brief  FrameInterpolator class tester.
 * @author zer0
 * @date   2016-05-23
 */

#include <gtest/gtest.h>
#include <libtbag/animation/FrameInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;

TEST(FrameInterpolatorTest, Default)
{
    long const START        =   0;
    long const FPS          = 100;
    int  const FRAME_COUNT  =  10;

    FrameInterpolator interpolator;
    interpolator.set(START, FPS, FRAME_COUNT);

    ASSERT_EQ(9, static_cast<int>(interpolator.getInterpolation( 900)));
    ASSERT_EQ(0, static_cast<int>(interpolator.getInterpolation(1000)));
    ASSERT_EQ(1, static_cast<int>(interpolator.getInterpolation(1100)));
}

