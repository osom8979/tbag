/**
 * @file   DurationStringTest.cpp
 * @brief  DurationString class tester.
 * @author zer0
 * @date   2016-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/time/DurationString.hpp>

using namespace libtbag;
using namespace libtbag::time;

TEST(DurationStringTest, Default)
{
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::nanoseconds >());
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::microseconds>());
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::milliseconds>());
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::seconds     >());
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::minutes     >());
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<std::chrono::hours       >());

    using TestRatio    = std::ratio<10>;
    using TestDuration = std::chrono::duration<char, TestRatio>;
    ASSERT_STREQ(__TBAG_DEFAULT_DURATION_STRING__, getDurationString<TestDuration>());
}

