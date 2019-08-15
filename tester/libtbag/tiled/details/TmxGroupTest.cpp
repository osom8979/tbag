/**
 * @file   TmxGroupTest.cpp
 * @brief  TmxGroup class tester.
 * @author zer0
 * @date   2019-08-15
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxGroup.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxGroupTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxGroup, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxGroup, obj2)
}

TEST(TmxGroupTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<group id="1" name="All" offsetx="2" offsety="3" opacity="0" visible="0" />
)";

    TmxGroup group;
    ASSERT_EQ(E_SUCCESS, group.read(TEST_XML));
    ASSERT_EQ(1, group.id);
    ASSERT_STREQ("All", group.name.c_str());
    ASSERT_EQ(2, group.offsetx);
    ASSERT_EQ(3, group.offsety);
    ASSERT_EQ(0, group.opacity);
    ASSERT_EQ(0, group.visible);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, group.write(xml));

    TmxGroup group2;
    ASSERT_EQ(E_SUCCESS, group2.read(xml));
    ASSERT_EQ(1, group2.id);
    ASSERT_STREQ("All", group2.name.c_str());
    ASSERT_EQ(2, group2.offsetx);
    ASSERT_EQ(3, group2.offsety);
    ASSERT_EQ(0, group2.opacity);
    ASSERT_EQ(0, group2.visible);
}

