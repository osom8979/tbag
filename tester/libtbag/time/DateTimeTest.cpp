/**
 * @file   DateTimeTest.cpp
 * @brief  DateTime class tester.
 * @author zer0
 * @date   2017-04-05
 */

#include <gtest/gtest.h>
#include <libtbag/time/DateTime.hpp>

using namespace libtbag;
using namespace libtbag::time;

TEST(DateTimeTest, Default)
{
    DateTime object;
    ASSERT_TRUE(true);
}

