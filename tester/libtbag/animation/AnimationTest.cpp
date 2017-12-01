/**
 * @file   AnimationTest.cpp
 * @brief  Animation class tester.
 * @author zer0
 * @date   2017-11-30
 */

#include <gtest/gtest.h>
#include <libtbag/animation/Animation.hpp>
#include <libtbag/animation/interpolator/LinearInterpolator.hpp>
#include <libtbag/algorithm/Equals.hpp>

using namespace libtbag;
using namespace libtbag::animation;
using namespace libtbag::animation::interpolator;

TEST(AnimationTest, GetNextFrameNumber)
{
    ASSERT_EQ(9, Animation::getNextFrameNumber(10, 100,  900));
    ASSERT_EQ(0, Animation::getNextFrameNumber(10, 100, 1000));
    ASSERT_EQ(1, Animation::getNextFrameNumber(10, 100, 1100));
}

TEST(AnimationTest, CachedAnimation)
{
    CachedAnimation ani;
    ani.atParams().interpolator.reset(new LinearInterpolator());
    ani.atParams().setRestartMode();
    ani.atParams().repeat_count = 2;
    ani.atParams().start_offset = std::chrono::milliseconds(0);
    ani.atParams().duration     = std::chrono::milliseconds(10);

    auto tp = time::TimePoint::now();
    ani.setStart(tp);
    ani.start();
    ani.update(tp);

    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(0), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(1), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.1f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.1f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(2), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.2f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.2f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(6);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(8), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.8f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.8f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(9), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.9f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.9f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(0), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(9);
    ani.update(tp);
    ASSERT_FALSE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(9), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.9f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.9f, ani.getInterpolated()));

    tp += std::chrono::milliseconds(1);
    ani.update(tp);
    ASSERT_TRUE(ani.isFinished());
    ASSERT_EQ(std::chrono::milliseconds(0), ani.getDuration());
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getRatio()));
    ASSERT_TRUE(algorithm::equals(0.0f, ani.getInterpolated()));
}

