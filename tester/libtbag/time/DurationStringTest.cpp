/**
 * @file   DurationStringTest.cpp
 * @brief  DurationString class tester.
 * @author zer0
 * @date   2016-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/time/DurationString.hpp>
#include <cstring>

using namespace libtbag;
using namespace libtbag::time;

TEST(DurationStringTest, Default)
{
    ASSERT_STREQ(TBAG_NANO_DURATION_STRING, getDurationString<std::chrono::nanoseconds >());
    ASSERT_STREQ(TBAG_MICRO_DURATION_STRING, getDurationString<std::chrono::microseconds>());
    ASSERT_STREQ(TBAG_MILLI_DURATION_STRING, getDurationString<std::chrono::milliseconds>());
    ASSERT_STREQ(TBAG_SEC_DURATION_STRING, getDurationString<std::chrono::seconds     >());
    ASSERT_STREQ(TBAG_MIN_DURATION_STRING, getDurationString<std::chrono::minutes     >());
    ASSERT_STREQ(TBAG_HOUR_DURATION_STRING, getDurationString<std::chrono::hours       >());

    using TestRatio = std::ratio<10>;
    using TestDuration = std::chrono::duration<char, TestRatio>;
    ASSERT_EQ(0, strlen(getDurationString<TestDuration>()));
}

TEST(DurationStringTest, ToMilliseconds)
{
    ASSERT_EQ(4000, toMilliseconds("4000"));
    ASSERT_EQ(2000, toMilliseconds("2000mil"));
    ASSERT_EQ(1000, toMilliseconds("1000milliseconds"));
    ASSERT_EQ(1000, toMilliseconds("1sec"));
    ASSERT_EQ(0, toMilliseconds("1nano"));
    ASSERT_EQ(0, toMilliseconds("1micro"));
    ASSERT_EQ(60*1000, toMilliseconds("1min"));
    ASSERT_EQ(2000, toMilliseconds("2s"));
    ASSERT_EQ(2*60*60*1000, toMilliseconds("2h"));
}

TEST(DurationStringTest, ToNanoseconds)
{
    ASSERT_EQ(4000, toNanoseconds("4000"));
    ASSERT_EQ(2000, toNanoseconds("2000nano"));
    ASSERT_EQ(1000, toNanoseconds("1000n"));
    ASSERT_EQ(1000*1000, toNanoseconds("1000micro"));
    ASSERT_EQ(1000*1000*1000, toNanoseconds("1000milli"));
    ASSERT_EQ(1000*1000*1000, toNanoseconds("1sec"));
}

TEST(DurationStringTest, ToHours)
{
    ASSERT_EQ(1, toHours("1"));
    ASSERT_EQ(1, toHours("1h"));
    ASSERT_EQ(0, toHours("1min"));
    ASSERT_EQ(0, toHours("1sec"));
    ASSERT_EQ(0, toHours("1mil"));
    ASSERT_EQ(0, toHours("1mic"));
    ASSERT_EQ(0, toHours("1nano"));
}

TEST(DurationStringTest, GetUpperTimeText)
{
    ASSERT_STREQ("1day", getUpperTimeTextByHours(24).c_str());
    ASSERT_STREQ("1month", getUpperTimeTextByHours(24*30).c_str());
    ASSERT_STREQ("1year", getUpperTimeTextByHours(24*365).c_str());
}

