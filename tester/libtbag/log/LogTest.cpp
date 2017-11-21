/**
 * @file   LogTest.cpp
 * @brief  Log class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::log;

TEST(LogTest, Console)
{
    std::string const LOGGER_NAME = "LogTest.Console";
    createColorConsoleLogger(LOGGER_NAME);
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
    tttDir(true, true);
    auto const PATH = tttDirGet() / "file.log";
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

TEST(LogTest, RotateFileSink)
{
    tttDir(true, true);
    auto const PATH = tttDirGet() / "rotate.log";
    std::string const LOGGER_NAME = "LogTest.RotateFileSink";
    std::size_t const MAX_SIZE = 128;

    std::string message;
    message.assign(MAX_SIZE, '0');
    ASSERT_EQ(MAX_SIZE, message.size());

    bool const ENABLE_MUTEX = true;
    createRotateFileLogger(LOGGER_NAME, PATH.toString(), MAX_SIZE, log::MakeType::RAW, ENABLE_MUTEX);
    auto * logger = log::getLogger(LOGGER_NAME);
    auto const * sink = logger->getSink();
    ASSERT_NE(nullptr, logger);
    ASSERT_NE(nullptr, sink);
    tLogM(LOGGER_NAME, message);

    auto const * rotate_sink = (log::sink::RotateFileSink<std::mutex> const *)sink;
    ASSERT_NE(nullptr, rotate_sink);
    filesystem::Path log_01 = rotate_sink->atRotatePath().getPath();
    ASSERT_EQ(MAX_SIZE, log_01.getState().size);
    std::cout << "Log file 01: " << log_01 << std::endl;

    tLogM(LOGGER_NAME, "1");
    tLogM(LOGGER_NAME, "2");
    filesystem::Path log_02 = rotate_sink->atRotatePath().getPath();
    ASSERT_EQ(2, log_02.getState().size);
    std::cout << "Log file 02: " << log_02 << std::endl;

    removeLogger(LOGGER_NAME);
}


