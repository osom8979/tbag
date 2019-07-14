/**
 * @file   TmxWangCornerColorTest.cpp
 * @brief  TmxWangCornerColor class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxWangCornerColor.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxWangCornerColorTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxWangCornerColor, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxWangCornerColor, obj2)
}

TEST(TmxWangCornerColorTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<wangcornercolor name="Stone" color="#ffefaf" tile="17" probability="0.25"/>
)";

    TmxWangCornerColor wang_corner_color;
    ASSERT_EQ(E_SUCCESS, wang_corner_color.read(TEST_XML));

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_corner_color.write(xml));

    TmxWangCornerColor wang_corner_color2;
    ASSERT_EQ(E_SUCCESS, wang_corner_color2.read(xml));
}

