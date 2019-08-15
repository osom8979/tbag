/**
 * @file   TmxTemplateTest.cpp
 * @brief  TmxTemplate class tester.
 * @author zer0
 * @date   2019-08-15
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxTemplate.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTemplateTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxTemplate, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxTemplate, obj2)
}

TEST(TmxTemplateTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<?xml version="1.0" encoding="UTF-8"?>
<template>
  <tileset firstgid="1" source="desert.tsx"/>
  <object name="cactus" gid="31" width="81" height="101"/>
</template>
)";

    TmxTemplate tmp;
    ASSERT_EQ(E_SUCCESS, tmp.read(TEST_XML));
    ASSERT_EQ(1, tmp.tile_sets.size());
    ASSERT_EQ(1, tmp.tile_sets[0].firstgid);
    ASSERT_STREQ("desert.tsx", tmp.tile_sets[0].source.c_str());
    ASSERT_EQ(1, tmp.objects.size());
    ASSERT_STREQ("cactus", tmp.objects[0].name.c_str());
    ASSERT_EQ(31, tmp.objects[0].gid);
    ASSERT_EQ(81, tmp.objects[0].width);
    ASSERT_EQ(101, tmp.objects[0].height);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, tmp.write(xml));

    TmxTemplate tmp2;
    ASSERT_EQ(E_SUCCESS, tmp2.read(xml));
    ASSERT_EQ(1, tmp2.tile_sets.size());
    ASSERT_EQ(1, tmp2.tile_sets[0].firstgid);
    ASSERT_STREQ("desert.tsx", tmp2.tile_sets[0].source.c_str());
    ASSERT_EQ(1, tmp2.objects.size());
    ASSERT_STREQ("cactus", tmp2.objects[0].name.c_str());
    ASSERT_EQ(31, tmp2.objects[0].gid);
    ASSERT_EQ(81, tmp2.objects[0].width);
    ASSERT_EQ(101, tmp2.objects[0].height);
}

