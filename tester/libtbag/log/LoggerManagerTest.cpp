/**
 * @file   LoggerManagerTest.cpp
 * @brief  LoggerManager class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/log/LoggerManager.hpp>

using namespace libtbag;
using namespace libtbag::log;

TEST(LoggerManagerTest, Default)
{
    auto * mgr = LoggerManager::getInstance();
    ASSERT_NE(nullptr, mgr);

    char const * const TEST_LOGGER_NAME = "test";
    ASSERT_NE(nullptr, createStdoutLogger(TEST_LOGGER_NAME));
    ASSERT_NE(nullptr, getLogger(TEST_LOGGER_NAME));
    ASSERT_TRUE(existsLogger(TEST_LOGGER_NAME));

    setSeverity(TEST_LOGGER_NAME, ERROR_SEVERITY);
    ASSERT_EQ(ERROR_SEVERITY, getSeverity(TEST_LOGGER_NAME));

    setAutoFlush(TEST_LOGGER_NAME);
    ASSERT_TRUE(getAutoFlush(TEST_LOGGER_NAME));

    ASSERT_TRUE(removeLogger(TEST_LOGGER_NAME));
}

