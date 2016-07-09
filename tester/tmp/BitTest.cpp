/**
 * @file   BitTest.cpp
 * @brief  Bit class tester.
 * @author zer0
 * @date   2016-07-09
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/Bit.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::tmp;

TEST(BitTest, Default)
{
    uint8_t max_8bit_unsigned = 0b10000000;
    int8_t  max_8bit_signed   = 0b10000000;

    ASSERT_EQ(BitFlag<uint8_t>::max(), max_8bit_unsigned);
    ASSERT_EQ(BitFlag<int8_t>::max(), max_8bit_signed);
}

