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

