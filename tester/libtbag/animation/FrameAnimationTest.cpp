/**
 * @file   FrameAnimationTest.cpp
 * @brief  FrameAnimation class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/FrameAnimation.hpp>

using namespace libtbag;
using namespace libtbag::animation;

TEST(FrameAnimationTest, Default)
{
    FrameAnimation object;
    ASSERT_TRUE(true);
}

TEST(FrameAnimationTest, GetNextFrameNumber)
{
    ASSERT_EQ(9, getNextFrameNumber(10, 100,  900));
    ASSERT_EQ(0, getNextFrameNumber(10, 100, 1000));
    ASSERT_EQ(1, getNextFrameNumber(10, 100, 1100));
}

