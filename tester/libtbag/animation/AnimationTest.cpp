/**
 * @file   AnimationTest.cpp
 * @brief  Animation class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/Animation.hpp>

using namespace libtbag;
using namespace libtbag::animation;

TEST(AnimationTest, GetNextFrameNumber)
{
    ASSERT_EQ(9, getNextFrameNumber(10, 100,  900));
    ASSERT_EQ(0, getNextFrameNumber(10, 100, 1000));
    ASSERT_EQ(1, getNextFrameNumber(10, 100, 1100));
}

