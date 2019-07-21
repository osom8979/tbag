/**
 * @file   TmxWangTileTest.cpp
 * @brief  TmxWangTile class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxWangTile.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxWangTileTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxWangTile, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxWangTile, obj2)
}

TEST(TmxWangTileTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<wangtile tileid="35" wangid="0xA1B2C3D4"/>)";

    TmxWangTile wang_tile;
    ASSERT_EQ(E_SUCCESS, wang_tile.read(TEST_XML));
    ASSERT_EQ(35, wang_tile.tileid);
    ASSERT_EQ(0xA1B2C3D4, wang_tile.wangid);
    ASSERT_EQ(0xA, wang_tile.getCornerRight());
    ASSERT_EQ(0xB, wang_tile.getCornerBottom());
    ASSERT_EQ(0xC, wang_tile.getCornerLeft());
    ASSERT_EQ(0xD, wang_tile.getCornerTop());
    ASSERT_EQ(0xA, wang_tile.getCornerRight());
    ASSERT_EQ(0xB, wang_tile.getCornerBottom());
    ASSERT_EQ(0xC, wang_tile.getCornerLeft());
    ASSERT_EQ(0xD, wang_tile.getCornerTop());
    ASSERT_EQ(0x1, wang_tile.getEdgeTopRight());
    ASSERT_EQ(0x2, wang_tile.getEdgeBottomRight());
    ASSERT_EQ(0x3, wang_tile.getEdgeBottomLeft());
    ASSERT_EQ(0x4, wang_tile.getEdgeTopLeft());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_tile.write(xml));

    TmxWangTile wang_tile2;
    ASSERT_EQ(E_SUCCESS, wang_tile2.read(xml));
    ASSERT_EQ(35, wang_tile2.tileid);
    ASSERT_EQ(0xA1B2C3D4, wang_tile2.wangid);
    ASSERT_EQ(0xA, wang_tile2.getCornerRight());
    ASSERT_EQ(0xB, wang_tile2.getCornerBottom());
    ASSERT_EQ(0xC, wang_tile2.getCornerLeft());
    ASSERT_EQ(0xD, wang_tile2.getCornerTop());
    ASSERT_EQ(0x1, wang_tile2.getEdgeTopRight());
    ASSERT_EQ(0x2, wang_tile2.getEdgeBottomRight());
    ASSERT_EQ(0x3, wang_tile2.getEdgeBottomLeft());
    ASSERT_EQ(0x4, wang_tile2.getEdgeTopLeft());
}

