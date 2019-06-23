/**
 * @file   TmxPolylineTest.cpp
 * @brief  TmxPolyline class tester.
 * @author zer0
 * @date   2019-06-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxPolyline.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPolylineTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxPolyline, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxPolyline, obj2)
}

TEST(TmxPolylineTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<polyline points="0,0 75,78 133,82 176,179 274,183"/>)";

    TmxPolyline polyline;
    ASSERT_EQ(E_SUCCESS, polyline.read(TEST_XML));
    ASSERT_EQ(5, polyline.points.size());
    ASSERT_EQ(0, polyline.points[0].x);
    ASSERT_EQ(0, polyline.points[0].y);
    ASSERT_EQ(75, polyline.points[1].x);
    ASSERT_EQ(78, polyline.points[1].y);
    ASSERT_EQ(133, polyline.points[2].x);
    ASSERT_EQ(82, polyline.points[2].y);
    ASSERT_EQ(176, polyline.points[3].x);
    ASSERT_EQ(179, polyline.points[3].y);
    ASSERT_EQ(274, polyline.points[4].x);
    ASSERT_EQ(183, polyline.points[4].y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, polyline.write(xml));

    TmxPolyline polyline2;
    ASSERT_EQ(E_SUCCESS, polyline2.read(xml));
    ASSERT_EQ(5, polyline2.points.size());
    ASSERT_EQ(0, polyline2.points[0].x);
    ASSERT_EQ(0, polyline2.points[0].y);
    ASSERT_EQ(75, polyline2.points[1].x);
    ASSERT_EQ(78, polyline2.points[1].y);
    ASSERT_EQ(133, polyline2.points[2].x);
    ASSERT_EQ(82, polyline2.points[2].y);
    ASSERT_EQ(176, polyline2.points[3].x);
    ASSERT_EQ(179, polyline2.points[3].y);
    ASSERT_EQ(274, polyline2.points[4].x);
    ASSERT_EQ(183, polyline2.points[4].y);
}

