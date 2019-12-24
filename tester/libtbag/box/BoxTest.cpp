/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.ptr();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.ptr();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.ptr();
    auto const * b4_data = b4.ptr();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.ptr();
    auto const * b6_data = b6.ptr();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, GetBtype)
{
    auto const char_btype = get_btype<char>();
    auto const si8_btype = get_btype<si8>();
    ASSERT_EQ(char_btype, si8_btype);
}

TEST(BoxTest, CreateIfNotExists)
{
    Box box(nullptr);
    ASSERT_FALSE(box);
    box.createIfNotExists();
    ASSERT_TRUE(box);
}

TEST(BoxTest, StlContainerCompatibility)
{
    Box b1;
    Box b2;
    b1.setInfo("1");
    b2.setInfo("2");

    TBAG_SCENARIO("std::vector") {
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

    TBAG_SCENARIO("std::set") {
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

    TBAG_SCENARIO("std::map") {
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

    TBAG_SCENARIO("std::unordered_set") {
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

    TBAG_SCENARIO("std::unordered_map") {
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
}

