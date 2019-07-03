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

TEST(SeverityTest, Severity)
{
    Severity s1 = EMERGENCY_SEVERITY;
    Severity s2 = s1;

    ASSERT_EQ(static_cast<int>(EMERGENCY_SEVERITY), s1.getLevel());
    ASSERT_EQ(static_cast<int>(EMERGENCY_SEVERITY), s2.getLevel());
}

