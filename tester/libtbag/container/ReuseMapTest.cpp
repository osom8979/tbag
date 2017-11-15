/**
 * @file   ReuseMapTest.cpp
 * @brief  ReuseMap class tester.
 * @author zer0
 * @date   2016-08-03
 */

#include <gtest/gtest.h>
#include <libtbag/container/ReuseMap.hpp>

using namespace libtbag;
using namespace libtbag::container;

//#define ENABLE_COMPILE_ERROR_OF_SHARED_REUSE_MAP

#if defined(ENABLE_COMPILE_ERROR_OF_SHARED_REUSE_MAP)
// This is compile error:
using CompileErrorMap = ReuseMap<int, std::shared_ptr<int> >;
static CompileErrorMap g_error_value;
#endif

TEST(ReuseMapTest, Constructor)
{
    using Map = ReuseMap<int, int>;
    Map map1;
    Map map2;

    map1.insert(1, 10);
    ASSERT_EQ(1, map1.size());

    map2 = map1;
    ASSERT_EQ(1, map1.size());
    ASSERT_EQ(1, map2.size());

    *(map1.begin()->second) = 100;
    ASSERT_EQ(100, *(map1.begin()->second));
    ASSERT_EQ(100, *(map2.begin()->second));
}

TEST(ReuseMapTest, Clone)
{
    using Map = ReuseMap<int, int>;
    Map map1;
    Map map2;

    map1.insert(1, 10);
    ASSERT_EQ(1, map1.size());

    map2 = map1.clone();
    ASSERT_EQ(1, map1.size());
    ASSERT_EQ(1, map2.size());

    *(map1.begin()->second) = 20;
    ASSERT_EQ(20, *(map1.begin()->second));
    ASSERT_EQ(10, *(map2.begin()->second));
}

TEST(ReuseMapTest, MoveConstructor)
{
    using Map = ReuseMap<int, int>;
    Map map1;
    Map map2;

    map1.insert(1, 1);
    map2 = std::move(map1);
    ASSERT_EQ(0U, map1.size());
    ASSERT_EQ(1U, map2.size());
}

TEST(ReuseMapTest, Default)
{
    ReuseMap<std::string, int> map;

    ASSERT_EQ(0U, map.size());
    ASSERT_EQ(0U, map.sizeOfReady());

    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.emptyOfReady());

    char const * const TEST1 = "TEST1";
    char const * const TEST2 = "TEST2";
    char const * const TEST3 = "TEST3";
    char const * const TEST4 = "TEST4";

    ASSERT_TRUE(map.insert(TEST1, 100).second);
    ASSERT_TRUE(map.insert(TEST2, 200).second);
    ASSERT_EQ(2U, map.size());
    ASSERT_EQ(0U, map.sizeOfReady());

    ASSERT_EQ(100, *map.find(TEST1)->second);
    ASSERT_EQ(200, *map.find(TEST2)->second);

    map.erase(TEST1);
    ASSERT_EQ(1U, map.size());
    ASSERT_EQ(1U, map.sizeOfReady());

    ASSERT_TRUE(map.insert(TEST3, 300).second);
    ASSERT_EQ(2U, map.size());
    ASSERT_EQ(0U, map.sizeOfReady());

    ASSERT_TRUE(map.insert(TEST1, 101).second);
    ASSERT_EQ(3U, map.size());
    ASSERT_EQ(0U, map.sizeOfReady());

    ASSERT_FALSE(map.insert(TEST1, 102).second);
    ASSERT_EQ(3U, map.size());
    ASSERT_EQ(0U, map.sizeOfReady());

    ASSERT_NE(map.end(), map.find(TEST1));
    ASSERT_EQ(map.end(), map.find(TEST4));

    int i = 0;
    for (auto & temp : map) {
        ++i;
    }
    ASSERT_EQ(3, i);

    map.clear();
    ASSERT_TRUE(map.empty());
    ASSERT_FALSE(map.emptyOfReady());
    ASSERT_EQ(0U, map.size());
    ASSERT_EQ(3U, map.sizeOfReady());
}

TEST(ReuseMapTest, UnorderedMap)
{
    UnorderedReuseMap<int, int> map1;
    UnorderedReuseMap<int, int> map2;

    int const TEST_KEY1 = 100;
    int const TEST_KEY2 = 200;

    int const TEST_VALUE1 = 101;
    int const TEST_VALUE2 = 201;
    int const TEST_VALUE3 = 301;

    ASSERT_EQ(0, map1.size());
    ASSERT_EQ(0, map1.sizeOfReady());
    ASSERT_TRUE(map1.empty());
    ASSERT_TRUE(map1.emptyOfReady());

    ASSERT_TRUE(map1.insert(TEST_KEY1).second);
    ASSERT_TRUE(map1.insert(TEST_KEY2, TEST_VALUE1).second);
    ASSERT_EQ(2, map1.size());
    ASSERT_EQ(0, map1.sizeOfReady());
    ASSERT_FALSE(map1.empty());
    ASSERT_TRUE(map1.emptyOfReady());

    ASSERT_TRUE(map1.erase(TEST_KEY1));
    ASSERT_EQ(1, map1.size());
    ASSERT_EQ(1, map1.sizeOfReady());

    ASSERT_EQ(map1.end(), map1.find(TEST_KEY1));
    ASSERT_NE(map1.end(), map1.find(TEST_KEY2));

    map2 = map1;
    *(map1.find(TEST_KEY2)->second) = TEST_VALUE2;
    ASSERT_EQ(TEST_VALUE2, *(map2.find(TEST_KEY2)->second));
    ASSERT_EQ(1, map1.size());
    ASSERT_EQ(1, map2.size());

    map2 = map1.clone();
    *(map1.find(TEST_KEY2)->second) = TEST_VALUE3;
    ASSERT_EQ(TEST_VALUE2, *(map2.find(TEST_KEY2)->second));
    ASSERT_EQ(1, map1.size());
    ASSERT_EQ(1, map2.size());

    map1.clear();
    map2.clear();
    ASSERT_EQ(0, map1.size());
    ASSERT_EQ(0, map2.size());
}

