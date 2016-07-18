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

    log::emergency("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::alert    ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::critical ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::error    ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::warning  ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::notice   ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::info     ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');
    log::debug    ("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "TEST", 0.1, 'T');

    log::removeDefaultLogger();
}

