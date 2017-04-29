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
using namespace libtbag::log;

TEST(LogTest, Console)
{
    createDefaultColorConsoleLogger();
    setDefaultLevel(log::NOTICE_SEVERITY);

    tDLogM("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "emergency", 0.1, 'T');
    tDLogA("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "alert", 0.1, 'T');
    tDLogC("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "critical", 0.1, 'T');
    tDLogE("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "error", 0.1, 'T');
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tDLogW("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "warning", 0.1, 'T');
    tDLogN("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "notice", 0.1, 'T');
    tDLogI("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "info", 0.1, 'T');
    tDLogD("LogTest: {0}, {1}, {0}, {2}, {3}", 100, "debug", 0.1, 'T');

    setDefaultLevel(log::DEBUG_SEVERITY);
    tDLogI("CHANGE LOG LEVEL: info.");
    tDLogD("CHANGE LOG LEVEL: debug.");

    setDefaultLevel(log::OFF_SEVERITY);
    tDLogM("OFF LOG: tDLogE");
    tDLogA("OFF LOG: tDLogA");
    tDLogC("OFF LOG: tDLogC");
    tDLogE("OFF LOG: tDLogR");
    tDLogW("OFF LOG: tDLogW");
    tDLogN("OFF LOG: tDLogN");
    tDLogI("OFF LOG: tDLogI");
    tDLogD("OFF LOG: tDLogD");

    removeDefaultLogger();
}

TEST(LogTest, File)
{
    createDefaultFileLogger("__log_test_file");
    setDefaultLevel(log::WARNING_SEVERITY);

    tDLogM("LogTest: emergency");
    tDLogA("LogTest: alert");
    tDLogC("LogTest: critical");
    tDLogE("LogTest: error");
    tDLogW("LogTest: warning");
    tDLogN("LogTest: notice");
    tDLogI("LogTest: info");
    tDLogD("LogTest: debug");

    removeDefaultLogger();
}

