/**
 * @file   Box_Init_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2020-01-02
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Init_Test, Zeros)
{
    Box box = {1, 2, 3, 4, 5};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(5, box.dim(0));
    ASSERT_EQ(5, box.size());
    ASSERT_EQ(1, box.at<int>(0));
    ASSERT_EQ(2, box.at<int>(1));
    ASSERT_EQ(3, box.at<int>(2));
    ASSERT_EQ(4, box.at<int>(3));
    ASSERT_EQ(5, box.at<int>(4));

    ASSERT_EQ(E_SUCCESS, box.zeros());
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(5, box.dim(0));
    ASSERT_EQ(5, box.size());
    ASSERT_EQ(0, box.at<int>(0));
    ASSERT_EQ(0, box.at<int>(1));
    ASSERT_EQ(0, box.at<int>(2));
    ASSERT_EQ(0, box.at<int>(3));
    ASSERT_EQ(0, box.at<int>(4));
}

TEST(Box_Init_Test, Ones)
{
    Box box = {1, 2, 3, 4, 5};
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(5, box.dim(0));
    ASSERT_EQ(5, box.size());
    ASSERT_EQ(1, box.at<int>(0));
    ASSERT_EQ(2, box.at<int>(1));
    ASSERT_EQ(3, box.at<int>(2));
    ASSERT_EQ(4, box.at<int>(3));
    ASSERT_EQ(5, box.at<int>(4));

    ASSERT_EQ(E_SUCCESS, box.ones());
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(5, box.dim(0));
    ASSERT_EQ(5, box.size());
    ASSERT_EQ(1, box.at<int>(0));
    ASSERT_EQ(1, box.at<int>(1));
    ASSERT_EQ(1, box.at<int>(2));
    ASSERT_EQ(1, box.at<int>(3));
    ASSERT_EQ(1, box.at<int>(4));
}

TEST(Box_Init_Test, Full)
{
    auto box = Box::full<double>(200.0f, 2, 3);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_fp64());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(2, box.rank());
    ASSERT_EQ(2, box.dim(0));
    ASSERT_EQ(3, box.dim(1));
    ASSERT_EQ(6, box.size());
    ASSERT_EQ(200.0, box.at<double>(0));
    ASSERT_EQ(200.0, box.at<double>(1));
    ASSERT_EQ(200.0, box.at<double>(2));
    ASSERT_EQ(200.0, box.at<double>(3));
    ASSERT_EQ(200.0, box.at<double>(4));
    ASSERT_EQ(200.0, box.at<double>(5));
}

TEST(Box_Init_Test, Rand)
{
    auto box = Box::rand<int>(10, 20, 200);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(0, box.ext0());
    ASSERT_EQ(0, box.ext1());
    ASSERT_EQ(0, box.ext2());
    ASSERT_EQ(0, box.ext3());
    ASSERT_EQ(1, box.rank());
    ASSERT_EQ(200, box.dim(0));
    ASSERT_EQ(200, box.size());
    for (auto i = 0; i < 200; ++i) {
        ASSERT_LE(10, box.at<int>(i));
        ASSERT_GE(20, box.at<int>(i));
    }
}

