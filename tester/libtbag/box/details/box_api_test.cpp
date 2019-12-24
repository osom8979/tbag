/**
 * @file   box_api_test.cpp
 * @brief  box method tester.
 * @author zer0
 * @date   2019-03-04
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_api.hpp>
#include <cstring>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(box_api_test, is_prefix_int8)
{
    ASSERT_FALSE(box_is_unknown_type(BT_INT8));
    ASSERT_FALSE(box_is_boolean_type(BT_INT8));
    ASSERT_FALSE(box_is_string_type(BT_INT8));
    ASSERT_TRUE(box_is_signed_type(BT_INT8));
    ASSERT_FALSE(box_is_unsigned_type(BT_INT8));
    ASSERT_FALSE(box_is_floating_type(BT_INT8));
    ASSERT_FALSE(box_is_complex_type(BT_INT8));
}

TEST(box_api_test, is_suffix_int8)
{
    ASSERT_FALSE(box_is_0bit_type(BT_INT8));
    ASSERT_TRUE(box_is_8bit_type(BT_INT8));
    ASSERT_FALSE(box_is_16bit_type(BT_INT8));
    ASSERT_FALSE(box_is_32bit_type(BT_INT8));
    ASSERT_FALSE(box_is_64bit_type(BT_INT8));
    ASSERT_FALSE(box_is_128bit_type(BT_INT8));
}

TEST(box_api_test, Default)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box.alloc_args(BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ASSERT_TRUE(box.exists_data());
    ASSERT_TRUE(box.exists_dims());
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(4*3*2, box.size);
    ASSERT_EQ(4*3*2*sizeof(ui32), box.total_data_byte);
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(3, box.rank);
    ASSERT_EQ(3*sizeof(ui32), box.total_dims_byte);
}

TEST(box_api_test, Resize)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box.resize_args(BT_INT32, BD_CPU, nullptr, 3, 3, 3, 3));
    ASSERT_TRUE(box.exists_data());
    ASSERT_TRUE(box.exists_dims());
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(3*3*3, box.size);
    ASSERT_EQ(3*3*3*sizeof(ui32), box.total_data_byte);
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(3, box.rank);
    ASSERT_EQ(3*sizeof(ui32), box.total_dims_byte);
    auto const * data = box.data;
    auto const * dims = box.dims;
    auto const total_data_byte = box.total_data_byte;
    auto const total_dims_byte = box.total_dims_byte;

    // Down size.
    ASSERT_EQ(E_SUCCESS, box.resize_args(BT_INT32, BD_CPU, nullptr, 2, 2, 2));
    ASSERT_TRUE(box.exists_data());
    ASSERT_TRUE(box.exists_dims());
    ASSERT_EQ(data, box.data);
    ASSERT_EQ(2*2, box.size);
    ASSERT_EQ(total_data_byte, box.total_data_byte);
    ASSERT_EQ(dims, box.dims);
    ASSERT_EQ(2, box.rank);
    ASSERT_EQ(total_dims_byte, box.total_dims_byte);

    // Up size.
    ASSERT_EQ(E_SUCCESS, box.resize_args(BT_FLOAT64, BD_CPU, nullptr, 4, 4, 4, 4, 4));
    ASSERT_TRUE(box.exists_data());
    ASSERT_TRUE(box.exists_dims());
    // ASSERT_NE(data, box.data); // Can be assigned to the same address.
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(4*4*4*4, box.size);
    ASSERT_EQ(4*4*4*4*sizeof(fp64), box.total_data_byte);
    // ASSERT_NE(dims, box.dims); // Can be assigned to the same address.
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(4, box.rank);
    ASSERT_EQ(4*sizeof(ui32), box.total_dims_byte);
}

TEST(box_api_test, SetAndGet)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box.resize_args(BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 * data = (ui32*)box.data;
    ASSERT_NE(nullptr, data);

    ui32 setter[24] = {0,};
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        setter[i] = i;
    }

    i = 0;
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 1);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 0);
    box.set_data_args(&setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 1);

    ui32 getter[24] = {0,};
    i = 0;
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 1);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 0);
    box.get_data_args(&getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 1);

    for (i = 0; i < 24; ++i) {
        ASSERT_EQ(i, setter[i]);
        ASSERT_EQ(i, getter[i]);
        ASSERT_EQ(i, data[i]);
    }
}

TEST(box_api_test, Clone)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box.resize_args(BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box.set_data(&i, BT_INT32, BD_CPU, nullptr, i);
    }

    auto err_box2 = box.clone();
    ASSERT_TRUE(err_box2);
    box_data box2 = std::move(err_box2.value);

    ASSERT_EQ(box.type, box2.type);
    ASSERT_EQ(box.device, box2.device);
    ASSERT_EQ(box.ext[0], box2.ext[0]);
    ASSERT_EQ(box.ext[1], box2.ext[1]);
    ASSERT_EQ(box.ext[2], box2.ext[2]);
    ASSERT_EQ(box.ext[3], box2.ext[3]);
    ASSERT_NE(box.data, box2.data);
    ASSERT_EQ(box.total_data_byte, box2.total_data_byte);
    ASSERT_EQ(box.size, box2.size);
    ASSERT_NE(box.dims, box2.dims);
    ASSERT_EQ(box.total_dims_byte, box2.total_dims_byte);
    ASSERT_EQ(box.rank, box2.rank);
    for (i = 0; i < 24; ++i) {
        si32 box_data;
        si32 box2_data;
        box.get_data(&box_data, BT_INT32, BD_CPU, nullptr, i);
        box2.get_data(&box2_data, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, box_data);
        ASSERT_EQ(i, box2_data);
    }
}

TEST(box_api_test, box_info)
{
    box_data box;
    ASSERT_EQ(nullptr, box.info);
    ASSERT_EQ(0, box.total_info_byte);
    ASSERT_EQ(0, box.info_size);

    char const test_info_data[] = "INFORMATION";
    auto const test_info_data_length = strlen(test_info_data)*sizeof(char);
    box.checked_assign_info_string(test_info_data);
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(test_info_data_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data, (char const *)box.info, test_info_data_length));

    char const test_info_data_long[] = "MORE_TEST_INFORMATION";
    auto const test_info_data_long_length = strlen(test_info_data_long)*sizeof(char);
    box.checked_assign_info_string(test_info_data_long);
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(test_info_data_long_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_long_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data_long, (char const *)box.info, test_info_data_long_length));

    auto const * prev_box_info = box.info;
    char const test_info_data_short[] = "INF";
    auto const test_info_data_short_length = strlen(test_info_data_short)*sizeof(char);
    box.checked_assign_info_string(test_info_data_short);
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(prev_box_info, box.info);
    ASSERT_EQ(test_info_data_long_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_short_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data_short, (char const *)box.info, test_info_data_short_length));
}

