/**
 * @file   LoggerTest.cpp
 * @brief  Logger class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/log/mgr/Logger.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::mgr;

TEST(LoggerTest, Default)
{
    Logger object;
    ASSERT_TRUE(true);
}

