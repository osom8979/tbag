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

TEST(box_api_test, Default)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_malloc_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
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

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 3, 3, 3));
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
    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 2, 2, 2));
    ASSERT_TRUE(box_exists_data(&box));
    ASSERT_TRUE(box_exists_dims(&box));
    ASSERT_EQ(data, box.data);
    ASSERT_EQ(2*2, box.size);
    ASSERT_EQ(total_data_byte, box.total_data_byte);
    ASSERT_EQ(dims, box.dims);
    ASSERT_EQ(2, box.rank);
    ASSERT_EQ(total_dims_byte, box.total_dims_byte);

    // Up size.
    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_FLOAT64, BD_CPU, nullptr, 4, 4, 4, 4, 4));
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

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 * data = (ui32*)box.data;
    ASSERT_NE(nullptr, data);

    ui32 setter[24] = {0,};
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        setter[i] = i;
    }

    i = 0;
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 1);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 0);
    box_data_set_args(&box, &setter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 1);

    ui32 getter[24] = {0,};
    i = 0;
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 0, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 1, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 0, 2, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 0, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 1, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 1, 2, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 0, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 1, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 2, 2, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 0, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 1, 1);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 0);
    box_data_get_args(&box, &getter[i++], BT_INT32, BD_CPU, nullptr, 3, 3, 2, 1);

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

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
    }

    box_data box2;
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
        si32 box_data;
        si32 box2_data;
        box_data_get(&box, &box_data, BT_INT32, BD_CPU, nullptr, i);
        box_data_get(&box2, &box2_data, BT_INT32, BD_CPU, nullptr, i);
        ASSERT_EQ(i, box_data);
        ASSERT_EQ(i, box2_data);
    }

    box_free(&box);
    box_free(&box2);
}

