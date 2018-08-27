/**
 * @file   RotateFileLogTest.cpp
 * @brief  Log Test of RotateFileSink class.
 * @author zer0
 * @date   2018-08-27
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/Unit.hpp>

#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::node;

using Path = libtbag::filesystem::Path;

// ---------------
namespace __impl {
// ---------------

static std::vector<Path> ignoreHistoryPath(std::vector<Path> const & scans)
{
    using namespace libtbag::string;
    using namespace libtbag::filesystem;
    using namespace libtbag::log::sink;

    auto const MATCH_REGEX = replaceRegex(RotateFileSink<>::HISTORY_SUFFIX, R"(\.)", R"(.*\.)") + "$";
    std::vector<Path> result;
    for (auto & path : scans) {
        if (isMatch(path.toString(), MATCH_REGEX) == false) {
            result.push_back(path);
        }
    }
    return result;
}

// ------------------
} // namespace __impl
// ------------------

TEST(RotateFileLogTest, Default)
{
    tttDir(true, true);
    auto const PATH = tttDir_Get() / "rotate.log";
    std::string const LOGGER_NAME = "LogTest.RotateFileSink";
    std::size_t const MAX_SIZE = 128;
    std::size_t const MAX_FILE_COUNT = 2;

    std::string message1(MAX_SIZE, '1');
    std::string message2(MAX_SIZE, '2');
    ASSERT_EQ(MAX_SIZE, message1.size());
    ASSERT_EQ(MAX_SIZE, message2.size());

    bool const ENABLE_MUTEX = true;
    createRotateFileLogger(LOGGER_NAME, PATH.toString(), MAX_SIZE, MAX_FILE_COUNT, log::MakeType::RAW, ENABLE_MUTEX);
    auto * logger = log::getLogger(LOGGER_NAME);
    auto const * sink = logger->getSink();
    ASSERT_NE(nullptr, logger);
    ASSERT_NE(nullptr, sink);

    auto const * rotate_sink = (log::sink::RotateFileSink<std::mutex> const *)sink;
    ASSERT_NE(nullptr, rotate_sink);
    ASSERT_EQ(1, rotate_sink->atHistory().size());

    tLogM(LOGGER_NAME, message1);
    filesystem::Path log_01 = rotate_sink->atRotatePath().getPath();
    ASSERT_EQ(MAX_SIZE, log_01.getState().size);
    ASSERT_EQ(1, rotate_sink->atHistory().size());
    ASSERT_EQ(log_01.toString(), rotate_sink->atHistory().front().toString());
    std::cout << "Log file 01: " << log_01 << std::endl;

    tLogM(LOGGER_NAME, message2);
    filesystem::Path log_02 = rotate_sink->atRotatePath().getPath();
    ASSERT_EQ(MAX_SIZE, log_02.getState().size);
    ASSERT_EQ(2, rotate_sink->atHistory().size());
    ASSERT_EQ(log_01.toString(), rotate_sink->atHistory().front().toString());
    std::cout << "Log file 02: " << log_02 << std::endl;

    tLogM(LOGGER_NAME, "1");
    tLogM(LOGGER_NAME, "2");
    filesystem::Path log_03 = rotate_sink->atRotatePath().getPath();
    ASSERT_EQ(2, log_03.getState().size);
    ASSERT_EQ(2, rotate_sink->atHistory().size());
    ASSERT_EQ(log_02.toString(), rotate_sink->atHistory().front().toString());
    std::cout << "Log file 03: " << log_03 << std::endl;

    removeLogger(LOGGER_NAME);
}

TEST(RotateFileLogTest, LoadXml)
{
    tttDir(true, true);
    auto const DIR = tttDir_Get();
    auto const PATH = DIR / "rotate.log";
    std::string const LOGGER_NAME = "RotateFileLogTest.LoadXml";
    int const MAX_FILE_COUNT = 10;

    char const * const XML_STRING0 = R"XML(
        <tlog>
            <logger>
                <name>@NAME@</name>
                <sink>rotate_file</sink>
                <destination>@DESTINATION@</destination>
                <max_size>1048576</max_size>
                <max_file_count>@MAX_FILE_COUNT@</max_file_count>
                <auto_flush>1</auto_flush>
                <multithread>0</multithread>
                <mutex>1</mutex>
                <severity>INFO</severity>
                <generator>raw</generator>
            </logger>
        </tlog>)XML";

    using namespace libtbag::string;
    auto const XML_STRING1 = replaceRegex(XML_STRING0, R"(@NAME@)", LOGGER_NAME);
    auto const XML_STRING2 = replaceRegex(XML_STRING1, R"(@DESTINATION@)", PATH.toString());
    auto const XML_STRING3 = replaceRegex(XML_STRING2, R"(@MAX_FILE_COUNT@)", libtbag::string::toString(MAX_FILE_COUNT));
    ASSERT_EQ(1, createLoggerWithXmlString(XML_STRING3));

    auto * logger = log::getLogger(LOGGER_NAME);
    ASSERT_NE(nullptr, logger);
    ASSERT_EQ(log::INFO_SEVERITY, logger->getSeverity());

    auto * sink = logger->getSink();
    ASSERT_NE(nullptr, sink);

    auto const max_history_val = sink->get(libtbag::log::sink::RotateFileSink<>::MAX_HISTORY_KEY);
    ASSERT_EQ(MAX_FILE_COUNT, libtbag::string::toValue<int>(max_history_val));

    char const MESSAGE_01_CHAR = '1';
    auto const MESSAGE_01_1024 = std::string(1024, MESSAGE_01_CHAR);
    for (int i = 0; i < 1024; ++i) {
        tLogM(LOGGER_NAME, "{}", MESSAGE_01_1024); // 1KB * 1024 = 1MB
    }
    ASSERT_EQ(1, __impl::ignoreHistoryPath(DIR.scanDir()).size());

    char const MESSAGE_02_CHAR = '2';
    auto const MESSAGE_02_1024 = std::string(1024, MESSAGE_02_CHAR);
    for (int i = 0; i < 1024; ++i) {
        tLogM(LOGGER_NAME, "{}", MESSAGE_02_1024); // 1KB * 1024 = 1MB
    }
    ASSERT_EQ(2, __impl::ignoreHistoryPath(DIR.scanDir()).size());

    char const MESSAGE_03_CHAR = '3';
    auto const MESSAGE_03_1024 = std::string(1024, MESSAGE_03_CHAR);
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 1024; ++i) {
            tLogM(LOGGER_NAME, "{}", MESSAGE_03_1024); // 1KB * 1024 = 1MB
        }
    }

    auto first_scane = __impl::ignoreHistoryPath(DIR.scanDir());
    ASSERT_EQ(10, first_scane.size());
    for (auto & file : first_scane) {
        ASSERT_EQ(libtbag::MEGA_BYTE_TO_BYTE, file.getState().size);
    }

    tLogM(LOGGER_NAME, "!");

    auto second_scane = __impl::ignoreHistoryPath(DIR.scanDir());
    ASSERT_EQ(10, second_scane.size());

    using namespace libtbag::filesystem;
    std::sort(second_scane.begin(), second_scane.end(), [](Path const & p1, Path const & p2) -> bool {
        return p1.toString().compare(p2.toString()) < 0;
    });

    auto & oldest_file = second_scane.front();
    auto & newest_file = second_scane.back();
    ASSERT_EQ(libtbag::MEGA_BYTE_TO_BYTE, oldest_file.getState().size);
    ASSERT_EQ(1, second_scane.back().getState().size);

    std::string content;
    ASSERT_EQ(Err::E_SUCCESS, libtbag::filesystem::readFile(oldest_file, content));
    ASSERT_EQ(MESSAGE_02_CHAR, content[0]);

    removeLogger(LOGGER_NAME);
}

TEST(RotateFileLogTest, HistoryFile)
{
    tttDir(true, true);
    auto const DIR = tttDir_Get();
    auto const PATH = DIR / "rotate.log";
    std::string const LOGGER_NAME = "RotateFileLogTest.HistoryFile";
    int const MAX_FILE_COUNT = 2;

    char const * const XML_STRING0 = R"XML(
        <tlog>
            <logger>
                <name>@NAME@</name>
                <sink>rotate_file</sink>
                <destination>@DESTINATION@</destination>
                <max_size>1024</max_size>
                <max_file_count>@MAX_FILE_COUNT@</max_file_count>
                <auto_flush>1</auto_flush>
                <multithread>0</multithread>
                <mutex>1</mutex>
                <severity>INFO</severity>
                <generator>raw</generator>
            </logger>
        </tlog>)XML";

    using namespace libtbag::string;
    auto const XML_STRING1 = replaceRegex(XML_STRING0, R"(@NAME@)", LOGGER_NAME);
    auto const XML_STRING2 = replaceRegex(XML_STRING1, R"(@DESTINATION@)", PATH.toString());
    auto const XML_STRING3 = replaceRegex(XML_STRING2, R"(@MAX_FILE_COUNT@)", libtbag::string::toString(MAX_FILE_COUNT));
    ASSERT_EQ(1, createLoggerWithXmlString(XML_STRING3));

    auto * logger = log::getLogger(LOGGER_NAME);
    ASSERT_NE(nullptr, logger);
    ASSERT_EQ(log::INFO_SEVERITY, logger->getSeverity());

    auto * sink = logger->getSink();
    ASSERT_NE(nullptr, sink);

    auto const max_history_val = sink->get(libtbag::log::sink::RotateFileSink<>::MAX_HISTORY_KEY);
    ASSERT_EQ(MAX_FILE_COUNT, libtbag::string::toValue<int>(max_history_val));

    char const MESSAGE_CHAR = '1';
    auto const MESSAGE_1024 = std::string(1024, MESSAGE_CHAR);
    tLogM(LOGGER_NAME, "{}", MESSAGE_1024); // 1KB
    ASSERT_EQ(2, DIR.scanDir().size()); // history & log file

    tLogM(LOGGER_NAME, "{}", MESSAGE_1024); // 1KB
    ASSERT_EQ(3, DIR.scanDir().size()); // history & two log file

    tLogM(LOGGER_NAME, "{}", MESSAGE_1024); // 1KB
    ASSERT_EQ(3, DIR.scanDir().size()); // history & two log file (remove oldest)

    removeLogger(LOGGER_NAME);
    logger = log::getLogger(LOGGER_NAME);
    ASSERT_EQ(nullptr, logger);

    COMMENT("NEXT, RELOAD LOGGER") {
        ASSERT_EQ(1, createLoggerWithXmlString(XML_STRING3));
        ASSERT_NE(nullptr, log::getLogger(LOGGER_NAME));
        ASSERT_EQ(3, DIR.scanDir().size()); // Previous state.

        tLogM(LOGGER_NAME, "{}", MESSAGE_1024); // 1KB
        ASSERT_EQ(3, DIR.scanDir().size()); // history & two log file (remove oldest)

        tLogM(LOGGER_NAME, "{}", MESSAGE_1024); // 1KB
        ASSERT_EQ(3, DIR.scanDir().size()); // history & two log file (remove oldest)

        removeLogger(LOGGER_NAME);
    }
}

