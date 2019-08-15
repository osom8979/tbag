/**
 * @file   TmxTileSetTest.cpp
 * @brief  TmxTileSet class tester.
 * @author zer0
 * @date   2019-08-15
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTileSet.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTileSetTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTileSet, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTileSet, obj2)
}

TEST(TmxTileSetTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<tileset firstgid="1" name="isometric_grass_and_water" tilewidth="64" tileheight="64" tilecount="24" columns="4">
  <tileoffset x="0" y="16"/>
  <grid orientation="isometric" width="64" height="32"/>
  <image source="isometric_grass_and_water.png" width="256" height="384"/>
  <terraintypes>
    <terrain name="Grass" tile="0"/>
    <terrain name="Water" tile="22"/>
  </terraintypes>
  <tile id="0" terrain="0,0,0,0"/>
  <tile id="1" terrain="0,0,0,0"/>
  <wangsets>
    <wangset name="Grass and Water" tile="15">
      <wangcornercolor name="Grass" color="#8ab022" tile="0" probability="1"/>
      <wangcornercolor name="Water" color="#378dc2" tile="1" probability="1"/>
      <wangtile tileid="0" wangid="0x10101010"/>
      <wangtile tileid="1" wangid="0x10101010"/>
    </wangset>
  </wangsets>
</tileset>
)";

    TmxTileSet tileset;
    ASSERT_EQ(E_SUCCESS, tileset.read(TEST_XML));
    ASSERT_EQ(1, tileset.firstgid);
    ASSERT_STREQ("isometric_grass_and_water", tileset.name.c_str());
    ASSERT_EQ(64, tileset.tilewidth);
    ASSERT_EQ(64, tileset.tileheight);
    ASSERT_EQ(24, tileset.tilecount);
    ASSERT_EQ(4, tileset.columns);
    ASSERT_EQ(1, tileset.tile_offsets.size());
    ASSERT_EQ(1, tileset.grids.size());
    ASSERT_EQ(1, tileset.images.size());
    ASSERT_EQ(2, tileset.terrain_types.size());
    ASSERT_EQ(2, tileset.tiles.size());
    ASSERT_EQ(1, tileset.wang_sets.size());
    ASSERT_EQ(2, tileset.wang_sets.wang_sets[0].wang_corner_colors.size());
    ASSERT_EQ(2, tileset.wang_sets.wang_sets[0].wang_tiles.size());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, tileset.write(xml));

    TmxTileSet tileset2;
    ASSERT_EQ(E_SUCCESS, tileset2.read(xml));
    ASSERT_EQ(1, tileset2.firstgid);
    ASSERT_STREQ("isometric_grass_and_water", tileset2.name.c_str());
    ASSERT_EQ(64, tileset2.tilewidth);
    ASSERT_EQ(64, tileset2.tileheight);
    ASSERT_EQ(24, tileset2.tilecount);
    ASSERT_EQ(4, tileset2.columns);
    ASSERT_EQ(1, tileset2.tile_offsets.size());
    ASSERT_EQ(1, tileset2.grids.size());
    ASSERT_EQ(1, tileset2.images.size());
    ASSERT_EQ(2, tileset2.terrain_types.size());
    ASSERT_EQ(2, tileset2.tiles.size());
    ASSERT_EQ(1, tileset2.wang_sets.size());
    ASSERT_EQ(2, tileset2.wang_sets.wang_sets[0].wang_corner_colors.size());
    ASSERT_EQ(2, tileset2.wang_sets.wang_sets[0].wang_tiles.size());
}

