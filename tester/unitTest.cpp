/**
 * @file   unitTest.cpp
 * @brief  unit package tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/unit.hpp>

using namespace libtbag;

TEST(unitTest, Byte)
{
    ASSERT_EQ(     BYTE_UNIT, 1024ull);
    ASSERT_EQ(KILO_BYTE_UNIT, 1024ull * 1024ull);
    ASSERT_EQ(MEGA_BYTE_UNIT, 1024ull * 1024ull * 1024ull);
    ASSERT_EQ(GIGA_BYTE_UNIT, 1024ull * 1024ull * 1024ull * 1024ull);
    ASSERT_EQ(TERA_BYTE_UNIT, 1024ull * 1024ull * 1024ull * 1024ull * 1024ull);
}

