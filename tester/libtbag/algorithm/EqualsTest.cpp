/**
 * @file   EqualsTest.cpp
 * @brief  Equals class tester.
 * @author zer0
 * @date   2017-07-20
 */

#include <gtest/gtest.h>
#include <libtbag/algorithm/Equals.hpp>

using namespace libtbag;
using namespace libtbag::algorithm;

TEST(EqualsTest, Default)
{
    ASSERT_FALSE(isFloatingPointEquals(0.015, 0.017, 0.001));
    ASSERT_TRUE (isFloatingPointEquals(0.015, 0.017, 0.1));

    ASSERT_FALSE(equals(0.101, 0.102, 0.0001));
    ASSERT_TRUE (equals(0.101, 0.102, 0.01));

    ASSERT_FALSE(equals(2, 1));
    ASSERT_TRUE (equals(1, 1));

    ASSERT_FALSE(equals(2, 1, 0.001));
    ASSERT_TRUE (equals(1, 1, 0.001));
}

