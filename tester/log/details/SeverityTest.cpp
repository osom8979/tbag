/**
 * @file   SeverityTest.cpp
 * @brief  Severity tester.
 * @author zer0
 * @date   2016-07-09
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/Severity.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(SeverityTest, Default)
{
    ASSERT_EQ(LOG_SEVERITY_COUNT, LOG_SEVERITY_OFF + 1);
    ASSERT_STREQ(DEFAULT_SEVERITY[LOG_SEVERITY_EMERGENCY].text, getLogString(LOG_SEVERITY_EMERGENCY));
    ASSERT_STREQ(UNKNOWN_LOG_SEVERITY_STRING, getLogString(-1));
    ASSERT_STREQ(UNKNOWN_LOG_SEVERITY_STRING, getLogString(LOG_SEVERITY_OFF + 1));
}

TEST(SeverityTest, Severity)
{
    Severity s1 = LogLevel::LEVEL_EMERGENCY;
    Severity s2 = s1;

    ASSERT_EQ(0b1, s1.getFlag());
    ASSERT_EQ(0b1, s2.getFlag());

    s2 ^= s1;
    ASSERT_EQ(0b1, s1.getFlag());
    ASSERT_EQ(0b0, s2.getFlag());

    s1 |= s2;
    ASSERT_EQ(0b1, s1.getFlag());
    ASSERT_EQ(0b0, s2.getFlag());

    s1 &= s2;
    ASSERT_EQ(0b0, s1.getFlag());
    ASSERT_EQ(0b0, s2.getFlag());
}
