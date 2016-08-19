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

TEST(ReuseMapTest, Constructor)
{
    using Map = ReuseMap<int, int>;
    Map map1;
    Map map2;
    Map map3;

    map1.create(1);
    map2 = std::move(map1);
    ASSERT_EQ(0U, map1.size());
    ASSERT_EQ(1U, map2.size());

    map3 = map2;
    ASSERT_EQ(0U, map1.size());
    ASSERT_EQ(1U, map2.size());
    ASSERT_EQ(1U, map3.size());
}

TEST(ReuseMapTest, Default)
{
    ReuseMap<std::string, int> map;

    ASSERT_EQ(0U, map.size());
    ASSERT_EQ(0U, map.sizeOfRemoveQueue());

    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.emptyOfRemoveQueue());

    char const * const TEST1 = "TEST1";
    char const * const TEST2 = "TEST2";
    char const * const TEST3 = "TEST3";
    char const * const TEST4 = "TEST4";

    int * test1_value = map.create(TEST1);
    int * test2_value = map.create(TEST2);
    ASSERT_NE(nullptr, test1_value);
    ASSERT_NE(nullptr, test2_value);
    ASSERT_EQ(2U, map.size());
    ASSERT_EQ(0U, map.sizeOfRemoveQueue());

    *test1_value = 100;
    *test2_value = 200;
    ASSERT_EQ(100, *map.find(TEST1));
    ASSERT_EQ(200, *map.find(TEST2));

    map.erase(TEST1);
    test1_value = nullptr;
    ASSERT_EQ(1U, map.size());
    ASSERT_EQ(1U, map.sizeOfRemoveQueue());

    int * test3_value = map.create(TEST3);
    ASSERT_NE(nullptr, test3_value);
    ASSERT_EQ(2U, map.size());
    ASSERT_EQ(0U, map.sizeOfRemoveQueue());

    map.create(TEST1);
    ASSERT_EQ(map.size(), 3U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    ASSERT_NE(nullptr, map.find(TEST1));
    ASSERT_EQ(nullptr, map.find(TEST4));

    map.clear();
    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.emptyOfRemoveQueue());
}

TEST(ReuseMapTest, ReusePtrMap)
{
    ReusePtrMap<int, int> map;

    int const TEST_NUMBER = 100;

    auto item = map.create(0);
    ASSERT_NE(nullptr, item);
    item->reset(new int(TEST_NUMBER));
    ASSERT_EQ(1U, map.size());
    ASSERT_EQ(0U, map.sizeOfRemoveQueue());

    map.erase(0);
    ASSERT_EQ(0U, map.size());
    ASSERT_EQ(1U, map.sizeOfRemoveQueue());

    map.create(1);
    ASSERT_EQ(1U, map.size());
    ASSERT_EQ(0U, map.sizeOfRemoveQueue());

    auto find_item = map.find(1);
    ASSERT_NE(nullptr, find_item);
    ASSERT_EQ(TEST_NUMBER, *(find_item->get()));
}

