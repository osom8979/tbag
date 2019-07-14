/**
 * @file   TmxLayerTest.cpp
 * @brief  TmxLayer class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxLayer.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxLayerTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxLayer, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxLayer, obj2)
}

TEST(TmxLayerTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<layer id="8" name="test" width="2" height="2">
  <data encoding="base64" compression="zlib">
    eJxjZGBgYAJiZgYIAAAAUAAH
  </data>
</layer>
)";

    TmxLayer layer;
    ASSERT_EQ(E_SUCCESS, layer.read(TEST_XML));
    ASSERT_EQ(8, layer.id);
    ASSERT_STREQ("test", layer.name.c_str());
    ASSERT_EQ(2, layer.width);
    ASSERT_EQ(2, layer.height);
    ASSERT_TRUE(layer.properties.empty());
    ASSERT_EQ(TmxData::Encoding::BASE64, layer.data.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, layer.data.compression);
    ASSERT_EQ(4, layer.data.gids.size());
    ASSERT_EQ(1, layer.data.gids[0]);
    ASSERT_EQ(2, layer.data.gids[1]);
    ASSERT_EQ(3, layer.data.gids[2]);
    ASSERT_EQ(0, layer.data.gids[3]);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, layer.write(xml));

    TmxLayer layer2;
    ASSERT_EQ(E_SUCCESS, layer2.read(xml));
    ASSERT_EQ(8, layer2.id);
    ASSERT_STREQ("test", layer2.name.c_str());
    ASSERT_EQ(2, layer2.width);
    ASSERT_EQ(2, layer2.height);
    ASSERT_TRUE(layer2.properties.empty());
    ASSERT_EQ(TmxData::Encoding::BASE64, layer2.data.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, layer2.data.compression);
    ASSERT_EQ(4, layer2.data.gids.size());
    ASSERT_EQ(1, layer2.data.gids[0]);
    ASSERT_EQ(2, layer2.data.gids[1]);
    ASSERT_EQ(3, layer2.data.gids[2]);
    ASSERT_EQ(0, layer2.data.gids[3]);
}

