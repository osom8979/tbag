/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.base();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.base();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.base();
    auto const * b4_data = b4.base();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.base();
    auto const * b6_data = b6.base();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, GetBtype)
{
    auto const char_btype = get_btype<char>();
    auto const si8_btype = get_btype<si8>();
    ASSERT_EQ(char_btype, si8_btype);
}

TEST(BoxTest, Diffs0)
{
    Box box;
    ASSERT_TRUE(box);

    auto const diffs = box.diffs();
    ASSERT_EQ(0, diffs.size());
}

TEST(BoxTest, Diffs1)
{
    auto box = Box::array<int>(3, 3);
    ASSERT_TRUE(box);

    auto const diffs = box.diffs({{nop, nop, -1}, {0, -1, 1}});
    ASSERT_EQ(2, diffs.size());
    ASSERT_EQ(3, diffs[0]);
    ASSERT_EQ(2, diffs[1]);
}

TEST(BoxTest, Diffs2)
{
    auto box = Box::array<int>(5, 5);
    ASSERT_TRUE(box);

    auto const diffs = box.diffs({{1, 4, 1}, {2, 4, 1}});
    ASSERT_EQ(2, diffs.size());
    ASSERT_EQ(3, diffs[0]);
    ASSERT_EQ(2, diffs[1]);
}

TEST(BoxTest, Diffs3)
{
    auto box = Box::array<int>(5, 5, 3);
    ASSERT_TRUE(box);

    auto const diffs = box.diffs({{1, 4, 1}, {1, 3, 2}});
    ASSERT_EQ(3, diffs.size());
    ASSERT_EQ(3, diffs[0]);
    ASSERT_EQ(1, diffs[1]);
    ASSERT_EQ(3, diffs[2]);
}

TEST(BoxTest, Diffs4)
{
    auto box = Box::array<int>(5, 5, 3, 1);
    ASSERT_TRUE(box);

    auto const diffs = box.diffs();
    ASSERT_EQ(4, diffs.size());
    ASSERT_EQ(5, diffs[0]);
    ASSERT_EQ(5, diffs[1]);
    ASSERT_EQ(3, diffs[2]);
    ASSERT_EQ(1, diffs[3]);
}

