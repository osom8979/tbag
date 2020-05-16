/**
 * @file   DurationTest.cpp
 * @brief  Duration class tester.
 * @author zer0
 * @date   2020-05-15
 */

#include <gtest/gtest.h>
#include <libtbag/time/Duration.hpp>
#include <sstream>

using namespace libtbag;
using namespace libtbag::time;

TEST(DurationTest, Constructors)
{
    ASSERT_EQ(Duration(1), Duration("1ns"));
    ASSERT_EQ(Duration(std::chrono::microseconds(2)), Duration("2us"));
}

TEST(DurationTest, Default)
{
    ASSERT_EQ(Duration(1), Duration(1));
    ASSERT_NE(Duration(1), Duration(2));
    std::stringstream ss;
    ss << Duration(10000000);
    ASSERT_FALSE(ss.str().empty());
}

