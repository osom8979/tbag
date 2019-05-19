/**
 * @file   box_common_test.cpp
 * @brief  box common tester.
 * @author zer0
 * @date   2019-03-04
 * @date   2019-05-15 (Rename: box_dim_test -> box_common_test)
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_common.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(box_common_test, Default)
{
    int const HEIGHT = 10;
    int const WIDTH = 20;

    auto * dims1 = box_dim_malloc(2);
    ASSERT_NE(nullptr, dims1);
    box_dim_set_args(dims1, 2, HEIGHT, WIDTH);
    ASSERT_EQ(HEIGHT, dims1[0]);
    ASSERT_EQ(WIDTH, dims1[1]);

    auto * dims2 = box_dim_malloc_args(2, HEIGHT, WIDTH);
    ASSERT_EQ(HEIGHT, dims2[0]);
    ASSERT_EQ(WIDTH, dims2[1]);
    ASSERT_TRUE(box_dim_is_equals(dims1, 2, dims2, 2));

    auto * dims3 = box_dim_clone(dims2, 2);
    ASSERT_EQ(HEIGHT, dims3[0]);
    ASSERT_EQ(WIDTH, dims3[1]);
    ASSERT_TRUE(box_dim_is_equals(dims2, 2, dims3, 2));

    box_dim_free(dims1);
    box_dim_free(dims2);
    box_dim_free(dims3);
}

TEST(box_common_test, box_dim_get_index_args)
{
    int const DEPTH = 1000;
    int const HEIGHT = 100;
    int const WIDTH = 10;

    auto * dims1 = box_dim_malloc_args(2, HEIGHT, WIDTH);
    ASSERT_NE(nullptr, dims1);
    auto index1 = box_dim_get_index_args(dims1, 2, 1, 2);
    ASSERT_EQ((1*WIDTH)+2, index1);
    box_dim_free(dims1);

    auto * dims2 = box_dim_malloc_args(3, DEPTH, HEIGHT, WIDTH);
    ASSERT_NE(nullptr, dims1);
    auto index2 = box_dim_get_index_args(dims2, 3, 3, 2, 1);
    ASSERT_EQ((3*HEIGHT*WIDTH)+(2*WIDTH)+1, index2);
    box_dim_free(dims2);
}

TEST(box_common_test, opaque)
{
    box_data box;

    si8  set_value1 = 1;
    si16 set_value2 = 2;
    si32 set_value3 = 3;
    si64 set_value4 = 4;
    ui8  set_value5 = 5;
    ui16 set_value6 = 6;
    ui32 set_value7 = 7;
    ui64 set_value8 = 8;

    si8  get_value1 = 0;
    si16 get_value2 = 0;
    si32 get_value3 = 0;
    si64 get_value4 = 0;
    ui8  get_value5 = 0;
    ui16 get_value6 = 0;
    ui32 get_value7 = 0;
    ui64 get_value8 = 0;

    box_opaque_set(&box, set_value1);
    box_opaque_get(&box, &get_value1);

    box_opaque_set(&box, set_value2);
    box_opaque_get(&box, &get_value2);

    box_opaque_set(&box, set_value3);
    box_opaque_get(&box, &get_value3);

    box_opaque_set(&box, set_value4);
    box_opaque_get(&box, &get_value4);

    box_opaque_set(&box, set_value5);
    box_opaque_get(&box, &get_value5);

    box_opaque_set(&box, set_value6);
    box_opaque_get(&box, &get_value6);

    box_opaque_set(&box, set_value7);
    box_opaque_get(&box, &get_value7);

    box_opaque_set(&box, set_value8);
    box_opaque_get(&box, &get_value8);

    ASSERT_EQ(set_value1, set_value1);
    ASSERT_EQ(set_value2, set_value2);
    ASSERT_EQ(set_value3, set_value3);
    ASSERT_EQ(set_value4, set_value4);
    ASSERT_EQ(set_value5, set_value5);
    ASSERT_EQ(set_value6, set_value6);
    ASSERT_EQ(set_value7, set_value7);
    ASSERT_EQ(set_value8, set_value8);
}

TEST(box_common_test, box_dim_get_stride)
{
    ui32 dims1[2] = {10, 5};
    ASSERT_EQ(5, box_dim_get_stride(dims1, 2, 0));
    ASSERT_EQ(1, box_dim_get_stride(dims1, 2, 1));

    ui32 dims2[3] = {10, 5, 3};
    ASSERT_EQ(5*3, box_dim_get_stride(dims2, 3, 0));
    ASSERT_EQ(3, box_dim_get_stride(dims2, 3, 1));
    ASSERT_EQ(1, box_dim_get_stride(dims2, 3, 2));

    ui32 dims3[4] = {15, 8, 5, 3};
    ASSERT_EQ(8*5*3, box_dim_get_stride(dims3, 4, 0));
    ASSERT_EQ(5*3, box_dim_get_stride(dims3, 4, 1));
    ASSERT_EQ(3, box_dim_get_stride(dims3, 4, 2));
    ASSERT_EQ(1, box_dim_get_stride(dims3, 4, 3));
}

TEST(box_common_test, box_index_abs)
{
    ASSERT_EQ(0, box_index_abs(10, 0));
    ASSERT_EQ(1, box_index_abs(10, 1));
    ASSERT_EQ(5, box_index_abs(10, 5));
    ASSERT_EQ(9, box_index_abs(10, -1));
    ASSERT_EQ(8, box_index_abs(10, -2));
    ASSERT_EQ(0, box_index_abs(10, -10));
}

