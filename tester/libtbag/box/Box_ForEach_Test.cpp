/**
 * @file   Box_ForEach_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-29
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_ForEach_Test, Default)
{
    Box box = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
    std::vector<int> result;
    auto const code = box.forEach([&](int const & val){
        result.emplace_back(val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(8, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
    ASSERT_EQ(4, result[3]);
    ASSERT_EQ(5, result[4]);
    ASSERT_EQ(6, result[5]);
    ASSERT_EQ(7, result[6]);
    ASSERT_EQ(8, result[7]);
}

TEST(Box_ForEach_Test, Default_Mutable)
{
    Box box = {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}};
    int i = 0;
    auto const code = box.forEach([&](int & val){
        val = i;
        ++i;
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(0, box.at<int>(0, 0, 0));
    ASSERT_EQ(1, box.at<int>(0, 0, 1));
    ASSERT_EQ(2, box.at<int>(0, 1, 0));
    ASSERT_EQ(3, box.at<int>(0, 1, 1));
    ASSERT_EQ(4, box.at<int>(1, 0, 0));
    ASSERT_EQ(5, box.at<int>(1, 0, 1));
    ASSERT_EQ(6, box.at<int>(1, 1, 0));
    ASSERT_EQ(7, box.at<int>(1, 1, 1));
}

