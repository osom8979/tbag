/**
 * @file   TmxTerrainTest.cpp
 * @brief  TmxTerrain class tester.
 * @author zer0
 * @date   2019-07-22
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTerrain.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTerrainTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTerrain, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTerrain, obj2)
}

TEST(TmxTerrainTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<terrain name="Brick" tile="9">
  <property name="spawncount" type="int" value="5"/>
</terrain>
)";

    TmxTerrain terrain;
    ASSERT_EQ(E_SUCCESS, terrain.read(TEST_XML));
    ASSERT_STREQ("Brick", terrain.name.c_str());
    ASSERT_EQ(9, terrain.tile);
    ASSERT_EQ(1, terrain.properties.size());
    ASSERT_STREQ("spawncount", terrain.properties[0].name.c_str());
    ASSERT_EQ(TmxProperty::Type::INT, terrain.properties[0].type);
    ASSERT_EQ(5, terrain.properties[0].toInt());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, terrain.write(xml));

    TmxTerrain terrain2;
    ASSERT_EQ(E_SUCCESS, terrain2.read(xml));
    ASSERT_STREQ("Brick", terrain2.name.c_str());
    ASSERT_EQ(9, terrain2.tile);
    ASSERT_EQ(1, terrain2.properties.size());
    ASSERT_STREQ("spawncount", terrain2.properties[0].name.c_str());
    ASSERT_EQ(TmxProperty::Type::INT, terrain2.properties[0].type);
    ASSERT_EQ(5, terrain2.properties[0].toInt());
}

