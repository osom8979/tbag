/**
 * @file   Box_Assign_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Assign_Test, Dim1)
{
    Box box = {10, 20};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(2, box.size());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(10, box.at<si32>(0));
    ASSERT_EQ(20, box.at<si32>(1));
}

TEST(Box_Assign_Test, Dim2)
{
    Box box = {{10.0f, 20.0f}, {30.0f, 40.0f}, {50.0f, 60.0f}};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_fp32());
    ASSERT_EQ(2, box.rank());
    ASSERT_EQ(6, box.size());
    ASSERT_EQ(3, box.dim(0));
    ASSERT_EQ(2, box.dim(1));
    ASSERT_EQ(10.0f, box.at<fp32>(0, 0));
    ASSERT_EQ(20.0f, box.at<fp32>(0, 1));
    ASSERT_EQ(30.0f, box.at<fp32>(1, 0));
    ASSERT_EQ(40.0f, box.at<fp32>(1, 1));
    ASSERT_EQ(50.0f, box.at<fp32>(2, 0));
    ASSERT_EQ(60.0f, box.at<fp32>(2, 1));
}

TEST(Box_Assign_Test, Dim3)
{
    Box box = {{{10.0},{20.0}},{{30.0},{40.0}},{{50.0},{60.0}}};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_fp64());
    ASSERT_EQ(3, box.rank());
    ASSERT_EQ(6, box.size());
    ASSERT_EQ(3, box.dim(0));
    ASSERT_EQ(2, box.dim(1));
    ASSERT_EQ(1, box.dim(2));
    ASSERT_EQ(10.0f, box.at<fp64>(0, 0, 0));
    ASSERT_EQ(20.0f, box.at<fp64>(0, 1, 0));
    ASSERT_EQ(30.0f, box.at<fp64>(1, 0, 0));
    ASSERT_EQ(40.0f, box.at<fp64>(1, 1, 0));
    ASSERT_EQ(50.0f, box.at<fp64>(2, 0, 0));
    ASSERT_EQ(60.0f, box.at<fp64>(2, 1, 0));
}

TEST(Box_Assign_Test, Dim4)
{
    Box box = {{{{10},{20}},{{30},{40}},{{50},{60}}},
               {{{11},{21}},{{31},{41}},{{51},{61}}}};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(4, box.rank());
    ASSERT_EQ(12, box.size());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));
    ASSERT_EQ(2, box.dim(2));
    ASSERT_EQ(1, box.dim(3));
    ASSERT_EQ(10, box.at<si32>(0, 0, 0, 0));
    ASSERT_EQ(20, box.at<si32>(0, 0, 1, 0));
    ASSERT_EQ(30, box.at<si32>(0, 1, 0, 0));
    ASSERT_EQ(40, box.at<si32>(0, 1, 1, 0));
    ASSERT_EQ(50, box.at<si32>(0, 2, 0, 0));
    ASSERT_EQ(60, box.at<si32>(0, 2, 1, 0));
    ASSERT_EQ(11, box.at<si32>(1, 0, 0, 0));
    ASSERT_EQ(21, box.at<si32>(1, 0, 1, 0));
    ASSERT_EQ(31, box.at<si32>(1, 1, 0, 0));
    ASSERT_EQ(41, box.at<si32>(1, 1, 1, 0));
    ASSERT_EQ(51, box.at<si32>(1, 2, 0, 0));
    ASSERT_EQ(61, box.at<si32>(1, 2, 1, 0));
}

TEST(Box_Assign_Test, Dim2_SingleElement)
{
    Box b4 = {{10}};
    ASSERT_TRUE(b4.is_device_cpu());
    ASSERT_TRUE(b4.is_si32());
    ASSERT_EQ(2, b4.rank());
    ASSERT_EQ(1, b4.size());
    ASSERT_EQ(1, b4.dim(0));
    ASSERT_EQ(1, b4.dim(1));
}

