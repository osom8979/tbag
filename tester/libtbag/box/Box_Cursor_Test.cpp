/**
 * @file   Box_Cursor_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-27
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Cursor_Test, Default)
{
    Box box = { {0, 1, 2, 3, 4},
                {5, 6, 7, 8, 9} };

    auto c0 = box.cursor();
    ASSERT_EQ(E_SUCCESS, c0.code);

    std::vector<int> v0;
    std::vector<int> v1;

    do {
        v0.emplace_back(c0.value.at<si32>());
        auto c1 = c0.value.sub();
        ASSERT_EQ(E_SUCCESS, c1.code);
        do {
            v1.emplace_back(c1.value.at<si32>());
            auto c2 = c1.value.sub();
            ASSERT_EQ(E_OORANGE, c2.code);
        } while (c1.value.next());
    } while (c0.value.next());

    ASSERT_EQ(2, v0.size());
    ASSERT_EQ(0, v0[0]);
    ASSERT_EQ(5, v0[1]);

    ASSERT_EQ(10, v1.size());
    ASSERT_EQ(0, v1[0]);
    ASSERT_EQ(1, v1[1]);
    ASSERT_EQ(2, v1[2]);
    ASSERT_EQ(3, v1[3]);
    ASSERT_EQ(4, v1[4]);
    ASSERT_EQ(5, v1[5]);
    ASSERT_EQ(6, v1[6]);
    ASSERT_EQ(7, v1[7]);
    ASSERT_EQ(8, v1[8]);
    ASSERT_EQ(9, v1[9]);
}

TEST(Box_Cursor_Test, Sub)
{
    Box box = { {0, 1, 2},
                {3, 4, 5},
                {6, 7, 8} };

    auto c0 = box.cursor();
    ASSERT_EQ(E_SUCCESS, c0.code);

    std::vector<int> result;

//    do {
//        auto c1 = c0.value.sub();
//        ASSERT_EQ(E_SUCCESS, c1.code);
//        do {
//            result.emplace_back(c1.value.at<si32>());
//        } while (c1.value.next());
//    } while (c0.value.next());
//
//    ASSERT_EQ(10, result.size());
//    ASSERT_EQ(0, result[0]);
//    ASSERT_EQ(1, result[1]);
//    ASSERT_EQ(2, result[2]);
//    ASSERT_EQ(3, result[3]);
//    ASSERT_EQ(4, result[4]);
//    ASSERT_EQ(5, result[5]);
//    ASSERT_EQ(6, result[6]);
//    ASSERT_EQ(7, result[7]);
//    ASSERT_EQ(8, result[8]);
//    ASSERT_EQ(9, result[9]);
}

