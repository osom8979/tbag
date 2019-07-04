/**
 * @file   SeverityTest.cpp
 * @brief  Severity tester.
 * @author zer0
 * @date   2016-07-09
 */

#include <gtest/gtest.h>
#include <libtbag/log/Severity.hpp>

using namespace libtbag;
using namespace libtbag::log;

TEST(SeverityTest, Default)
{
    Severity s1 = EMERGENCY_SEVERITY;
    Severity s2 = s1;

    ASSERT_EQ(static_cast<int>(EMERGENCY_SEVERITY), s1.getLevel());
    ASSERT_EQ(static_cast<int>(EMERGENCY_SEVERITY), s2.getLevel());
}

TEST(SeverityTest, IsContain)
{
    ASSERT_TRUE(DEBUG_SEVERITY.isContain(NOTICE_SEVERITY));
    ASSERT_TRUE(DEBUG_SEVERITY.isContain(INFO_SEVERITY));
    ASSERT_TRUE(DEBUG_SEVERITY.isContain(DEBUG_SEVERITY));

    ASSERT_TRUE(INFO_SEVERITY.isContain(NOTICE_SEVERITY));
    ASSERT_TRUE(INFO_SEVERITY.isContain(INFO_SEVERITY));
    ASSERT_FALSE(INFO_SEVERITY.isContain(DEBUG_SEVERITY));

    ASSERT_TRUE(NOTICE_SEVERITY.isContain(NOTICE_SEVERITY));
    ASSERT_FALSE(NOTICE_SEVERITY.isContain(INFO_SEVERITY));
    ASSERT_FALSE(NOTICE_SEVERITY.isContain(DEBUG_SEVERITY));

    // clang-format off
    ASSERT_TRUE( ALERT_SEVERITY.isContain(-999));
    ASSERT_TRUE( ALERT_SEVERITY.isContain(      OFF_SEVERITY));
    ASSERT_TRUE( ALERT_SEVERITY.isContain(EMERGENCY_SEVERITY));
    ASSERT_TRUE( ALERT_SEVERITY.isContain(    ALERT_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain( CRITICAL_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(    ERROR_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(  WARNING_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(   NOTICE_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(     INFO_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(    DEBUG_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(  UNKNOWN_SEVERITY));
    ASSERT_FALSE(ALERT_SEVERITY.isContain(9999));
    // clang-format on
}

