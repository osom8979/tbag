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

    log::emergency    ("LOG_TEST: emergency");
    log::alert        ("LOG_TEST: alert");
    log::critical     ("LOG_TEST: critical");
    log::error        ("LOG_TEST: error");
    log::warning      ("LOG_TEST: warning");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    log::notice       ("LOG_TEST: notice");
    log::informational("LOG_TEST: informational");
    log::debug        ("LOG_TEST: debug");

    log::removeDefaultLogger();
}

