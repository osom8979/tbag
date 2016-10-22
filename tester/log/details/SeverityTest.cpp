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
    ASSERT_EQ(LOG_SEVERITY_COUNT, static_cast<int>(LogLevel::LEVEL_OFF) + 1);
    ASSERT_STREQ(DEFAULT_SEVERITY[static_cast<int>(LogLevel::LEVEL_EMERGENCY)], getLogString(LogLevel::LEVEL_EMERGENCY));
    ASSERT_STREQ(UNKNOWN_LOG_SEVERITY_STRING, getLogString(-1));
    ASSERT_STREQ(UNKNOWN_LOG_SEVERITY_STRING, getLogString(static_cast<int>(LogLevel::LEVEL_OFF) + 1));
}

TEST(SeverityTest, Severity)
{
    Severity s1 = LogLevel::LEVEL_EMERGENCY;
    Severity s2 = s1;

    ASSERT_EQ(static_cast<int>(LogLevel::LEVEL_EMERGENCY), s1.level);
    ASSERT_EQ(static_cast<int>(LogLevel::LEVEL_EMERGENCY), s2.level);
}

