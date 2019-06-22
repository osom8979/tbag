/**
 * @file   TmxTileOffsetTest.cpp
 * @brief  TmxTileOffset class tester.
 * @author zer0
 * @date   2019-06-19
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTileOffset.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTileOffsetTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTileOffset, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTileOffset, obj2)
}

TEST(TmxTileOffsetTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<tileoffset x="10" y="20" />)";

    TmxTileOffset offset;
    ASSERT_EQ(E_SUCCESS, offset.read(TEST_XML));
    ASSERT_EQ(10, offset.x);
    ASSERT_EQ(20, offset.y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, offset.write(xml));

    TmxTileOffset offset2;
    ASSERT_EQ(E_SUCCESS, offset2.read(xml));
    ASSERT_EQ(10, offset2.x);
    ASSERT_EQ(20, offset2.y);
}

