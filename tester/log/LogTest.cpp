/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>

using namespace libtbag;

TEST(LogTest, Default)
{
    log::createDefaultConsoleLogger();

    log::getDefaultLogger()->emergency("LOG_EMERGENCY");
    log::getDefaultLogger()->debug("LOG_DEBUG");
    log::getDefaultLogger()->informational("LOG_INFORMATIONAL");

    log::removeDefaultLogger();

//    Log::getInstance()->setLevel(Log::LOG_LEVEL_DEBUG);
//    Log::getInstance()->setDefaultFormat();
//    Log::getInstance()->setAsyncMode();
//
//    Log::getInstance()->setEnableConsole();
//    Log::getInstance()->setEnableFile();
//
//    Log::getInstance()->initConsole();
//    Log::getInstance()->initFile("Log");
//
//    Log::getInstance()->trace("Trace");
//    Log::getInstance()->debug("Debug");
//    Log::getInstance()->info("Info");
//    Log::getInstance()->error("Error");
//    Log::getInstance()->emerg("Emergency");
//
//    _TLOG("TRACE");
//    _DLOG("DEBUG");
}

