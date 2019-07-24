/**
 * @file   TmxImageTest.cpp
 * @brief  TmxImage class tester.
 * @author zer0
 * @date   2019-07-24
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxImage.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxImageTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxImage, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxImage, obj2)
}

TEST(TmxImageTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<image source="tmw_desert_spacing.png" trans="#ff00ff" width="265" height="199"/>
)";

    TmxImage image;
    ASSERT_EQ(E_SUCCESS, image.read(TEST_XML));
    ASSERT_STREQ("tmw_desert_spacing.png", image.source.c_str());
    ASSERT_EQ(265, image.width);
    ASSERT_EQ(199, image.height);
    ASSERT_STREQ("#FF00FF", image.trans.toRgb24String().c_str());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, image.write(xml));

    TmxImage image2;
    ASSERT_EQ(E_SUCCESS, image2.read(xml));
    ASSERT_STREQ("tmw_desert_spacing.png", image2.source.c_str());
    ASSERT_EQ(265, image2.width);
    ASSERT_EQ(199, image2.height);
    ASSERT_STREQ("#FF00FF", image2.trans.toRgb24String().c_str());
}

