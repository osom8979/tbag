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
    ASSERT_EQ(map1.size(), 0U);
    ASSERT_EQ(map2.size(), 1U);

    map3 = map2;
    ASSERT_EQ(map1.size(), 0U);
    ASSERT_EQ(map2.size(), 1U);
    ASSERT_EQ(map3.size(), 1U);
}

TEST(ReuseMapTest, Default)
{
    ReuseMap<std::string, int> map;

    ASSERT_EQ(map.size(), 0U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.emptyOfRemoveQueue());

    char const * const TEST1 = "TEST1";
    char const * const TEST2 = "TEST2";
    char const * const TEST3 = "TEST3";
    char const * const TEST4 = "TEST4";

    int * test1_value = map.create(TEST1);
    int * test2_value = map.create(TEST2);
    ASSERT_TRUE(test1_value != nullptr);
    ASSERT_TRUE(test2_value != nullptr);
    ASSERT_EQ(map.size(), 2U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    *test1_value = 100;
    *test2_value = 200;
    ASSERT_EQ(*map.find(TEST1), 100);
    ASSERT_EQ(*map.find(TEST2), 200);

    map.erase(TEST1);
    test1_value = nullptr;
    ASSERT_EQ(map.size(), 1U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 1U);

    int * test3_value = map.create(TEST3);
    ASSERT_TRUE(test3_value != nullptr);
    ASSERT_EQ(map.size(), 2U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    map.create(TEST1);
    ASSERT_EQ(map.size(), 3U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    ASSERT_TRUE(map.find(TEST1) != nullptr);
    ASSERT_TRUE(map.find(TEST4) == nullptr);

    map.clear();
    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.emptyOfRemoveQueue());
}

TEST(ReuseMapTest, ReusePtrMap)
{
    ReusePtrMap<int, int> map;

    int const TEST_NUMBER = 100;

    auto item = map.create(0);
    ASSERT_TRUE(item != nullptr);
    item->reset(new int(TEST_NUMBER));
    ASSERT_EQ(map.size(), 1U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    map.erase(0);
    ASSERT_EQ(map.size(), 0U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 1U);

    map.create(1);
    ASSERT_EQ(map.size(), 1U);
    ASSERT_EQ(map.sizeOfRemoveQueue(), 0U);

    auto find_item = map.find(1);
    ASSERT_TRUE(find_item != nullptr);
    ASSERT_EQ(*(find_item->get()), TEST_NUMBER);
}

