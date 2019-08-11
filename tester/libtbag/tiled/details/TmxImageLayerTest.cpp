/**
 * @file   TmxImageLayerTest.cpp
 * @brief  TmxImageLayer class tester.
 * @author zer0
 * @date   2019-08-11
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxImageLayer.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxImageLayerTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxImageLayer, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxImageLayer, obj2)
}

TEST(TmxImageLayerTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<imagelayer id="5" name="layer">
  <image source="tmw_desert_spacing.png" trans="#ff00ff" width="265" height="199"/>
</imagelayer>
)";

    TmxImageLayer layer;
    ASSERT_EQ(E_SUCCESS, layer.read(TEST_XML));
    ASSERT_EQ(5, layer.id);
    ASSERT_STREQ("layer", layer.name.c_str());
    ASSERT_STREQ("tmw_desert_spacing.png", layer.image.source.c_str());
    ASSERT_STREQ("#FF00FF", layer.image.trans.toRgb24String().c_str());
    ASSERT_EQ(265, layer.image.width);
    ASSERT_EQ(199, layer.image.height);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, layer.write(xml));

    TmxImageLayer layer2;
    ASSERT_EQ(E_SUCCESS, layer2.read(xml));
    ASSERT_EQ(5, layer2.id);
    ASSERT_STREQ("layer", layer2.name.c_str());
    ASSERT_STREQ("tmw_desert_spacing.png", layer2.image.source.c_str());
    ASSERT_STREQ("#FF00FF", layer2.image.trans.toRgb24String().c_str());
    ASSERT_EQ(265, layer2.image.width);
    ASSERT_EQ(199, layer2.image.height);
}

