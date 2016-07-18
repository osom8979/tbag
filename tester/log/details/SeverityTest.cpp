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
    ASSERT_EQ(LOG_SEVERITY_OFF + 1, LOG_SEVERITY_COUNT);
    ASSERT_STREQ(getLogString(LOG_SEVERITY_EMERGENCY), DEFAULT_SEVERITY[LOG_SEVERITY_EMERGENCY].text);
    ASSERT_STREQ(getLogString(-1), UNKNOWN_LOG_SEVERITY_STRING);
    ASSERT_STREQ(getLogString(LOG_SEVERITY_OFF + 1), UNKNOWN_LOG_SEVERITY_STRING);
}

TEST(SeverityTest, Severity)
{
    Severity s1 = LogLevel::LEVEL_EMERGENCY;
    Severity s2 = s1;

    ASSERT_EQ(s1.getFlag(), 0b1);
    ASSERT_EQ(s2.getFlag(), 0b1);

    s2 ^= s1;
    ASSERT_EQ(s1.getFlag(), 0b1);
    ASSERT_EQ(s2.getFlag(), 0b0);

    s1 |= s2;
    ASSERT_EQ(s1.getFlag(), 0b1);
    ASSERT_EQ(s2.getFlag(), 0b0);

    s1 &= s2;
    ASSERT_EQ(s1.getFlag(), 0b0);
    ASSERT_EQ(s2.getFlag(), 0b0);
}
