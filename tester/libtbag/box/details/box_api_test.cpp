/**
 * @file   box_api_test.cpp
 * @brief  box method tester.
 * @author zer0
 * @date   2019-03-04
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_api.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(box_api_test, Default)
{
    box_data box;
    box_clear(&box);

    ASSERT_EQ(E_SUCCESS, box_malloc_args(&box, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 3, 4, 3, 2));
    ASSERT_TRUE(box_exists_data(&box));
    ASSERT_TRUE(box_exists_dims(&box));
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(4*3*2, box.size);
    ASSERT_EQ(4*3*2*sizeof(ui32), box.total_data_byte);
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(3, box.rank);
    ASSERT_EQ(3*sizeof(ui32), box.total_dims_byte);

    box_free(&box);
    ASSERT_EQ(nullptr, box.data);
    ASSERT_EQ(0, box.size);
    ASSERT_EQ(0, box.total_data_byte);
    ASSERT_EQ(nullptr, box.dims);
    ASSERT_EQ(0, box.rank);
    ASSERT_EQ(0, box.total_dims_byte);
}

TEST(box_api_test, Resize)
{
    box_data box;
    box_clear(&box);

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 3, 3, 3, 3));
    ASSERT_TRUE(box_exists_data(&box));
    ASSERT_TRUE(box_exists_dims(&box));
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
    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 2, 2, 2));
    ASSERT_TRUE(box_exists_data(&box));
    ASSERT_TRUE(box_exists_dims(&box));
    ASSERT_EQ(data, box.data);
    ASSERT_EQ(2*2, box.size);
    ASSERT_EQ(total_data_byte, box.total_data_byte);
    ASSERT_EQ(dims, box.dims);
    ASSERT_EQ(2, box.rank);
    ASSERT_EQ(total_dims_byte, box.total_dims_byte);

    // Up size.
    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BOX_TYPE_FLOAT64, BOX_DEVICE_CPU, nullptr, 4, 4, 4, 4, 4));
    ASSERT_TRUE(box_exists_data(&box));
    ASSERT_TRUE(box_exists_dims(&box));
    // ASSERT_NE(data, box.data); // Can be assigned to the same address.
    ASSERT_NE(nullptr, box.data);
    ASSERT_EQ(4*4*4*4, box.size);
    ASSERT_EQ(4*4*4*4*sizeof(fp64), box.total_data_byte);
    // ASSERT_NE(dims, box.dims); // Can be assigned to the same address.
    ASSERT_NE(nullptr, box.dims);
    ASSERT_EQ(4, box.rank);
    ASSERT_EQ(4*sizeof(ui32), box.total_dims_byte);

    box_free(&box);
    ASSERT_EQ(nullptr, box.data);
    ASSERT_EQ(0, box.size);
    ASSERT_EQ(0, box.total_data_byte);
    ASSERT_EQ(nullptr, box.dims);
    ASSERT_EQ(0, box.rank);
    ASSERT_EQ(0, box.total_dims_byte);
}

