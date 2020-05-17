/**
 * @file   ByteTest.cpp
 * @brief  Byte class tester.
 * @author zer0
 * @date   2020-05-17
 */

#include <gtest/gtest.h>
#include <libtbag/util/Byte.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(ByteTest, Constructors)
{
    ASSERT_EQ(Byte(1), Byte("1b"));
}

