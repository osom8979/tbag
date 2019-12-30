/**
 * @file   Box_Info_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Info_Test, String)
{
    std::string const info = "TEST";

    Box box = {{0}};
    box.setInfo(info);
    ASSERT_EQ(info.size(), box.info_capacity());
    ASSERT_EQ(info.size(), box.info_size());
    ASSERT_EQ(info, box.getInfoString());
}

TEST(Box_Info_Test, ImmutabilityOfOtherProperties)
{
    Box box = {{0, 1}, {2, 3}};

    auto const type = box.type();
    auto const device = box.device();
    auto const ext0 = box.ext0();
    auto const ext1 = box.ext1();
    auto const ext2 = box.ext2();
    auto const ext3 = box.ext3();
    auto const data = box.data();
    auto const total_data_byte = box.total_data_byte();
    auto const size = box.size();
    auto const dims = box.dims();
    auto const total_dims_byte = box.total_dims_byte();
    auto const rank = box.rank();

    ASSERT_EQ(type_si32(), type);
    ASSERT_EQ(4, size);
    ASSERT_EQ(2, rank);

    box.setInfo("TEST");
    ASSERT_EQ(type, box.type());
    ASSERT_EQ(device, box.device());
    ASSERT_EQ(ext0, box.ext0());
    ASSERT_EQ(ext1, box.ext1());
    ASSERT_EQ(ext2, box.ext2());
    ASSERT_EQ(ext3, box.ext3());
    ASSERT_EQ(data, box.data());
    ASSERT_EQ(total_data_byte, box.total_data_byte());
    ASSERT_EQ(size, box.size());
    ASSERT_EQ(dims, box.dims());
    ASSERT_EQ(total_dims_byte, box.total_dims_byte());
    ASSERT_EQ(rank, box.rank());

    box.setInfo("TEST_1234");
    ASSERT_EQ(type, box.type());
    ASSERT_EQ(device, box.device());
    ASSERT_EQ(ext0, box.ext0());
    ASSERT_EQ(ext1, box.ext1());
    ASSERT_EQ(ext2, box.ext2());
    ASSERT_EQ(ext3, box.ext3());
    ASSERT_EQ(data, box.data());
    ASSERT_EQ(total_data_byte, box.total_data_byte());
    ASSERT_EQ(size, box.size());
    ASSERT_EQ(dims, box.dims());
    ASSERT_EQ(total_dims_byte, box.total_dims_byte());
    ASSERT_EQ(rank, box.rank());
}

TEST(Box_Info_Test, Buffer)
{
    Box::Buffer const buffer = {1, 2};

    Box box;
    box.setInfo(buffer);
    ASSERT_EQ(buffer.size(), box.info_capacity());
    ASSERT_EQ(buffer.size(), box.info_size());

    auto const info_buffer = box.getInfoBuffer();
    ASSERT_EQ(buffer.size(), info_buffer.size());
    ASSERT_EQ(buffer[0], info_buffer[0]);
    ASSERT_EQ(buffer[1], info_buffer[1]);
}

TEST(Box_Info_Test, NullAssign)
{
    Box box(nullptr);
    ASSERT_FALSE(static_cast<bool>(box));

    std::string const INFO = "TEST";
    box.setInfo(INFO);

    ASSERT_TRUE(static_cast<bool>(box));
    ASSERT_EQ(INFO, box.getInfoString());
}

TEST(Box_Info_Test, Resize)
{
    Box box = {{0}};
    box.setInfo("TEST");
    auto const * info_ptr1 = box.info();
    ASSERT_NE(nullptr, info_ptr1);
    ASSERT_EQ(4, box.info_size());

    box.setInfo("T");
    auto const * info_ptr2 = box.info();
    ASSERT_NE(nullptr, info_ptr2);
    ASSERT_EQ(1, box.info_size());
    ASSERT_EQ(info_ptr1, info_ptr2);
}

TEST(Box_Info_Test, EmptyString)
{
    Box box;
    box.setInfo("");
    ASSERT_EQ(0, box.info_size());
    ASSERT_GE(box.info_capacity(), box.info_size());
}

TEST(Box_Info_Test, EmptyBuffer)
{
    Box box;
    box.setInfo(Box::Buffer());
    ASSERT_EQ(0, box.info_size());
    ASSERT_GE(box.info_capacity(), box.info_size());
}

