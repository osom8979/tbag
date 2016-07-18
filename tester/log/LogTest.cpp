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

TEST(LogTest, Default)
{
    log::createDefaultConsoleLogger();
    log::setDefaultLevel(log::LogLevel::LEVEL_NOTICE);

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

