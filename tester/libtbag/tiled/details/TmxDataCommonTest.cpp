/**
 * @file   TmxDataCommonTest.cpp
 * @brief  TmxDataCommon class tester.
 * @author zer0
 * @date   2019-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxDataCommon.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxDataCommonTest, Base64)
{
    TmxDataCommon::GlobalTileIds const IDS = { 1, 2, 3, 4 };
    auto const BASE64_TEXT = TmxDataCommon::writeToBase64(IDS.data(), IDS.size());
    ASSERT_FALSE(BASE64_TEXT.empty());

    auto const GLOBAL_TILE_IDS = TmxDataCommon::readFromBase64(BASE64_TEXT);
    ASSERT_EQ(GLOBAL_TILE_IDS.size(), IDS.size());
    ASSERT_EQ(GLOBAL_TILE_IDS[0], IDS[0]);
    ASSERT_EQ(GLOBAL_TILE_IDS[1], IDS[1]);
    ASSERT_EQ(GLOBAL_TILE_IDS[2], IDS[2]);
    ASSERT_EQ(GLOBAL_TILE_IDS[3], IDS[3]);
}

TEST(TmxDataCommonTest, ZlibBase64)
{
    TmxDataCommon::GlobalTileIds const IDS = { 1, 2, 3, 4 };
    auto const BASE64_TEXT = TmxDataCommon::writeToZlibBase64(IDS.data(), IDS.size());
    ASSERT_FALSE(BASE64_TEXT.empty());

    auto const GLOBAL_TILE_IDS = TmxDataCommon::readFromZlibBase64(BASE64_TEXT);
    ASSERT_EQ(GLOBAL_TILE_IDS.size(), IDS.size());
    ASSERT_EQ(GLOBAL_TILE_IDS[0], IDS[0]);
    ASSERT_EQ(GLOBAL_TILE_IDS[1], IDS[1]);
    ASSERT_EQ(GLOBAL_TILE_IDS[2], IDS[2]);
    ASSERT_EQ(GLOBAL_TILE_IDS[3], IDS[3]);
}

