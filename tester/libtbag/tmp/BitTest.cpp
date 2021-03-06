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

TEST(BitTest, Max)
{
    uint8_t const max_8bit_unsigned = (int8_t)0x80; // 0b10000000;
    int8_t  const max_8bit_signed   = (int8_t)0x80; // 0b10000000;

    ASSERT_EQ(max_8bit_unsigned, BitFlag<uint8_t>::max());
    ASSERT_EQ(  max_8bit_signed, BitFlag< int8_t>::max());
}

TEST(BitTest, FindMax_Signed)
{
    int8_t const test1 = (int8_t)0x41; // 0b01000001;
    int8_t const test2 = (int8_t)0xC0; // 0b11000000;
    int8_t const test3 = (int8_t)0x01; // 0b00000001;
    int8_t const test4 = (int8_t)0x80; // 0b10000000;
    int8_t const test5 = (int8_t)0x09; // 0b00001001;
    int8_t const test6 = (int8_t)0x00; // 0b00000000;

    int8_t const test1_result = (int8_t)0x40; // 0b01000000;
    int8_t const test2_result = (int8_t)0x80; // 0b10000000;
    int8_t const test3_result = (int8_t)0x01; // 0b00000001;
    int8_t const test4_result = (int8_t)0x80; // 0b10000000;
    int8_t const test5_result = (int8_t)0x08; // 0b00001000;
    int8_t const test6_result = (int8_t)0x00; // 0b00000000;

    ASSERT_EQ(test1_result, BitFlag<int8_t>::findHighBit(test1));
    ASSERT_EQ(test2_result, BitFlag<int8_t>::findHighBit(test2));
    ASSERT_EQ(test3_result, BitFlag<int8_t>::findHighBit(test3));
    ASSERT_EQ(test4_result, BitFlag<int8_t>::findHighBit(test4));
    ASSERT_EQ(test5_result, BitFlag<int8_t>::findHighBit(test5));
    ASSERT_EQ(test6_result, BitFlag<int8_t>::findHighBit(test6));
}

TEST(BitTest, FindMax_Unsigned)
{
    uint8_t const test1 = (int8_t)0x41; // 0b01000001;
    uint8_t const test2 = (int8_t)0xC0; // 0b11000000;
    uint8_t const test3 = (int8_t)0x01; // 0b00000001;
    uint8_t const test4 = (int8_t)0x80; // 0b10000000;
    uint8_t const test5 = (int8_t)0x09; // 0b00001001;
    uint8_t const test6 = (int8_t)0x00; // 0b00000000;

    uint8_t const test1_result = (int8_t)0x40; // 0b01000000;
    uint8_t const test2_result = (int8_t)0x80; // 0b10000000;
    uint8_t const test3_result = (int8_t)0x01; // 0b00000001;
    uint8_t const test4_result = (int8_t)0x80; // 0b10000000;
    uint8_t const test5_result = (int8_t)0x08; // 0b00001000;
    uint8_t const test6_result = (int8_t)0x00; // 0b00000000;

    ASSERT_EQ(test1_result, BitFlag<uint8_t>::findHighBit(test1));
    ASSERT_EQ(test2_result, BitFlag<uint8_t>::findHighBit(test2));
    ASSERT_EQ(test3_result, BitFlag<uint8_t>::findHighBit(test3));
    ASSERT_EQ(test4_result, BitFlag<uint8_t>::findHighBit(test4));
    ASSERT_EQ(test5_result, BitFlag<uint8_t>::findHighBit(test5));
    ASSERT_EQ(test6_result, BitFlag<uint8_t>::findHighBit(test6));
}

