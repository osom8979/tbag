/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;

TEST(LogTest, Console)
{
    log::createDefaultColorConsoleLogger();
    log::setDefaultLevel(log::LogLevel::LEVEL_NOTICE);

    tDLogEF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "emergency", 0.1, 'T');
    tDLogAF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "alert", 0.1, 'T');
    tDLogCF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "critical", 0.1, 'T');
    tDLogRF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "error", 0.1, 'T');
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tDLogWF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "warning", 0.1, 'T');
    tDLogNF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "notice", 0.1, 'T');
    tDLogIF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "info", 0.1, 'T');
    tDLogDF("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "debug", 0.1, 'T');

    log::setDefaultLevel(log::LogLevel::LEVEL_DEBUG);
    tDLogI("CHANGE LOG LEVEL: info.");
    tDLogD("CHANGE LOG LEVEL: debug.");

    log::setDefaultLevel(log::LogLevel::LEVEL_OFF);
    tDLogE("OFF LOG: tDLogE");
    tDLogA("OFF LOG: tDLogA");
    tDLogC("OFF LOG: tDLogC");
    tDLogR("OFF LOG: tDLogR");
    tDLogW("OFF LOG: tDLogW");
    tDLogN("OFF LOG: tDLogN");
    tDLogI("OFF LOG: tDLogI");
    tDLogD("OFF LOG: tDLogD");

    log::removeDefaultLogger();
}

TEST(LogTest, File)
{
    log::createDefaultFileLogger("__log_test_file");
    log::setDefaultLevel(log::LogLevel::LEVEL_WARNING);

    tDLogE("LogTest: emergency");
    tDLogA("LogTest: alert");
    tDLogC("LogTest: critical");
    tDLogR("LogTest: error");
    tDLogW("LogTest: warning");
    tDLogN("LogTest: notice");
    tDLogI("LogTest: info");
    tDLogD("LogTest: debug");

    log::removeDefaultLogger();
}

