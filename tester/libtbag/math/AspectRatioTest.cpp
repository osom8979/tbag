/**
 * @file   AspectRatioTest.cpp
 * @brief  AspectRatio class tester.
 * @author zer0
 * @date   2018-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/math/AspectRatio.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(AspectRatioTest, Default)
{
    auto test = calcAspectRatio(800, 600);
    ASSERT_EQ(4, test.first);
    ASSERT_EQ(3, test.second);

    test = calcAspectRatio(1920, 1080);
    ASSERT_EQ(16, test.first);
    ASSERT_EQ(9, test.second);
}

TEST(AspectRatioTest, ScaleUp)
{
    geometry::Sizei size;
    size = scaleUpAspectRatio(geometry::Sizei(1024, 576),
                              geometry::Sizei(1024, 763));
    ASSERT_EQ(1024, size.width);
    ASSERT_EQ( 576, size.height);

    size = scaleUpAspectRatio(geometry::Sizei(200, 400),
                              geometry::Sizei(400, 800));
    ASSERT_EQ(400, size.width);
    ASSERT_EQ(800, size.height);

    size = scaleUpAspectRatio(geometry::Sizei(200, 400),
                              geometry::Sizei(300, 800));
    ASSERT_EQ(300, size.width);
    ASSERT_EQ(600, size.height);

    size = scaleUpAspectRatio(geometry::Sizei(200, 400),
                              geometry::Sizei(800, 800));
    ASSERT_EQ(400, size.width);
    ASSERT_EQ(800, size.height);
}

