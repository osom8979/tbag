/**
 * @file   TmxMapTest.cpp
 * @brief  TmxMap class tester.
 * @author zer0
 * @date   2019-08-15
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxMap.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxMapTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxMap, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxMap, obj2)
}

TEST(TmxMapTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<?xml version="1.0" encoding="UTF-8"?>
<map version="1.0" orientation="hexagonal" renderorder="right-down" width="20" height="20" tilewidth="14" tileheight="12" hexsidelength="6" staggeraxis="y" staggerindex="odd" nextobjectid="2">
 <tileset firstgid="1" name="hex mini" tilewidth="18" tileheight="18">
  <tileoffset x="0" y="1"/>
  <image source="hexmini.png" width="106" height="72"/>
 </tileset>
 <layer name="Ground" width="20" height="20">
  <data encoding="base64" compression="zlib">
   eJyl1FEKhDAMBNBSt6jVaL3/Za2QwDAkVdiPQda2zyTonimlU1N6Ws+lkZ6l56AUXcPY2qlniv5uL5Z5BdyDvFXXMoX3Rp44axl6nqFejj3LLK6xgmf3Zg06Qs+O+qiaDOZOVgXPs7jfCme8Hkce1+fNlGdlM3myDTzc580fz1htW2Baj15/R/J72wLvcVZN5HnzGnmVPJ5hNH+0dt33j4ex91TARUs+WjNZz/fewKvJfy+/1naR+dX7OfdEnUYefyOeZZ7Vht/b5HjefxJbO1iTE7YWuEpg5hfPzi8D782x3Mg7DV4=
  </data>
 </layer>
</map>
)";

    TmxMap map;
    ASSERT_EQ(E_SUCCESS, map.read(TEST_XML));
    ASSERT_STREQ("1.0", map.version.c_str());
    ASSERT_EQ(TmxMap::Orientation::HEXAGONAL, map.orientation);
    ASSERT_EQ(TmxMap::RenderOrder::RIGHT_DOWN, map.render_order);
    ASSERT_EQ(20, map.width);
    ASSERT_EQ(20, map.height);
    ASSERT_EQ(14, map.tile_width);
    ASSERT_EQ(12, map.tile_height);
    ASSERT_EQ(6, map.hex_side_length);
    ASSERT_EQ(TmxMap::StaggerAxis::Y, map.stagger_axis);
    ASSERT_EQ(TmxMap::StaggerIndex::ODD, map.stagger_index);
    ASSERT_EQ(2, map.next_object_id);
    ASSERT_EQ(1, map.tilesets.size());
    ASSERT_EQ(1, map.tilesets[0].tile_offsets.size());
    ASSERT_EQ(0, map.tilesets[0].tile_offsets[0].x);
    ASSERT_EQ(1, map.tilesets[0].tile_offsets[0].y);
    ASSERT_EQ(1, map.tilesets[0].images.size());
    ASSERT_STREQ("hexmini.png", map.tilesets[0].images[0].source.c_str());
    ASSERT_EQ(106, map.tilesets[0].images[0].width);
    ASSERT_EQ(72, map.tilesets[0].images[0].height);
    ASSERT_EQ(1, map.layers.size());
    ASSERT_STREQ("Ground", map.layers[0].name.c_str());
    ASSERT_EQ(20, map.layers[0].width);
    ASSERT_EQ(20, map.layers[0].height);
    ASSERT_EQ(TmxData::Encoding::BASE64, map.layers[0].data.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, map.layers[0].data.compression);
    ASSERT_FALSE(map.layers[0].data.gids.empty());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, map.write(xml));

    TmxMap map2;
    ASSERT_EQ(E_SUCCESS, map2.read(xml));
    ASSERT_STREQ("1.0", map2.version.c_str());
    ASSERT_EQ(TmxMap::Orientation::HEXAGONAL, map2.orientation);
    ASSERT_EQ(TmxMap::RenderOrder::RIGHT_DOWN, map2.render_order);
    ASSERT_EQ(20, map2.width);
    ASSERT_EQ(20, map2.height);
    ASSERT_EQ(14, map2.tile_width);
    ASSERT_EQ(12, map2.tile_height);
    ASSERT_EQ(6, map2.hex_side_length);
    ASSERT_EQ(TmxMap::StaggerAxis::Y, map2.stagger_axis);
    ASSERT_EQ(TmxMap::StaggerIndex::ODD, map2.stagger_index);
    ASSERT_EQ(2, map2.next_object_id);
    ASSERT_EQ(1, map2.tilesets.size());
    ASSERT_EQ(1, map2.tilesets[0].tile_offsets.size());
    ASSERT_EQ(0, map2.tilesets[0].tile_offsets[0].x);
    ASSERT_EQ(1, map2.tilesets[0].tile_offsets[0].y);
    ASSERT_EQ(1, map2.tilesets[0].images.size());
    ASSERT_STREQ("hexmini.png", map2.tilesets[0].images[0].source.c_str());
    ASSERT_EQ(106, map2.tilesets[0].images[0].width);
    ASSERT_EQ(72, map2.tilesets[0].images[0].height);
    ASSERT_EQ(1, map2.layers.size());
    ASSERT_STREQ("Ground", map2.layers[0].name.c_str());
    ASSERT_EQ(20, map2.layers[0].width);
    ASSERT_EQ(20, map2.layers[0].height);
    ASSERT_EQ(TmxData::Encoding::BASE64, map2.layers[0].data.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, map2.layers[0].data.compression);
    ASSERT_FALSE(map2.layers[0].data.gids.empty());
}

