/**
 * @file   NumberTest.cpp
 * @brief  Number class tester.
 * @author zer0
 * @date   2018-03-24
 */

#include <gtest/gtest.h>
#include <libtbag/math/Number.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(NumberTest, Division)
{
    ASSERT_EQ( 5, TBAG_INT_DIVISION_IMPL(10, 2));
    ASSERT_EQ(10, TBAG_INT_DIVISION_IMPL(20, 2));
    ASSERT_EQ( 5, TBAG_INT_DIVISION_IMPL(20, 4));
    ASSERT_EQ(20, TBAG_INT_DIVISION_IMPL(40, 2));
    ASSERT_EQ(10, TBAG_INT_DIVISION_IMPL(40, 4));
    ASSERT_EQ( 5, TBAG_INT_DIVISION_IMPL(40, 8));

    ASSERT_EQ( 5, TBAG_INT_DIVISION(10, 1));
    ASSERT_EQ(10, TBAG_INT_DIVISION(20, 1));
    ASSERT_EQ( 5, TBAG_INT_DIVISION(20, 2));
    ASSERT_EQ(20, TBAG_INT_DIVISION(40, 1));
    ASSERT_EQ(10, TBAG_INT_DIVISION(40, 2));
    ASSERT_EQ( 5, TBAG_INT_DIVISION(40, 3));
}

TEST(NumberTest, Modulo)
{
    ASSERT_EQ(1, TBAG_INT_MODULO( 9, 8));
    ASSERT_EQ(1, TBAG_INT_MODULO( 9, 4));
    ASSERT_EQ(1, TBAG_INT_MODULO( 9, 2));

    ASSERT_EQ(2, TBAG_INT_MODULO(10, 8));
    ASSERT_EQ(2, TBAG_INT_MODULO(10, 4));
    ASSERT_EQ(0, TBAG_INT_MODULO(10, 2));

    ASSERT_EQ(3, TBAG_INT_MODULO(11, 8));
    ASSERT_EQ(3, TBAG_INT_MODULO(11, 4));
    ASSERT_EQ(1, TBAG_INT_MODULO(11, 2));

    ASSERT_EQ(4, TBAG_INT_MODULO(12, 8));
    ASSERT_EQ(0, TBAG_INT_MODULO(12, 4));
    ASSERT_EQ(0, TBAG_INT_MODULO(12, 2));
}

