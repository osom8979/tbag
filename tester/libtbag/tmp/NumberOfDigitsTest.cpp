/**
 * @file   NumberOfDigitsTest.cpp
 * @brief  NumberOfDigits class tester.
 * @author zer0
 * @date   2018-05-30
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/NumberOfDigits.hpp>

using namespace libtbag;
using namespace libtbag::tmp;

TEST(NumberOfDigitsTest, Default)
{
    int value;
    value = DecimalNumberOfDigits<0>::value;
    ASSERT_EQ(0, value);

    value = DecimalNumberOfDigits<1>::value;
    ASSERT_EQ(1, value);

    value = DecimalNumberOfDigits<9>::value;
    ASSERT_EQ(1, value);

    value = DecimalNumberOfDigits<10>::value;
    ASSERT_EQ(2, value);

    value = DecimalNumberOfDigits<99>::value;
    ASSERT_EQ(2, value);

    value = DecimalNumberOfDigits<100>::value;
    ASSERT_EQ(3, value);
}