TEST(box_api_test, box_cursor_01)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 1, 10));
    for (ui32 i = 0; i < 10; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box));
        ASSERT_EQ(0, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(1, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(2, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(3, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(4, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(6, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(7, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(8, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(9, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 0, -1));
        ASSERT_EQ(0, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(1, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(2, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(3, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(4, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(6, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(7, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(8, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, -2, -1));
        ASSERT_EQ(8, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, -8, 0, -1));
        ASSERT_EQ(2, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(1, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 5));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(6, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(7, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(8, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(9, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 5, 8));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(6, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(7, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 5, 8, 2));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(7, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 5, 8, 3));
        ASSERT_EQ(5, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 1, 4, 10));
        ASSERT_EQ(1, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_FALSE(box_cursor_init(&cursor, &box, 0u, 1, 4, -1));
    }

    {
        box_cursor cursor;
        ASSERT_FALSE(box_cursor_init(&cursor, &box, 0u, 4, 1));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 4, 1, -1));
        ASSERT_EQ(4, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(3, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(2, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 4, 1, -2));
        ASSERT_EQ(4, *(si32*)(cursor.begin));
        ASSERT_TRUE(box_cursor_next(&cursor));
        ASSERT_EQ(2, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    {
        box_cursor cursor;
        ASSERT_TRUE(box_cursor_init(&cursor, &box, 0u, 4, 1, -10));
        ASSERT_EQ(4, *(si32*)(cursor.begin));
        ASSERT_FALSE(box_cursor_next(&cursor));
    }

    box_free(&box);
}

TEST(box_api_test, box_cursor_02)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 2, 3, 3));
    for (ui32 i = 0; i < 3*3; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor0, &box));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(0, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(1, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(2, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(3, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(4, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(5, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(6, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(7, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(8, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor0, &box, 0u, 0, -1));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(0, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(1, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(2, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(3, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(4, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(5, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor0, &box, 0u, 1, 2));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(3, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(4, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(5, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor0, &box, 0u, 0, 3, 2));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(0, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(1, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(2, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_EQ(6, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(7, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(8, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor0, &box, 0u, 0, 3, 2));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1, 1));
        ASSERT_EQ(1, *(si32*)(cursor1.begin));
        ASSERT_TRUE(box_cursor_next(&cursor1));
        ASSERT_EQ(2, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1, 1, -1));
        ASSERT_EQ(7, *(si32*)(cursor1.begin));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    box_free(&box);
}

TEST(box_api_test, box_cursor_03)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 3, 2, 2));
    for (ui32 i = 0; i < 3*2*2; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
    }

    {
        box_cursor cursor0;
        box_cursor cursor1;
        box_cursor cursor2;

        ASSERT_TRUE(box_cursor_init(&cursor0, &box));
        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));

        ASSERT_EQ(0, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(1, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_TRUE(box_cursor_next(&cursor1));

        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));
        ASSERT_EQ(2, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(3, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));

        ASSERT_EQ(4, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(5, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_TRUE(box_cursor_next(&cursor1));

        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));
        ASSERT_EQ(6, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(7, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_TRUE(box_cursor_next(&cursor0));

        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));

        ASSERT_EQ(8, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(9, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_TRUE(box_cursor_next(&cursor1));

        ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));
        ASSERT_EQ(10, *(si32*)(cursor2.begin));
        ASSERT_TRUE(box_cursor_next(&cursor2));
        ASSERT_EQ(11, *(si32*)(cursor2.begin));
        ASSERT_FALSE(box_cursor_next(&cursor2));
        ASSERT_FALSE(box_cursor_next(&cursor1));
        ASSERT_FALSE(box_cursor_next(&cursor0));
    }

    box_free(&box);
}

TEST(box_api_test, box_cursor_04)
{
    box_data box;

    ASSERT_EQ(E_SUCCESS, box_resize_args(&box, BT_INT32, BD_CPU, nullptr, 3, 4, 3, 2));
    ui32 i = 0;
    for (i = 0; i < 24; ++i) {
        box_data_set(&box, &i, BT_INT32, BD_CPU, nullptr, i);
    }

    i = 0;

    int loop_counter0 = 0;
    box_cursor cursor0;
    ASSERT_TRUE(box_cursor_init(&cursor0, &box));
    do {
        int loop_counter1 = 0;
        box_cursor cursor1;
        ASSERT_TRUE(box_cursor_init(&cursor1, &cursor0, &box, 1));
        do {
            int loop_counter2 = 0;
            box_cursor cursor2;
            ASSERT_TRUE(box_cursor_init(&cursor2, &cursor1, &box, 2));
            do {
                ASSERT_EQ(i, *(si32*)(cursor2.begin));
                ++i;
                ++loop_counter2;
            } while (box_cursor_next(&cursor2));
            ASSERT_EQ(2, loop_counter2);
            ++loop_counter1;
        } while (box_cursor_next(&cursor1));
        ASSERT_EQ(3, loop_counter1);
        ++loop_counter0;
    } while (box_cursor_next(&cursor0));
    ASSERT_EQ(4, loop_counter0);
    ASSERT_EQ(24, i);

    box_free(&box);
}

TEST(box_api_test, box_info)
{
    box_data box;
    ASSERT_EQ(nullptr, box.info);
    ASSERT_EQ(0, box.total_info_byte);
    ASSERT_EQ(0, box.info_size);

    char const test_info_data[] = "INFORMATION";
    auto const test_info_data_length = strlen(test_info_data)*sizeof(char);
    ASSERT_TRUE(box_info_checked_assign_string(&box, test_info_data));
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(test_info_data_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data, (char const *)box.info, test_info_data_length));

    char const test_info_data_long[] = "MORE_TEST_INFORMATION";
    auto const test_info_data_long_length = strlen(test_info_data_long)*sizeof(char);
    ASSERT_TRUE(box_info_checked_assign_string(&box, test_info_data_long));
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(test_info_data_long_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_long_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data_long, (char const *)box.info, test_info_data_long_length));

    auto const * prev_box_info = box.info;
    char const test_info_data_short[] = "INF";
    auto const test_info_data_short_length = strlen(test_info_data_short)*sizeof(char);
    ASSERT_TRUE(box_info_checked_assign_string(&box, test_info_data_short));
    ASSERT_NE(nullptr, box.info);
    ASSERT_EQ(prev_box_info, box.info);
    ASSERT_EQ(test_info_data_long_length, box.total_info_byte);
    ASSERT_EQ(test_info_data_short_length, box.info_size);
    ASSERT_EQ(0, strncmp(test_info_data_short, (char const *)box.info, test_info_data_short_length));

    box_free(&box);
}

