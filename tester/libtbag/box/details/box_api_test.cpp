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
    box_data box1;
    box_clear(&box1);
    ASSERT_EQ(E_SUCCESS, box_malloc_args(&box1, BOX_TYPE_INT32, BOX_DEVICE_CPU, nullptr, 3, 3, 2, 1));
    box_free(&box1);
}

