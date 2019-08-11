/**
 * @file   TmxObjectTest.cpp
 * @brief  TmxObject class tester.
 * @author zer0
 * @date   2019-07-24
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxObject.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxObjectTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxObject, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxObject, obj2)
}

TEST(TmxObjectTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<object id="6" name="guard" type="NPC" x="277" y="18">
  <polyline points="0,0 75,78 133,82 176,179 274,183"/>
</object>
)";

    TmxObject object;
    ASSERT_EQ(E_SUCCESS, object.read(TEST_XML));
    ASSERT_EQ(6, object.id);
    ASSERT_STREQ("guard", object.name.c_str());
    ASSERT_STREQ("NPC", object.type.c_str());
    ASSERT_EQ(277, object.x);
    ASSERT_EQ(18, object.y);
    ASSERT_EQ(  5, object.polyline.points.size());
    ASSERT_EQ(  0, object.polyline.points[0].x);
    ASSERT_EQ(  0, object.polyline.points[0].y);
    ASSERT_EQ( 75, object.polyline.points[1].x);
    ASSERT_EQ( 78, object.polyline.points[1].y);
    ASSERT_EQ(133, object.polyline.points[2].x);
    ASSERT_EQ( 82, object.polyline.points[2].y);
    ASSERT_EQ(176, object.polyline.points[3].x);
    ASSERT_EQ(179, object.polyline.points[3].y);
    ASSERT_EQ(274, object.polyline.points[4].x);
    ASSERT_EQ(183, object.polyline.points[4].y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, object.write(xml));

    TmxObject object2;
    ASSERT_EQ(E_SUCCESS, object2.read(xml));
    ASSERT_EQ(6, object.id);
    ASSERT_STREQ("guard", object.name.c_str());
    ASSERT_STREQ("NPC", object.type.c_str());
    ASSERT_EQ(277, object2.x);
    ASSERT_EQ(18, object2.y);
    ASSERT_EQ(  5, object2.polyline.points.size());
    ASSERT_EQ(  0, object2.polyline.points[0].x);
    ASSERT_EQ(  0, object2.polyline.points[0].y);
    ASSERT_EQ( 75, object2.polyline.points[1].x);
    ASSERT_EQ( 78, object2.polyline.points[1].y);
    ASSERT_EQ(133, object2.polyline.points[2].x);
    ASSERT_EQ( 82, object2.polyline.points[2].y);
    ASSERT_EQ(176, object2.polyline.points[3].x);
    ASSERT_EQ(179, object2.polyline.points[3].y);
    ASSERT_EQ(274, object2.polyline.points[4].x);
    ASSERT_EQ(183, object2.polyline.points[4].y);
}

