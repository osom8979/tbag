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

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_edge_color.write(xml));

    TmxWangEdgeColor wang_edge_color2;
    ASSERT_EQ(E_SUCCESS, wang_edge_color2.read(xml));
}

