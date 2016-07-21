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

    tDLogE("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "emergency", 0.1, 'T');
    tDLogA("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "alert", 0.1, 'T');
    tDLogC("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "critical", 0.1, 'T');
    tDLogR("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "error", 0.1, 'T');
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tDLogW("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "warning", 0.1, 'T');
    tDLogN("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "notice", 0.1, 'T');
    tDLogI("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "info", 0.1, 'T');
    tDLogD("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "debug", 0.1, 'T');

    log::setDefaultLevel(log::LogLevel::LEVEL_DEBUG);
    tDLogI("CHANGE LOG LEVEL: {}", "info");
    tDLogD("CHANGE LOG LEVEL: {}", "debug");

    log::removeDefaultLogger();
}

TEST(LogTest, File)
{
    log::createDefaultFileLogger("__log_test_file");
    log::setDefaultLevel(log::LogLevel::LEVEL_WARNING);

    log::emergency("LogTest: emergency");
    log::alert    ("LogTest: alert");
    log::critical ("LogTest: critical");
    log::error    ("LogTest: error");
    log::warning  ("LogTest: warning");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    log::notice   ("LogTest: notice");
    log::info     ("LogTest: info");
    log::debug    ("LogTest: debug");

    tDLogE("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogA("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogC("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogR("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogW("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogN("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogI("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    tDLogD("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');

    log::removeDefaultLogger();
}

