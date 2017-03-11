/**
 * @file   LoggerManagerTest.cpp
 * @brief  LoggerManager class tester.
 * @author zer0
 * @date   2016-07-17
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/LoggerManager.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(LoggerManagerTest, Default)
{
    LoggerManager::getInstance();
}

