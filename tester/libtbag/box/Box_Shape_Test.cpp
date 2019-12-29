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

TEST(Box_Shape_Test, ResizeArgs1)
{
    Box box;
    ui64 ext[] = {0, 1, 2, 3};
    ASSERT_EQ(E_SUCCESS, box.resize<int>(Box::device_cpu(), ext, 2, 3));
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(1, box.ext1());
    ASSERT_EQ(2, box.ext2());
    ASSERT_EQ(3, box.ext3());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));
}

TEST(Box_Shape_Test, ResizeArgs2)
{
    auto box = Box::array<int>(2, 3);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(2, box.rank());
    ASSERT_EQ(2*3, box.size());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));

    box.resize<double>(2);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_fp64());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(2, box.size());
    ASSERT_EQ(2, box.dim(0));

    box.resize<double>(3, 4, 5);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_fp64());
    ASSERT_EQ(3, box.rank());
    ASSERT_EQ(3*4*5, box.size());
    ASSERT_EQ(3, box.dim(0));
    ASSERT_EQ(4, box.dim(1));
    ASSERT_EQ(5, box.dim(2));
}

TEST(Box_Shape_Test, ResizeDims1)
{
    Box box;
    ui64 ext[] = {0, 1, 2, 3};
    ui32 dims[] = {2, 3};
    ASSERT_EQ(E_SUCCESS, box.resize<int>(Box::device_cpu(), ext, 2, dims));
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(1, box.ext1());
    ASSERT_EQ(2, box.ext2());
    ASSERT_EQ(3, box.ext3());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));
}

TEST(Box_Shape_Test, ResizeDims2)
{
    Box box;
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());

    ui32 dims[] = {2, 3};
    ASSERT_EQ(E_SUCCESS, box.resize<int>(2, dims));
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));
}

TEST(Box_Shape_Test, ResizeBox1)
{
    Box box1 = {{0, 1, 3}, {4, 5, 6}};
    ASSERT_TRUE(box1.is_device_cpu());
    ASSERT_TRUE(box1.is_si32());
    ASSERT_EQ(0, box1.ext0());
    ASSERT_EQ(0, box1.ext1());
    ASSERT_EQ(0, box1.ext2());
    ASSERT_EQ(0, box1.ext3());
    ASSERT_EQ(2, box1.dim(0));
    ASSERT_EQ(3, box1.dim(1));

    Box box2;
    ASSERT_EQ(E_SUCCESS, box2.resize<int>(box1.base()));
    ASSERT_TRUE(box2.is_device_cpu());
    ASSERT_TRUE(box2.is_si32());
    ASSERT_EQ(0, box2.ext0());
    ASSERT_EQ(0, box2.ext1());
    ASSERT_EQ(0, box2.ext2());
    ASSERT_EQ(0, box2.ext3());
    ASSERT_EQ(2, box2.dim(0));
    ASSERT_EQ(3, box2.dim(1));
}

TEST(Box_Shape_Test, ResizeBox2)
{
    Box box1 = {{0, 1, 3}, {4, 5, 6}};
    ASSERT_TRUE(box1.is_device_cpu());
    ASSERT_TRUE(box1.is_si32());
    ASSERT_EQ(0, box1.ext0());
    ASSERT_EQ(0, box1.ext1());
    ASSERT_EQ(0, box1.ext2());
    ASSERT_EQ(0, box1.ext3());
    ASSERT_EQ(2, box1.dim(0));
    ASSERT_EQ(3, box1.dim(1));

    Box box2;
    ASSERT_EQ(E_SUCCESS, box2.resize<int>(box1));
    ASSERT_TRUE(box2.is_device_cpu());
    ASSERT_TRUE(box2.is_si32());
    ASSERT_EQ(0, box2.ext0());
    ASSERT_EQ(0, box2.ext1());
    ASSERT_EQ(0, box2.ext2());
    ASSERT_EQ(0, box2.ext3());
    ASSERT_EQ(2, box2.dim(0));
    ASSERT_EQ(3, box2.dim(1));
}

TEST(Box_Shape_Test, Stride)
{
    auto b0 = Box::array<int>(2, 3);
    ASSERT_EQ(3, b0.getStride(0));
    ASSERT_EQ(1, b0.getStride(1));

    auto b1 = Box::array<double>(2, 3);
    ASSERT_EQ(3, b1.getStride(0));
    ASSERT_EQ(1, b1.getStride(1));

    auto b2 = Box::array<double>(3, 4, 5);
    ASSERT_EQ(4*5, b2.getStride(0));
    ASSERT_EQ(5, b2.getStride(1));
    ASSERT_EQ(1, b2.getStride(2));
}

TEST(Box_Shape_Test, Astype)
{
    auto const INFO_TEXT = "TEST";
    Box b0 = {{10, 20, 30, 40}};
    b0.setInfo(INFO_TEXT);
    b0.setOpaque(9999);
    Box const b1 = b0.astype<double>();

    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(0, b0.ext0());
    ASSERT_EQ(0, b0.ext1());
    ASSERT_EQ(0, b0.ext2());
    ASSERT_EQ(0, b0.ext3());
    ASSERT_EQ(2, b0.rank());
    ASSERT_EQ(4, b0.size());
    ASSERT_EQ(1, b0.dim(0));
    ASSERT_EQ(4, b0.dim(1));
    ASSERT_EQ(10, b0.at<si32>(0, 0));
    ASSERT_EQ(20, b0.at<si32>(0, 1));
    ASSERT_EQ(30, b0.at<si32>(0, 2));
    ASSERT_EQ(40, b0.at<si32>(0, 3));
    ASSERT_STREQ(INFO_TEXT, b0.getInfoString().c_str());
    ASSERT_EQ(9999, b0.getOpaque<int>());

    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_fp64());
    ASSERT_EQ(0, b1.ext0());
    ASSERT_EQ(0, b1.ext1());
    ASSERT_EQ(0, b1.ext2());
    ASSERT_EQ(0, b1.ext3());
    ASSERT_EQ(2, b1.rank());
    ASSERT_EQ(4, b1.size());
    ASSERT_EQ(1, b1.dim(0));
    ASSERT_EQ(4, b1.dim(1));
    ASSERT_EQ(10.0, b1.at<fp64>(0, 0));
    ASSERT_EQ(20.0, b1.at<fp64>(0, 1));
    ASSERT_EQ(30.0, b1.at<fp64>(0, 2));
    ASSERT_EQ(40.0, b1.at<fp64>(0, 3));
    ASSERT_STREQ(INFO_TEXT, b1.getInfoString().c_str());
    ASSERT_EQ(9999, b1.getOpaque<int>());
}

