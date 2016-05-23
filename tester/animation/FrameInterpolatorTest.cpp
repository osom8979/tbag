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
    long const START_MILLISEC =   0;
    long const FPS_MILLISEC   = 100;
    int  const FRAME_SIZE     =  10;

    FrameInterpolator interpolator;
    interpolator.set(START_MILLISEC, FPS_MILLISEC, FRAME_SIZE);

    ASSERT_EQ(static_cast<int>(interpolator.getInterpolation( 900)), 9);
    ASSERT_EQ(static_cast<int>(interpolator.getInterpolation(1000)), 0);
    ASSERT_EQ(static_cast<int>(interpolator.getInterpolation(1100)), 1);
}

