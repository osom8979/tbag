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

TEST(LogTest, Parse)
{
    ASSERT_TRUE(parseAutoFlush(TBAG_LOGGER_AUTO_FLUSH_ON));
    ASSERT_FALSE(parseAutoFlush(TBAG_LOGGER_AUTO_FLUSH_OFF));
    ASSERT_FALSE(parseAutoFlush(""));

    ASSERT_TRUE(parseMultiThread(TBAG_LOGGER_MULTITHREAD_ON));
    ASSERT_FALSE(parseMultiThread(TBAG_LOGGER_MULTITHREAD_OFF));
    ASSERT_FALSE(parseMultiThread(""));

    ASSERT_TRUE(parseMutexThread(TBAG_LOGGER_MUTEX_ON));
    ASSERT_FALSE(parseMutexThread(TBAG_LOGGER_MUTEX_OFF));
    ASSERT_FALSE(parseMutexThread(""));

    Severity const       OFF_SEVERITY = ::libtbag::log::level::OFF_SEVERITY;
    Severity const EMERGENCY_SEVERITY = ::libtbag::log::level::EMERGENCY_SEVERITY;
    Severity const     ALERT_SEVERITY = ::libtbag::log::level::ALERT_SEVERITY;
    Severity const  CRITICAL_SEVERITY = ::libtbag::log::level::CRITICAL_SEVERITY;
    Severity const     ERROR_SEVERITY = ::libtbag::log::level::ERROR_SEVERITY;
    Severity const   WARNING_SEVERITY = ::libtbag::log::level::WARNING_SEVERITY;
    Severity const    NOTICE_SEVERITY = ::libtbag::log::level::NOTICE_SEVERITY;
    Severity const      INFO_SEVERITY = ::libtbag::log::level::INFORMATIONAL_SEVERITY;
    Severity const     DEBUG_SEVERITY = ::libtbag::log::level::DEBUG_SEVERITY;

    ASSERT_EQ(      OFF_SEVERITY, parseSeverity(OFF_SEVERITY.text));
    ASSERT_EQ(EMERGENCY_SEVERITY, parseSeverity(EMERGENCY_SEVERITY.text));
    ASSERT_EQ(    ALERT_SEVERITY, parseSeverity(ALERT_SEVERITY.text));
    ASSERT_EQ( CRITICAL_SEVERITY, parseSeverity(CRITICAL_SEVERITY.text));
    ASSERT_EQ(    ERROR_SEVERITY, parseSeverity(ERROR_SEVERITY.text));
    ASSERT_EQ(  WARNING_SEVERITY, parseSeverity(WARNING_SEVERITY.text));
    ASSERT_EQ(   NOTICE_SEVERITY, parseSeverity(NOTICE_SEVERITY.text));
    ASSERT_EQ(     INFO_SEVERITY, parseSeverity(INFO_SEVERITY.text));
    ASSERT_EQ(    DEBUG_SEVERITY, parseSeverity(DEBUG_SEVERITY.text));
    ASSERT_EQ(      OFF_SEVERITY, parseSeverity(""));

    ASSERT_EQ(MakeType::DEFAULT, parseGeneratorType(TBAG_LOGGER_GENERATOR_DEFAULT));
    ASSERT_EQ(MakeType::DEFAULT_COLOR, parseGeneratorType(TBAG_LOGGER_GENERATOR_DEFAULT_COLOR));
    ASSERT_EQ(MakeType::DEFAULT, parseGeneratorType(""));
}

