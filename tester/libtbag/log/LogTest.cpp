/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/archive/Archive.hpp>

#include <thread>
#include <chrono>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::log;
//using namespace libtbag::log::node;

TEST(LogTest, Coverage)
{
    tDLogM("tDLogM()");
    tDLogA("tDLogA()");
    tDLogC("tDLogC()");
    tDLogE("tDLogE()");
    tDLogW("tDLogW()");
    tDLogN("tDLogN()");
    tDLogI("tDLogI()");
    tDLogD("tDLogD()");

    tDLogIfM(true, "tDLogM({})", 1);
    tDLogIfA(true, "tDLogA({})", 1);
    tDLogIfC(true, "tDLogC({})", 1);
    tDLogIfE(true, "tDLogE({})", 1);
    tDLogIfW(true, "tDLogW({})", 1);
    tDLogIfN(true, "tDLogN({})", 1);
    tDLogIfI(true, "tDLogI({})", 1);
    tDLogIfD(true, "tDLogD({})", 1);
}

TEST(LogTest, Stdout)
{
    std::string const LOGGER_NAME = "LogTest.Console";
    createColorStdoutLogger(LOGGER_NAME);
    setSeverity(LOGGER_NAME, log::NOTICE_SEVERITY);

    tLogM(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "emergency", 0.1, 'T');
    tLogA(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "alert", 0.1, 'T');
    tLogC(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "critical", 0.1, 'T');
    tLogE(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "error", 0.1, 'T');
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    tLogW(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "warning", 0.1, 'T');
    tLogN(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "notice", 0.1, 'T');
    tLogI(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "info", 0.1, 'T');
    tLogD(LOGGER_NAME, "LogTest: {0}, {1}, {0}, {2}, {3}", 100, "debug", 0.1, 'T');

    setSeverity(LOGGER_NAME, log::DEBUG_SEVERITY);
    tLogI(LOGGER_NAME, "CHANGE LOG LEVEL: info.");
    tLogD(LOGGER_NAME, "CHANGE LOG LEVEL: debug.");

    setSeverity(LOGGER_NAME, log::OFF_SEVERITY);
    tLogM(LOGGER_NAME, "OFF LOG: tDLogE");
    tLogA(LOGGER_NAME, "OFF LOG: tDLogA");
    tLogC(LOGGER_NAME, "OFF LOG: tDLogC");
    tLogE(LOGGER_NAME, "OFF LOG: tDLogR");
    tLogW(LOGGER_NAME, "OFF LOG: tDLogW");
    tLogN(LOGGER_NAME, "OFF LOG: tDLogN");
    tLogI(LOGGER_NAME, "OFF LOG: tDLogI");
    tLogD(LOGGER_NAME, "OFF LOG: tDLogD");

    removeLogger(LOGGER_NAME);
}

TEST(LogTest, FileSink)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / "file.log";
    std::string const LOGGER_NAME = "LogTest.FileSink";

    createFileLogger(LOGGER_NAME, PATH.toString());
    ASSERT_TRUE(PATH.exists());
    setSeverity(LOGGER_NAME, log::WARNING_SEVERITY);

    tLogM(LOGGER_NAME, "LogTest: emergency");
    tLogA(LOGGER_NAME, "LogTest: alert");
    tLogC(LOGGER_NAME, "LogTest: critical");
    tLogE(LOGGER_NAME, "LogTest: error");
    tLogW(LOGGER_NAME, "LogTest: warning");
    tLogN(LOGGER_NAME, "LogTest: notice");
    tLogI(LOGGER_NAME, "LogTest: info");
    tLogD(LOGGER_NAME, "LogTest: debug");

    removeLogger(LOGGER_NAME);
    ASSERT_LT(0, PATH.getState().size);
    ASSERT_TRUE(PATH.exists());
}

