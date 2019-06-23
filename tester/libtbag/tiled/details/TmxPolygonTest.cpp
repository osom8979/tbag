/**
 * @file   TmxPolygonTest.cpp
 * @brief  TmxPolygon class tester.
 * @author zer0
 * @date   2019-06-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxPolygon.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPolygonTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxPolygon, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxPolygon, obj2)
}

TEST(TmxPolygonTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<polygon points="0,0 55,-23 96,-117"/>)";

    TmxPolygon polygon;
    ASSERT_EQ(E_SUCCESS, polygon.read(TEST_XML));
    ASSERT_EQ(3, polygon.points.size());
    ASSERT_EQ(0, polygon.points[0].x);
    ASSERT_EQ(0, polygon.points[0].y);
    ASSERT_EQ(55, polygon.points[1].x);
    ASSERT_EQ(-23, polygon.points[1].y);
    ASSERT_EQ(96, polygon.points[2].x);
    ASSERT_EQ(-117, polygon.points[2].y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, polygon.write(xml));

    TmxPolygon polygon2;
    ASSERT_EQ(E_SUCCESS, polygon2.read(xml));
    ASSERT_EQ(3, polygon2.points.size());
    ASSERT_EQ(0, polygon2.points[0].x);
    ASSERT_EQ(0, polygon2.points[0].y);
    ASSERT_EQ(55, polygon2.points[1].x);
    ASSERT_EQ(-23, polygon2.points[1].y);
    ASSERT_EQ(96, polygon2.points[2].x);
    ASSERT_EQ(-117, polygon2.points[2].y);
}

