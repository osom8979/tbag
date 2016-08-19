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
    ASSERT_EQ(                                        1024ull,      BYTE_UNIT);
    ASSERT_EQ(                              1024ull * 1024ull, KILO_BYTE_UNIT);
    ASSERT_EQ(                    1024ull * 1024ull * 1024ull, MEGA_BYTE_UNIT);
    ASSERT_EQ(          1024ull * 1024ull * 1024ull * 1024ull, GIGA_BYTE_UNIT);
    ASSERT_EQ(1024ull * 1024ull * 1024ull * 1024ull * 1024ull, TERA_BYTE_UNIT);
}

