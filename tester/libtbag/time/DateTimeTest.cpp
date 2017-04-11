/**
 * @file   DateTimeTest.cpp
 * @brief  DateTime class tester.
 * @author zer0
 * @date   2017-04-05
 */

#include <gtest/gtest.h>
#include <libtbag/time/DateTime.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::time;

TEST(DateTimeTest, Default)
{
    DateTime dt(2017, 4, 5, 3, 28, 27, 100, 99);
    ASSERT_EQ(2017, dt.getYear());
    ASSERT_EQ(   4, dt.getMonth());
    ASSERT_EQ(   5, dt.getDay());
    ASSERT_EQ(   3, dt.getHours());
    ASSERT_EQ(  28, dt.getMinutes());
    ASSERT_EQ(  27, dt.getSeconds());
    ASSERT_EQ( 100, dt.getMillisec());
    ASSERT_EQ(  99, dt.getMicrosec());

    DateTime dt2(1900, 1, 1);
    ASSERT_EQ(1900, dt2.getYear());
    ASSERT_EQ(   1, dt2.getMonth());
    ASSERT_EQ(   1, dt2.getDay());
    ASSERT_EQ(   0, dt2.getHours());
    ASSERT_EQ(   0, dt2.getMinutes());
    ASSERT_EQ(   0, dt2.getSeconds());
    ASSERT_EQ(   0, dt2.getMillisec());
    ASSERT_EQ(   0, dt2.getMicrosec());

    ASSERT_GT(dt, dt2);

    DateTime dt3;
    DateTime dt4(dt3.getYear(), dt3.getMonth(), dt3.getDay(), dt3.getHours(), dt3.getMinutes(), dt3.getSeconds(), dt3.getMillisec(), dt3.getMicrosec());
    ASSERT_EQ(dt3, dt4);

    DateTime dt5 = dt3;
    ASSERT_EQ(dt3, dt5);
}

