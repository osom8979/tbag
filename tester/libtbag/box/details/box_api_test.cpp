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

TEST(box_api_test, SetAndGet)
{
    box_data box;
    box_clear(&box);

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 3, 4, 3, 2));
    ui32 * data = (ui32*)box.data;
    ASSERT_NE(nullptr, data);

    ui32 setter[24] = {0,};
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        setter[i] = i;
    }

    i = 0;
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 0, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 0, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 1, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 1, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 2, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 2, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 0, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 0, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 1, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 1, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 2, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 2, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 0, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 0, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 1, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 1, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 2, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 2, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 0, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 0, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 1, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 1, 1);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 2, 0);
    box_data_set_args(&box, &setter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 2, 1);

    ui32 getter[24] = {0,};
    i = 0;
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 0, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 0, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 1, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 1, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 2, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 0, 2, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 0, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 0, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 1, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 1, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 2, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 1, 2, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 0, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 0, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 1, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 1, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 2, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 2, 2, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 0, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 0, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 1, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 1, 1);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 2, 0);
    box_data_get_args(&box, &getter[i++], BOX_TYPE_UINT32, BOX_DEVICE_CPU, 3, 3, 2, 1);

    for (i = 0; i < 24; ++i) {
        ASSERT_EQ(i, setter[i]);
        ASSERT_EQ(i, getter[i]);
        ASSERT_EQ(i, data[i]);
    }

    box_free(&box);
}

TEST(box_api_test, Clone)
{
    box_data box;
    box_clear(&box);

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BOX_TYPE_UINT32, BOX_DEVICE_CPU, i);
    }

    box_data box2;
    box_clear(&box2);
    ASSERT_EQ(E_SUCCESS, box_clone(&box2, &box));

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
        ui32 box_data;
        ui32 box2_data;
        box_data_get(&box, &box_data, BOX_TYPE_UINT32, BOX_DEVICE_CPU, i);
        box_data_get(&box2, &box2_data, BOX_TYPE_UINT32, BOX_DEVICE_CPU, i);
        ASSERT_EQ(i, box_data);
        ASSERT_EQ(i, box2_data);
    }

    box_free(&box);
    box_free(&box2);
}

