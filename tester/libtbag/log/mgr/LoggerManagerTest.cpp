/**
 * @file   LoggerManagerTest.cpp
 * @brief  LoggerManager class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/log/mgr/LoggerManager.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::mgr;

TEST(LoggerManagerTest, Default)
{
    LoggerManager::getInstance();
}

