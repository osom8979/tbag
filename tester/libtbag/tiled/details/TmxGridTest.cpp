/**
 * @file   TmxGridTest.cpp
 * @brief  TmxGrid class tester.
 * @author zer0
 * @date   2019-06-19
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxGrid.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>
#include <iostream>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxGridTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxGrid, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxGrid, obj2)
}

TEST(TmxGridTest, Read)
{
    char const * const TEST_XML = R"(<grid width="10" height="20" orientation="isometric" />)";

    TmxGrid grid;
    ASSERT_EQ(E_SUCCESS, grid.read(TEST_XML));
    ASSERT_EQ(10, grid.width);
    ASSERT_EQ(20, grid.height);
    ASSERT_EQ(TmxGrid::Orientation::ISOMETRIC, grid.orientation);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, grid.write(xml));

    TmxGrid grid2;
    ASSERT_EQ(E_SUCCESS, grid2.read(xml));
    ASSERT_EQ(10, grid2.width);
    ASSERT_EQ(20, grid2.height);
    ASSERT_EQ(TmxGrid::Orientation::ISOMETRIC, grid2.orientation);
}

