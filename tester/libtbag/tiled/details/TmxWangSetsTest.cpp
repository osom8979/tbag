/**
 * @file   TmxWangSetsTest.cpp
 * @brief  TmxWangSets class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxWangSets.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxWangSetsTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxWangSets, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxWangSets, obj2)
}

TEST(TmxWangSetsTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<wangsets>
  <wangset name="WalkwaysAndPaths" tile="39">
    <wangtile tileid="80" wangid="0x2020202"/>
  </wangset>
  <wangset name="Path and Grass" tile="-1">
    <wangtile tileid="50" wangid="0x2010102"/>
  </wangset>
</wangsets>
)";

    TmxWangSets wang_sets;
    ASSERT_EQ(E_SUCCESS, wang_sets.read(TEST_XML));
    ASSERT_EQ(2, wang_sets.wang_sets.size());
    ASSERT_STREQ("WalkwaysAndPaths", wang_sets.wang_sets[0].name.c_str());
    ASSERT_EQ(39, wang_sets.wang_sets[0].tile);
    ASSERT_EQ(1, wang_sets.wang_sets[0].wang_tiles.size());
    ASSERT_STREQ("Path and Grass", wang_sets.wang_sets[1].name.c_str());
    ASSERT_EQ(-1, wang_sets.wang_sets[1].tile);
    ASSERT_EQ(1, wang_sets.wang_sets[1].wang_tiles.size());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_sets.write(xml));

    TmxWangSets wang_sets2;
    ASSERT_EQ(E_SUCCESS, wang_sets2.read(xml));
    ASSERT_EQ(2, wang_sets2.wang_sets.size());
    ASSERT_STREQ("WalkwaysAndPaths", wang_sets2.wang_sets[0].name.c_str());
    ASSERT_EQ(39, wang_sets2.wang_sets[0].tile);
    ASSERT_EQ(1, wang_sets2.wang_sets[0].wang_tiles.size());
    ASSERT_STREQ("Path and Grass", wang_sets2.wang_sets[1].name.c_str());
    ASSERT_EQ(-1, wang_sets2.wang_sets[1].tile);
    ASSERT_EQ(1, wang_sets2.wang_sets[1].wang_tiles.size());
}

