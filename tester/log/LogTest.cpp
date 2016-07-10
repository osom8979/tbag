/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::log;

TEST(LogStaticTest, CoverageOnly)
{
    Log::getInstance()->setLevel(Log::LOG_LEVEL_DEBUG);
    Log::getInstance()->setDefaultFormat();
    Log::getInstance()->setAsyncMode();

    Log::getInstance()->setEnableConsole();
    Log::getInstance()->setEnableFile();

    Log::getInstance()->initConsole();
    Log::getInstance()->initFile("Log");

    Log::getInstance()->trace("Trace");
    Log::getInstance()->debug("Debug");
    Log::getInstance()->info("Info");
    Log::getInstance()->error("Error");
    Log::getInstance()->emerg("Emergency");

    _TLOG("TRACE");
    _DLOG("DEBUG");
}

