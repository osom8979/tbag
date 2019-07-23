/**
 * @file   TmxTerrainTypesTest.cpp
 * @brief  TmxTerrainTypes class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTerrainTypes.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTerrainTypesTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTerrainTypes, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTerrainTypes, obj2)
}

TEST(TmxTerrainTypesTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<terraintypes>
  <terrain name="Desert" tile="29"/>
  <terrain name="Brick" tile="9"/>
  <terrain name="Cobblestone" tile="33"/>
  <terrain name="Dirt" tile="14"/>
</terraintypes>
)";

    TmxTerrainTypes terrain_types;
    ASSERT_EQ(E_SUCCESS, terrain_types.read(TEST_XML));
    ASSERT_EQ(4, terrain_types.terrains.size());
    ASSERT_STREQ("Desert", terrain_types.terrains[0].name.c_str());
    ASSERT_EQ(29, terrain_types.terrains[0].tile);
    ASSERT_STREQ("Brick", terrain_types.terrains[1].name.c_str());
    ASSERT_EQ(9, terrain_types.terrains[1].tile);
    ASSERT_STREQ("Cobblestone", terrain_types.terrains[2].name.c_str());
    ASSERT_EQ(33, terrain_types.terrains[2].tile);
    ASSERT_STREQ("Dirt", terrain_types.terrains[3].name.c_str());
    ASSERT_EQ(14, terrain_types.terrains[3].tile);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, terrain_types.write(xml));

    TmxTerrainTypes terrain_types2;
    ASSERT_EQ(E_SUCCESS, terrain_types2.read(xml));
    ASSERT_EQ(4, terrain_types2.terrains.size());
    ASSERT_STREQ("Desert", terrain_types2.terrains[0].name.c_str());
    ASSERT_EQ(29, terrain_types2.terrains[0].tile);
    ASSERT_STREQ("Brick", terrain_types2.terrains[1].name.c_str());
    ASSERT_EQ(9, terrain_types2.terrains[1].tile);
    ASSERT_STREQ("Cobblestone", terrain_types2.terrains[2].name.c_str());
    ASSERT_EQ(33, terrain_types2.terrains[2].tile);
    ASSERT_STREQ("Dirt", terrain_types2.terrains[3].name.c_str());
    ASSERT_EQ(14, terrain_types2.terrains[3].tile);
}

