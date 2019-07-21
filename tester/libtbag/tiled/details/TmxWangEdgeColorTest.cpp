/**
 * @file   TmxWangEdgeColorTest.cpp
 * @brief  TmxWangEdgeColor class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxWangEdgeColor.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxWangEdgeColorTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxWangEdgeColor, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxWangEdgeColor, obj2)
}

TEST(TmxWangEdgeColorTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<wangedgecolor name="Grass" color="#97ff67" tile="80" probability="1"/>
)";

    TmxWangEdgeColor wang_edge_color;
    ASSERT_EQ(E_SUCCESS, wang_edge_color.read(TEST_XML));
    ASSERT_STREQ("Grass", wang_edge_color.name.c_str());
    ASSERT_STREQ("#97FF67", wang_edge_color.color.toRgb24String().c_str());
    ASSERT_EQ(80, wang_edge_color.tile);
    ASSERT_FLOAT_EQ(1, wang_edge_color.probability);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_edge_color.write(xml));

    TmxWangEdgeColor wang_edge_color2;
    ASSERT_EQ(E_SUCCESS, wang_edge_color2.read(xml));
    ASSERT_STREQ("Grass", wang_edge_color2.name.c_str());
    ASSERT_STREQ("#97FF67", wang_edge_color2.color.toRgb24String().c_str());
    ASSERT_EQ(80, wang_edge_color2.tile);
    ASSERT_FLOAT_EQ(1, wang_edge_color2.probability);
}

