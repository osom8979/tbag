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

TEST(BitTest, max)
{
    uint8_t max_8bit_unsigned = 0b10000000;
    int8_t  max_8bit_signed   = 0b10000000;

    ASSERT_EQ(BitFlag<uint8_t>::max(), max_8bit_unsigned);
    ASSERT_EQ(BitFlag<int8_t>::max(), max_8bit_signed);
}

TEST(BitTest, findMax_by_signed)
{
    int8_t test1 = 0b01000001;
    int8_t test2 = 0b11000000;
    int8_t test3 = 0b00000001;
    int8_t test4 = 0b10000000;
    int8_t test5 = 0b00001001;
    int8_t test6 = 0b00000000;

    int8_t test1_result = 0b01000000;
    int8_t test2_result = 0b10000000;
    int8_t test3_result = 0b00000001;
    int8_t test4_result = 0b10000000;
    int8_t test5_result = 0b00001000;
    int8_t test6_result = 0b00000000;

    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test1), test1_result);
    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test2), test2_result);
    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test3), test3_result);
    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test4), test4_result);
    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test5), test5_result);
    ASSERT_EQ(BitFlag<int8_t>::findHighBit(test6), test6_result);
}

TEST(BitTest, findMax_by_unsigned)
{
    uint8_t test1 = 0b01000001;
    uint8_t test2 = 0b11000000;
    uint8_t test3 = 0b00000001;
    uint8_t test4 = 0b10000000;
    uint8_t test5 = 0b00001001;
    uint8_t test6 = 0b00000000;

    uint8_t test1_result = 0b01000000;
    uint8_t test2_result = 0b10000000;
    uint8_t test3_result = 0b00000001;
    uint8_t test4_result = 0b10000000;
    uint8_t test5_result = 0b00001000;
    uint8_t test6_result = 0b00000000;

    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test1), test1_result);
    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test2), test2_result);
    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test3), test3_result);
    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test4), test4_result);
    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test5), test5_result);
    ASSERT_EQ(BitFlag<uint8_t>::findHighBit(test6), test6_result);
}
