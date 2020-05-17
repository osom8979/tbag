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

TEST(DurationStringTest, SystemClock)
{
    using system_duration = std::chrono::system_clock::duration;
    auto const text = getDurationString<system_duration>();
    std::cout << "System clock(rep=" << system_duration::period::num
              << "/" << system_duration::period::den
              << ") duration text: " << text << std::endl;
}

TEST(DurationStringTest, ParseNanoseconds)
{
    ASSERT_EQ(4000, parseNanoseconds("4000").value);
    ASSERT_EQ(2000, parseNanoseconds("2000nano").value);
    ASSERT_EQ(1000, parseNanoseconds("1000n").value);
    ASSERT_EQ(1000*1000, parseNanoseconds("1000micro").value);
    ASSERT_EQ(1000*1000*1000, parseNanoseconds("1000milli").value);
    ASSERT_EQ(1000*1000*1000, parseNanoseconds("1sec").value);
}

TEST(DurationStringTest, ParseMicroseconds)
{
    ASSERT_EQ(1000, parseMicroseconds("1000").value);
    ASSERT_EQ(1, parseMicroseconds("1000nano").value);
    ASSERT_EQ(1000, parseMicroseconds("1000micro").value);
    ASSERT_EQ(1000*1000, parseMicroseconds("1000milli").value);
    ASSERT_EQ(1000*1000*1000, parseMicroseconds("1000sec").value);
    ASSERT_EQ(60*1000*1000, parseMicroseconds("1min").value);
    ASSERT_EQ(1llu*60*60*1000*1000, parseMicroseconds("1h").value);
}

TEST(DurationStringTest, ParseMilliseconds)
{
    ASSERT_EQ(4000, parseMilliseconds("4000").value);
    ASSERT_EQ(2000, parseMilliseconds("2000milli").value);
    ASSERT_EQ(1000, parseMilliseconds("1000milliseconds").value);
    ASSERT_EQ(1000, parseMilliseconds("1sec").value);

    ASSERT_EQ(0, parseMilliseconds("1 nano").value);
    ASSERT_EQ(0, parseMilliseconds("1 micro").value);
    ASSERT_EQ(60*1000, parseMilliseconds("1 min").value);
    ASSERT_EQ(2000, parseMilliseconds("2 s").value);
    ASSERT_EQ(2*60*60*1000, parseMilliseconds("2 h").value);
}

TEST(DurationStringTest, ParseSeconds)
{
    ASSERT_EQ(1000, parseSeconds("1000").value);
    ASSERT_EQ(0, parseSeconds("1000nano").value);
    ASSERT_EQ(0, parseSeconds("1000micro").value);
    ASSERT_EQ(1, parseSeconds("1000milli").value);
    ASSERT_EQ(1000, parseSeconds("1000sec").value);
    ASSERT_EQ(60, parseSeconds("1min").value);
    ASSERT_EQ(60*60, parseSeconds("1h").value);
}

TEST(DurationStringTest, ParseMinutes)
{
    ASSERT_EQ(1000, parseMinutes("1000").value);
    ASSERT_EQ(0, parseMinutes("1000nano").value);
    ASSERT_EQ(0, parseMinutes("1000micro").value);
    ASSERT_EQ(0, parseMinutes("1000milli").value);
    ASSERT_EQ(1000/60, parseMinutes("1000sec").value);
    ASSERT_EQ(1, parseMinutes("1min").value);
    ASSERT_EQ(60, parseMinutes("1h").value);
}

TEST(DurationStringTest, ParseHours)
{
    ASSERT_EQ(1, parseHours("1").value);
    ASSERT_EQ(1, parseHours("1h").value);
    ASSERT_EQ(0, parseHours("1min").value);
    ASSERT_EQ(0, parseHours("1sec").value);
    ASSERT_EQ(0, parseHours("1mil").value);
    ASSERT_EQ(0, parseHours("1mic").value);
    ASSERT_EQ(0, parseHours("1nano").value);
}

TEST(DurationStringTest, GetUpperTimeText)
{
    ASSERT_STREQ("1d", getUpperTimeTextByHours(24).c_str());
}

