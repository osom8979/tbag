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
    LoggerManager::getInstance();
}

