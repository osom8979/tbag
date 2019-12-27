/**
 * @file   Box_Copy_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-27
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Copy_Test, CopyToNull)
{
    char const * const TEST_INFO = "1234";

    Box src = {{1, 2}};
    src.setInfo(TEST_INFO);
    ASSERT_TRUE(src.is_si32());
    ASSERT_TRUE(src.is_device_cpu());
    ASSERT_TRUE(src.testExtensions(0, 0, 0, 0));
    ASSERT_NE(nullptr, src.data());
    ASSERT_LE(sizeof(Box::si32)*2, src.total_data_byte());
    ASSERT_EQ(2, src.size());
    ASSERT_NE(nullptr, src.dims());
    ASSERT_LE(sizeof(Box::ui32)*2, src.total_dims_byte());
    ASSERT_EQ(2, src.rank());
    ASSERT_EQ(1, src.at<Box::si32>(0, 0));
    ASSERT_EQ(2, src.at<Box::si32>(0, 1));
    ASSERT_NE(nullptr, src.info());
    ASSERT_LE(4, src.total_info_byte());
    ASSERT_EQ(4, src.info_size());
    ASSERT_STREQ(TEST_INFO, src.getInfoString().c_str());
    ASSERT_EQ(nullptr, src.getOpaquePointer());
    ASSERT_EQ(nullptr, src.getOpaqueDeleter());

    Box dest(nullptr);
    ASSERT_FALSE(dest.exists());
    ASSERT_EQ(E_SUCCESS, src.copyTo(dest));

    auto const * src_box_data = src.getBoxData();
    ASSERT_NE(nullptr, src_box_data);
    auto const * dest_box_data = dest.getBoxData();
    ASSERT_NE(nullptr, dest_box_data);
    ASSERT_NE(src_box_data, dest_box_data);

    ASSERT_TRUE(dest.is_si32());
    ASSERT_TRUE(dest.is_device_cpu());
    ASSERT_TRUE(dest.testExtensions(0, 0, 0, 0));
    ASSERT_NE(nullptr, dest.data());
    ASSERT_LE(sizeof(Box::si32)*2, dest.total_data_byte());
    ASSERT_EQ(2, dest.size());
    ASSERT_NE(nullptr, dest.dims());
    ASSERT_LE(sizeof(Box::ui32)*2, dest.total_dims_byte());
    ASSERT_EQ(2, dest.rank());
    ASSERT_EQ(1, dest.at<Box::si32>(0, 0));
    ASSERT_EQ(2, dest.at<Box::si32>(0, 1));
    ASSERT_NE(nullptr, dest.info());
    ASSERT_LE(4, dest.total_info_byte());
    ASSERT_EQ(4, dest.info_size());
    ASSERT_STREQ(TEST_INFO, dest.getInfoString().c_str());
    ASSERT_EQ(nullptr, dest.getOpaquePointer());
    ASSERT_EQ(nullptr, dest.getOpaqueDeleter());
}

TEST(Box_Copy_Test, CopyFromNull)
{
    Box src(nullptr);
    ASSERT_FALSE(src.exists());

    Box dest = {{1, 2}};
    dest.setInfo("1234");
    ASSERT_EQ(E_SUCCESS, src.copyTo(dest));

    auto const * src_box_data = src.getBoxData();
    ASSERT_EQ(nullptr, src_box_data);
    auto const * dest_box_data = dest.getBoxData();
    ASSERT_NE(nullptr, dest_box_data);

    ASSERT_TRUE(src.isEmpty());
    ASSERT_EQ(0, src.getInfoSize());

    ASSERT_TRUE(dest.empty());
    ASSERT_EQ(0, dest.info_size());
}

TEST(Box_Copy_Test, NullToNull)
{
    Box src(nullptr);
    ASSERT_FALSE(src.exists());

    Box dest(nullptr);
    ASSERT_FALSE(dest.exists());

    ASSERT_EQ(E_SUCCESS, src.copyTo(dest));
    ASSERT_FALSE(src.exists());
    ASSERT_FALSE(dest.exists());
}

TEST(Box_Copy_Test, DowngradeDataSize)
{
    Box const src = {1.0f};
    Box dest = {{2, 3}};
    ASSERT_EQ(E_SUCCESS, src.copyTo(dest));
    auto const * src_box_data = src.getBoxData();
    ASSERT_NE(nullptr, src_box_data);
    auto const * dest_box_data = dest.getBoxData();
    ASSERT_NE(nullptr, dest_box_data);
    ASSERT_NE(src_box_data, dest_box_data);

    ASSERT_TRUE(dest.is_fp32());
    ASSERT_TRUE(dest.is_device_cpu());
    ASSERT_TRUE(dest.testExtensions(0, 0, 0, 0));
    ASSERT_NE(nullptr, dest.data());
    ASSERT_LE(sizeof(Box::fp32)*1, dest.total_data_byte());
    ASSERT_EQ(1, dest.size());
    ASSERT_NE(nullptr, dest.dims());
    ASSERT_LE(sizeof(Box::fp32)*1, dest.total_dims_byte());
    ASSERT_EQ(1, dest.rank());
    ASSERT_EQ(1.0f, dest.at<Box::fp32>(0));
}

TEST(Box_Copy_Test, UpgradeDataSize)
{
    Box const src = {{2, 3}};
    Box dest = {1.0f};
    ASSERT_EQ(E_SUCCESS, src.copyTo(dest));
    auto const * src_box_data = src.getBoxData();
    ASSERT_NE(nullptr, src_box_data);
    auto const * dest_box_data = dest.getBoxData();
    ASSERT_NE(nullptr, dest_box_data);
    ASSERT_NE(src_box_data, dest_box_data);

    ASSERT_TRUE(dest.is_si32());
    ASSERT_TRUE(dest.is_device_cpu());
    ASSERT_TRUE(dest.testExtensions(0, 0, 0, 0));
    ASSERT_NE(nullptr, dest.data());
    ASSERT_LE(sizeof(Box::si32)*2, dest.total_data_byte());
    ASSERT_EQ(2, dest.size());
    ASSERT_NE(nullptr, dest.dims());
    ASSERT_LE(sizeof(Box::ui32)*2, dest.total_dims_byte());
    ASSERT_EQ(2, dest.rank());
    ASSERT_EQ(2, dest.at<Box::si32>(0, 0));
    ASSERT_EQ(3, dest.at<Box::si32>(0, 1));
}

