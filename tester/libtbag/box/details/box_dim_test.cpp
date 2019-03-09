/**
 * @file   box_dim_test.cpp
 * @brief  box dim tester.
 * @author zer0
 * @date   2019-03-04
 */

#include <gtest/gtest.h>
#include <libtbag/box/details/box_dim.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::box::details;

TEST(box_dim_test, Default)
{
    auto * dims1 = box_dim_malloc(2);
    ASSERT_NE(nullptr, dims1);
    box_dim_set_args(dims1, 2, 10, 20);
    ASSERT_EQ(10, dims1[0]);
    ASSERT_EQ(20, dims1[1]);

    auto * dims2 = box_dim_malloc_args(2, 10, 20);
    ASSERT_EQ(10, dims2[0]);
    ASSERT_EQ(20, dims2[1]);
    ASSERT_TRUE(box_dim_is_equals(dims1, 2, dims2, 2));

    auto * dims3 = box_dim_clone(dims2, 2);
    ASSERT_EQ(10, dims3[0]);
    ASSERT_EQ(20, dims3[1]);
    ASSERT_TRUE(box_dim_is_equals(dims2, 2, dims3, 2));

    box_dim_free(dims1);
    box_dim_free(dims2);
    box_dim_free(dims3);
}

