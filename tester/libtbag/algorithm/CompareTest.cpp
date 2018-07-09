/**
 * @file   CompareTest.cpp
 * @brief  Compare class tester.
 * @author zer0
 * @date   2018-07-09
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/Compare.hpp>
#include <map>

using namespace libtbag;
using namespace libtbag::algorithm;

TEST(CompareTest, CompareMap)
{
    std::map<std::string, std::string> map1, map2;
    ASSERT_TRUE(map1.insert(std::make_pair("key1", "val1")).second);
    ASSERT_TRUE(map2.insert(std::make_pair("key1", "val1")).second);
    ASSERT_TRUE(compareMap(map1, map2));

    std::map<std::string, std::string> map3, map4;
    ASSERT_TRUE(map3.insert(std::make_pair("key1", "val1")).second);
    ASSERT_TRUE(map4.insert(std::make_pair("key2", "val1")).second);
    ASSERT_FALSE(compareMap(map3, map4));

    std::map<std::string, std::string> map5, map6;
    ASSERT_TRUE(map5.insert(std::make_pair("key1", "val1")).second);
    ASSERT_TRUE(map6.insert(std::make_pair("key1", "val2")).second);
    ASSERT_FALSE(compareMap(map5, map6));
}

