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

template <typename T, typename E>
inline bool __check_floating_point_of_equals(T a, T b, E epsilon)
{
    return Equals<T, E>::isFloatingPoint();
}

template <typename T>
inline bool __check_floating_point_of_equals(T a, T b)
{
    return Equals<T, T>::isFloatingPoint();
}

TEST(EqualsTest, Equals)
{
    ASSERT_FALSE(isFloatingPointEquals(0.015, 0.017, 0.001));
    ASSERT_TRUE (isFloatingPointEquals(0.015, 0.017, 0.1));

    ASSERT_TRUE(__check_floating_point_of_equals(0.101, 0.102, 0.0001));
    ASSERT_TRUE(__check_floating_point_of_equals(0.101, 0.102, 100));
    ASSERT_TRUE(__check_floating_point_of_equals(0.101, 0.102));

    ASSERT_FALSE(__check_floating_point_of_equals(100, 100, 0.01));
    ASSERT_FALSE(__check_floating_point_of_equals(100, 100, 100));
    ASSERT_FALSE(__check_floating_point_of_equals(100, 100));

    ASSERT_FALSE(equals(0.101, 0.102, 0.0001));
    ASSERT_TRUE (equals(0.101, 0.102, 0.01));

    ASSERT_FALSE(equals(2, 1));
    ASSERT_TRUE (equals(1, 1));
    ASSERT_FALSE(equals(1, 2));

    ASSERT_FALSE(equals(2, 1, 0.001));
    ASSERT_TRUE (equals(1, 1, 0.001));

    ASSERT_TRUE(equals(5.0, 4.9999999987));
}

TEST(EqualsTest, GreaterOrEquals)
{
    ASSERT_FALSE(greaterOrEquals(0.101, 0.102, 0.0001));
    ASSERT_TRUE (greaterOrEquals(0.101, 0.102, 0.01));

    ASSERT_TRUE (greaterOrEquals(2, 1));
    ASSERT_TRUE (greaterOrEquals(1, 1));
    ASSERT_FALSE(greaterOrEquals(1, 2));

    ASSERT_TRUE (greaterOrEquals(2, 1, 0.001));
    ASSERT_TRUE (greaterOrEquals(1, 1, 0.001));
    ASSERT_FALSE(greaterOrEquals(1, 2, 0.001));

    ASSERT_TRUE (greaterOrEquals(5.009, 4.9999999987));
    ASSERT_TRUE (greaterOrEquals(5.0  , 4.9999999987));
    ASSERT_FALSE(greaterOrEquals(4.9  , 4.9999999987));
}

TEST(EqualsTest, LessOrEquals)
{
    ASSERT_TRUE(lessOrEquals(0.101, 0.102, 0.0001));
    ASSERT_TRUE(lessOrEquals(0.101, 0.102, 0.01));

    ASSERT_FALSE(lessOrEquals(2, 1));
    ASSERT_TRUE (lessOrEquals(1, 1));
    ASSERT_TRUE (lessOrEquals(1, 2));

    ASSERT_FALSE(lessOrEquals(2, 1, 0.001));
    ASSERT_TRUE (lessOrEquals(1, 1, 0.001));
    ASSERT_TRUE (lessOrEquals(1, 2, 0.001));

    ASSERT_FALSE(lessOrEquals(5.009, 4.9999999987));
    ASSERT_TRUE (lessOrEquals(5.0  , 4.9999999987));
    ASSERT_TRUE (lessOrEquals(4.9  , 4.9999999987));
}

