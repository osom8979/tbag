/**
 * @file   TmxTileTest.cpp
 * @brief  TmxTile class tester.
 * @author zer0
 * @date   2019-08-11
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTile.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTileTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTile, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTile, obj2)
}

TEST(TmxTileTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<tile id="7" terrain="0,1,0,0"/>)";

    TmxTile tile;
    ASSERT_EQ(E_SUCCESS, tile.read(TEST_XML));
    ASSERT_EQ(7, tile.id);
    ASSERT_TRUE(tile.terrain.enable);
    ASSERT_EQ(0, tile.terrain.top_left);
    ASSERT_EQ(1, tile.terrain.top_right);
    ASSERT_EQ(0, tile.terrain.bottom_left);
    ASSERT_EQ(0, tile.terrain.bottom_right);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, tile.write(xml));

    TmxTile tile2;
    ASSERT_EQ(E_SUCCESS, tile2.read(xml));
    ASSERT_EQ(7, tile2.id);
    ASSERT_TRUE(tile2.terrain.enable);
    ASSERT_EQ(0, tile2.terrain.top_left);
    ASSERT_EQ(1, tile2.terrain.top_right);
    ASSERT_EQ(0, tile2.terrain.bottom_left);
    ASSERT_EQ(0, tile2.terrain.bottom_right);
}

