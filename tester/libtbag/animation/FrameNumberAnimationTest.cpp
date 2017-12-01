/**
 * @file   FrameNumberAnimationTest.cpp
 * @brief  FrameNumberAnimation class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/FrameNumberAnimation.hpp>
#include <libtbag/animation/interpolator/LinearInterpolator.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(FrameNumberAnimationTest, Default)
{
    FrameNumberAnimation ani(10);
    ani.atParams().interpolator.reset(new LinearInterpolator());
    ani.atParams().setRestartMode();
    ani.atParams().repeat_count = 2;
    ani.atParams().start_offset = std::chrono::milliseconds(0);
    ani.atParams().duration     = std::chrono::milliseconds(10);

    auto tp = time::TimePoint::now();
    ani.start(tp);
    ani.update(tp);

    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(0, ani.getCurrentFrame());

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(1, ani.getCurrentFrame());

    tp += std::chrono::milliseconds(8);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(9, ani.getCurrentFrame());

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(0, ani.getCurrentFrame());

    tp += std::chrono::milliseconds(9);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(9, ani.getCurrentFrame());

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_TRUE(ani.isFinished());
    ASSERT_EQ(0, ani.getCurrentFrame());
}

