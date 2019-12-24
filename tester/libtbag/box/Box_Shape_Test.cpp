/**
 * @file   Box_Shape_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Shape_Test, Reshape)
{
    auto b0 = Box::shape<int>(2, 3);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(2, b0.rank());
    ASSERT_EQ(2*3, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(3, b0.dim(1));

    b0.reshape<double>(2);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_fp64());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));

    b0.reshape<double>(3, 4, 5);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_fp64());
    ASSERT_EQ(3, b0.rank());
    ASSERT_EQ(3*4*5, b0.size());
    ASSERT_EQ(3, b0.dim(0));
    ASSERT_EQ(4, b0.dim(1));
    ASSERT_EQ(5, b0.dim(2));
}

TEST(Box_Shape_Test, Stride)
{
    auto b0 = Box::shape<int>(2, 3);
    ASSERT_EQ(3, b0.getStride(0));
    ASSERT_EQ(1, b0.getStride(1));

    auto b1 = Box::shape<double>(2, 3);
    ASSERT_EQ(3, b1.getStride(0));
    ASSERT_EQ(1, b1.getStride(1));

    auto b2 = Box::shape<double>(3, 4, 5);
    ASSERT_EQ(4*5, b2.getStride(0));
    ASSERT_EQ(5, b2.getStride(1));
    ASSERT_EQ(1, b2.getStride(2));
}

TEST(Box_Shape_Test, Astype)
{
    Box b0 = {10, 20, 30, 40};
    Box b1 = b0.astype<double>();

    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(4, b0.size());
    ASSERT_EQ(4, b0.dim(0));
    ASSERT_EQ(10, b0.at<si32>(0));
    ASSERT_EQ(20, b0.at<si32>(1));

    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_fp64());
    ASSERT_EQ(1, b1.rank());
    ASSERT_EQ(4, b1.size());
    ASSERT_EQ(4, b1.dim(0));
    ASSERT_EQ(10.0, b1.at<fp64>(0));
    ASSERT_EQ(20.0, b1.at<fp64>(1));
    ASSERT_EQ(30.0, b1.at<fp64>(2));
    ASSERT_EQ(40.0, b1.at<fp64>(3));
}

