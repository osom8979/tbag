/**
 * @file   TmxWangSetTest.cpp
 * @brief  TmxWangSet class tester.
 * @author zer0
 * @date   2019-07-14
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxWangSet.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxWangSetTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxWangSet, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxWangSet, obj2)
}

TEST(TmxWangSetTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<wangset name="Path and Grass" tile="-1">
  <wangedgecolor name="Path" color="#ffa526" tile="60" probability="0.1"/>
  <wangedgecolor name="Grass" color="#00ff00" tile="80" probability="1"/>
  <wangtile tileid="50" wangid="0x2010102"/>
  <wangtile tileid="51" wangid="0x1010102"/>
  <wangtile tileid="52" wangid="0x1010202"/>
  <wangtile tileid="53" wangid="0x2010202"/>
  <wangtile tileid="59" wangid="0x2010101"/>
  <wangtile tileid="60" wangid="0x1010101"/>
  <wangtile tileid="61" wangid="0x1010201"/>
  <wangtile tileid="62" wangid="0x2010201"/>
  <wangtile tileid="68" wangid="0x2020101"/>
  <wangtile tileid="69" wangid="0x1020101"/>
  <wangtile tileid="70" wangid="0x1020201"/>
  <wangtile tileid="71" wangid="0x2020201"/>
  <wangtile tileid="77" wangid="0x2020102"/>
  <wangtile tileid="78" wangid="0x1020102"/>
  <wangtile tileid="79" wangid="0x1020202"/>
  <wangtile tileid="80" wangid="0x2020202"/>
</wangset>
)";

    TmxWangSet wang_set;
    ASSERT_EQ(E_SUCCESS, wang_set.read(TEST_XML));

    std::string xml;
    ASSERT_EQ(E_SUCCESS, wang_set.write(xml));

    TmxWangSet wang_set2;
    ASSERT_EQ(E_SUCCESS, wang_set2.read(xml));
}

