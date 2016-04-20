/**
 * @file   PowerTest.cpp
 * @brief  Power class tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/Power.hpp>

using namespace libtbag;
using namespace libtbag::tmp;

TEST(PowerTest, Default)
{
    int64_t test_value = 0;

    test_value = Power<7, 0>::value;
    ASSERT_EQ(test_value, 1);

    test_value = Power<7, 1>::value;
    ASSERT_EQ(test_value, 7);

    test_value = Power<7, 2>::value;
    ASSERT_EQ(test_value, 49);

    test_value = Power<7, 3>::value;
    ASSERT_EQ(test_value, 343);
}

