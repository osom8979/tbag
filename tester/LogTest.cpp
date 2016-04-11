/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/Log.hpp>

using namespace libtbag;

TEST(LogStaticTest, CoverageOnly)
{
    Log::getInstance()->setLevel(Log::Level::DBG);
    Log::getInstance()->setDefaultFormat();

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

