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

TEST(Box_ForEach_Test, ReadOnlyVoidPtr)
{
    Box box = {{100, 200, 300, 400}};
    std::vector<int> result;
    auto const code = box.forEach([&](void const * val){
        result.emplace_back(*((int*)val));
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ(100, result[0]);
    ASSERT_EQ(200, result[1]);
    ASSERT_EQ(300, result[2]);
    ASSERT_EQ(400, result[3]);
}

TEST(Box_ForEach_Test, MutableVoidPtr)
{
    Box box = {{0, 0, 0, 0}};
    int i = 10;
    auto const code = box.forEach([&](void * val){
        *((int*)val) = i;
        ++i;
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(10, box.at<int>(0, 0));
    ASSERT_EQ(11, box.at<int>(0, 1));
    ASSERT_EQ(12, box.at<int>(0, 2));
    ASSERT_EQ(13, box.at<int>(0, 3));
}

TEST(Box_ForEach_Test, ReadOnlyRef)
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

TEST(Box_ForEach_Test, ReadOnlyDownCast)
{
    Box box = {{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}};
    std::vector<ui16> result;
    auto const code = box.forEach([&](ui16 const & val){
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

TEST(Box_ForEach_Test, ReadOnlyUpCast)
{
    Box box = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
    std::vector<double> result;
    auto const code = box.forEach([&](double const & val){
        result.emplace_back(val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(8, result.size());
    ASSERT_EQ(1.0, result[0]);
    ASSERT_EQ(2.0, result[1]);
    ASSERT_EQ(3.0, result[2]);
    ASSERT_EQ(4.0, result[3]);
    ASSERT_EQ(5.0, result[4]);
    ASSERT_EQ(6.0, result[5]);
    ASSERT_EQ(7.0, result[6]);
    ASSERT_EQ(8.0, result[7]);
}

TEST(Box_ForEach_Test, ReadOnlyPtr)
{
    Box box = {{{1, 2}, {3, 4}}};
    std::vector<int> result;
    auto const code = box.forEach([&](int const * val){
        result.emplace_back(*val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
    ASSERT_EQ(4, result[3]);
}

TEST(Box_ForEach_Test, MutableRef)
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

TEST(Box_ForEach_Test, MutablePtr)
{
    Box box = {{0, 0}};
    int i = 10;
    auto const code = box.forEach([&](int * val){
        *val = i;
        ++i;
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(10, box.at<int>(0, 0));
    ASSERT_EQ(11, box.at<int>(0, 1));
}

TEST(Box_ForEach_Test, Slice_Dim1)
{
    Box box = {0, 1, 2, 3};
    std::vector<box_slice> slices = { {1, 3, 1} };
    std::vector<int> result;
    auto const code = box.forEach(slices.data(), slices.size(), [&](int const & val){
        result.emplace_back(val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(2, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
}

TEST(Box_ForEach_Test, Slice_Dim2)
{
    using namespace libtbag::box::details;
    Box box = {{0, 1, 2, 3}, {4, 5, 6, 7}};
    std::vector<box_slice> slices = {{box_nop, box_nop, -1}, {1, 3, 1}};
    std::vector<int> result;
    auto const code = box.forEach(slices.data(), slices.size(), [&](int const & val){
        result.emplace_back(val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ(5, result[0]);
    ASSERT_EQ(6, result[1]);
    ASSERT_EQ(1, result[2]);
    ASSERT_EQ(2, result[3]);
}

TEST(Box_ForEach_Test, Slice_Dim3)
{
    using namespace libtbag::box::details;
    Box box = {
        {{ 0,  1,  2,  3}, { 4,  5,  6,  7}},
        {{ 8,  9, 10, 11}, {12, 13, 14, 15}},
        {{16, 17, 18, 19}, {20, 21, 22, 23}},
    };
    std::vector<box_slice> slices = {
        {0, box_nop, 2},
        {box_nop, box_nop, -1},
        {1, 3, 1}
    };
    std::vector<int> result;
    auto const code = box.forEach(slices.data(), slices.size(), [&](int const & val){
        result.emplace_back(val);
    });
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(8, result.size());
    ASSERT_EQ(5, result[0]);
    ASSERT_EQ(6, result[1]);
    ASSERT_EQ(1, result[2]);
    ASSERT_EQ(2, result[3]);
    ASSERT_EQ(21, result[4]);
    ASSERT_EQ(22, result[5]);
    ASSERT_EQ(17, result[6]);
    ASSERT_EQ(18, result[7]);
}

