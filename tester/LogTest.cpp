/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/Log.hpp>

using namespace libtbag;

TEST(Log, CoverageOnly)
{
    Log::getConsole()->info("Log/Console/Info");
}

