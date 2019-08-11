/**
 * @file   TmxObjectGroupTest.cpp
 * @brief  TmxObjectGroup class tester.
 * @author zer0
 * @date   2019-08-11
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxObjectGroup.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxObjectGroupTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxObjectGroup, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxObjectGroup, obj2)
}

TEST(TmxObjectGroupTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<objectgroup id="3" name="Objects">
  <object id="10" gid="282" x="413.333" y="225.333" width="16" height="16"/>
  <object id="11" gid="282" x="421.667" y="218" width="16" height="16"/>
</objectgroup>
)";

    TmxObjectGroup group;
    ASSERT_EQ(E_SUCCESS, group.read(TEST_XML));
    ASSERT_EQ(3, group.id);
    ASSERT_STREQ("Objects", group.name.c_str());
    ASSERT_EQ(2, group.objects.size());
    ASSERT_EQ(10, group.objects[0].id);
    ASSERT_EQ(11, group.objects[1].id);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, group.write(xml));

    TmxObjectGroup group2;
    ASSERT_EQ(E_SUCCESS, group2.read(xml));
    ASSERT_EQ(3, group2.id);
    ASSERT_STREQ("Objects", group2.name.c_str());
    ASSERT_EQ(2, group2.objects.size());
    ASSERT_EQ(10, group2.objects[0].id);
    ASSERT_EQ(11, group2.objects[1].id);
}

