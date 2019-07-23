/**
 * @file   TmxPropertiesTest.cpp
 * @brief  TmxProperties class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxProperties.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPropertiesTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxProperties, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxProperties, obj2)
}

TEST(TmxPropertiesTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<properties>
  <property name="door" value="true"/>
</properties>
)";

    TmxProperties properties;
    ASSERT_EQ(E_SUCCESS, properties.read(TEST_XML));
    ASSERT_EQ(1, properties.properties.size());
    ASSERT_STREQ("door", properties.properties[0].name.c_str());
    ASSERT_STREQ("true", properties.properties[0].value.c_str());
    ASSERT_TRUE(properties.properties[0].toBool());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, properties.write(xml));

    TmxProperties properties2;
    ASSERT_EQ(E_SUCCESS, properties2.read(xml));
    ASSERT_EQ(1, properties2.properties.size());
    ASSERT_STREQ("door", properties2.properties[0].name.c_str());
    ASSERT_STREQ("true", properties2.properties[0].value.c_str());
    ASSERT_TRUE(properties2.properties[0].toBool());
}

