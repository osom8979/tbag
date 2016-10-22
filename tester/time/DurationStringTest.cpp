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
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::nanoseconds >::value);
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::microseconds>::value);
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::milliseconds>::value);
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::seconds     >::value);
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::minutes     >::value);
    ASSERT_STRNE(__TBAG_DEFAULT_DURATION_STRING__, DurationString<std::chrono::hours       >::value);

    using TestRatio    = std::ratio<10>;
    using TestDuration = std::chrono::duration<char, TestRatio>;
    ASSERT_STREQ(__TBAG_DEFAULT_DURATION_STRING__, DurationString<TestDuration>::value);
}

