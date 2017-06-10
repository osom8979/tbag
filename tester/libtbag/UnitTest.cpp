/**
 * @file   UnitTest.cpp
 * @brief  Unit package tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/Unit.hpp>

using namespace libtbag;

TEST(UnitTest, Byte)
{
    ASSERT_EQ(                              1024ull, KILO_BYTE_TO_BYTE);
    ASSERT_EQ(                    1024ull * 1024ull, MEGA_BYTE_TO_BYTE);
    ASSERT_EQ(          1024ull * 1024ull * 1024ull, GIGA_BYTE_TO_BYTE);
    ASSERT_EQ(1024ull * 1024ull * 1024ull * 1024ull, TERA_BYTE_TO_BYTE);
}

