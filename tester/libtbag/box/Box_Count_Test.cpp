/**
 * @file   Box_Count_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2020-01-07
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Count_Test, Int32)
{
    Box box = { 0, 1, 2, 3, 4 };
    ASSERT_TRUE(box.any());
    ASSERT_FALSE(box.all());
    ASSERT_EQ(4, box.count());
}

TEST(Box_Count_Test, All)
{
    Box box = {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 },
    };

    ASSERT_FALSE(Box::all(box < 0));
    ASSERT_FALSE(Box::all(box < 1));
    ASSERT_FALSE(Box::all(box < 5));
    ASSERT_FALSE(Box::all(box < 9));
    ASSERT_TRUE(Box::all(box < 10));

    ASSERT_TRUE(Box::all(box > 0));
    ASSERT_FALSE(Box::all(box > 1));
    ASSERT_FALSE(Box::all(box > 5));
    ASSERT_FALSE(Box::all(box > 9));
    ASSERT_FALSE(Box::all(box > 10));
}

TEST(Box_Count_Test, Any)
{
    Box box = {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 },
    };

    ASSERT_FALSE(Box::any(box < 0));
    ASSERT_FALSE(Box::any(box < 1));
    ASSERT_TRUE(Box::any(box < 5));
    ASSERT_TRUE(Box::any(box < 9));
    ASSERT_TRUE(Box::any(box < 10));

    ASSERT_TRUE(Box::any(box > 0));
    ASSERT_TRUE(Box::any(box > 1));
    ASSERT_TRUE(Box::any(box > 5));
    ASSERT_FALSE(Box::any(box > 9));
    ASSERT_FALSE(Box::any(box > 10));
}

TEST(Box_Count_Test, Count)
{
    Box box = {
            { 1, 2, 3 },
            { 4, 5, 6 },
            { 7, 8, 9 },
    };

    ASSERT_EQ(0, Box::count(box < 0));
    ASSERT_EQ(0, Box::count(box < 1));
    ASSERT_EQ(4, Box::count(box < 5));
    ASSERT_EQ(8, Box::count(box < 9));
    ASSERT_EQ(9, Box::count(box < 10));

    ASSERT_EQ(9, Box::count(box > 0));
    ASSERT_EQ(8, Box::count(box > 1));
    ASSERT_EQ(4, Box::count(box > 5));
    ASSERT_EQ(0, Box::count(box > 9));
    ASSERT_EQ(0, Box::count(box > 10));
}

