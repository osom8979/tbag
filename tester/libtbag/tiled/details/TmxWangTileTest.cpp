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
    char const * const TEST_XML = R"(<wangtile tileid="35" wangid="0x20203020"/>)";

    TmxWangTile wang_tile;
    ASSERT_EQ(E_SUCCESS, wang_tile.read(TEST_XML));

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_tile.write(xml));

    TmxWangTile wang_tile2;
    ASSERT_EQ(E_SUCCESS, wang_tile2.read(xml));
}

