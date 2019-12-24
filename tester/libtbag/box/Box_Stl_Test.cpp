/**
 * @file   Box_Stl_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

#include <vector>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::box;

struct Box_Stl_Test_Fixture : public testing::Test
{
    Box b1;
    Box b2;

    Box_Stl_Test_Fixture()
    {
        // EMPTY.
    }

    virtual ~Box_Stl_Test_Fixture()
    {
        // EMPTY.
    }

    void SetUp() override
    {
        b1.setInfo("1");
        b2.setInfo("2");
    }

    void TearDown() override
    {
        b1 = Box(nullptr);
        b2 = Box(nullptr);
    }
};

TEST_F(Box_Stl_Test_Fixture, Vector)
{
    std::vector<Box> container;
    container.push_back(b1);
    container.push_back(b2);

    ASSERT_EQ(2, container.size());
    ASSERT_STREQ("1", container[0].getInfoString().c_str());
    ASSERT_STREQ("2", container[1].getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2[0].getInfoString().c_str());
    ASSERT_STREQ("2", container2[1].getInfoString().c_str());

    ASSERT_EQ(container[0].ptr(), container2[0].ptr());
    ASSERT_EQ(container[1].ptr(), container2[1].ptr());
}

TEST_F(Box_Stl_Test_Fixture, Deque)
{
    std::deque<Box> container;
    container.push_back(b1);
    container.push_back(b2);

    ASSERT_EQ(2, container.size());
    ASSERT_STREQ("1", container[0].getInfoString().c_str());
    ASSERT_STREQ("2", container[1].getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2[0].getInfoString().c_str());
    ASSERT_STREQ("2", container2[1].getInfoString().c_str());

    ASSERT_EQ(container[0].ptr(), container2[0].ptr());
    ASSERT_EQ(container[1].ptr(), container2[1].ptr());
}

TEST_F(Box_Stl_Test_Fixture, Set)
{
    std::set<Box> container;
    ASSERT_TRUE(container.insert(b1).second);
    ASSERT_TRUE(container.insert(b2).second);
    ASSERT_FALSE(container.insert(b1).second);
    ASSERT_FALSE(container.insert(b2).second);

    ASSERT_EQ(2, container.size());
    ASSERT_STREQ("1", container.find(b1)->getInfoString().c_str());
    ASSERT_STREQ("2", container.find(b2)->getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2.find(b1)->getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(b2)->getInfoString().c_str());
}

TEST_F(Box_Stl_Test_Fixture, Map)
{
    std::map<int, Box> container;
    ASSERT_TRUE(container.insert(std::make_pair(1, b1)).second);
    ASSERT_TRUE(container.insert(std::make_pair(2, b2)).second);
    ASSERT_TRUE(container.insert(std::make_pair(3, b1)).second);
    ASSERT_TRUE(container.insert(std::make_pair(4, b2)).second);

    ASSERT_EQ(4, container.size());
    ASSERT_STREQ("1", container.find(1)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container.find(2)->second.getInfoString().c_str());
    ASSERT_STREQ("1", container.find(3)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container.find(4)->second.getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2.find(1)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(2)->second.getInfoString().c_str());
    ASSERT_STREQ("1", container2.find(3)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(4)->second.getInfoString().c_str());

    ASSERT_EQ(container.find(1)->second.ptr(), container2.find(1)->second.ptr());
    ASSERT_EQ(container.find(2)->second.ptr(), container2.find(2)->second.ptr());
    ASSERT_EQ(container.find(3)->second.ptr(), container2.find(3)->second.ptr());
    ASSERT_EQ(container.find(4)->second.ptr(), container2.find(4)->second.ptr());
}

TEST_F(Box_Stl_Test_Fixture, UnorderedSet)
{
    std::unordered_set<Box> container;
    ASSERT_TRUE(container.insert(b1).second);
    ASSERT_TRUE(container.insert(b2).second);
    ASSERT_FALSE(container.insert(b1).second);
    ASSERT_FALSE(container.insert(b2).second);

    ASSERT_EQ(2, container.size());
    ASSERT_STREQ("1", container.find(b1)->getInfoString().c_str());
    ASSERT_STREQ("2", container.find(b2)->getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2.find(b1)->getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(b2)->getInfoString().c_str());
}

TEST_F(Box_Stl_Test_Fixture, UnorderedMap)
{
    std::unordered_map<int, Box> container;
    ASSERT_TRUE(container.insert(std::make_pair(1, b1)).second);
    ASSERT_TRUE(container.insert(std::make_pair(2, b2)).second);
    ASSERT_TRUE(container.insert(std::make_pair(3, b1)).second);
    ASSERT_TRUE(container.insert(std::make_pair(4, b2)).second);

    ASSERT_EQ(4, container.size());
    ASSERT_STREQ("1", container.find(1)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container.find(2)->second.getInfoString().c_str());
    ASSERT_STREQ("1", container.find(3)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container.find(4)->second.getInfoString().c_str());

    auto container2 = container;
    ASSERT_STREQ("1", container2.find(1)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(2)->second.getInfoString().c_str());
    ASSERT_STREQ("1", container2.find(3)->second.getInfoString().c_str());
    ASSERT_STREQ("2", container2.find(4)->second.getInfoString().c_str());

    ASSERT_EQ(container.find(1)->second.ptr(), container2.find(1)->second.ptr());
    ASSERT_EQ(container.find(2)->second.ptr(), container2.find(2)->second.ptr());
    ASSERT_EQ(container.find(3)->second.ptr(), container2.find(3)->second.ptr());
    ASSERT_EQ(container.find(4)->second.ptr(), container2.find(4)->second.ptr());
}

