/**
 * @file   LoggerTest.cpp
 * @brief  Logger class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/log/Logger.hpp>
#include <libtbag/log/msg/RawGenerator.hpp>
#include <libtbag/log/sink/FunctionalSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;
using namespace libtbag::log::sink;

TEST(LoggerTest, Default)
{
    auto gen = std::make_shared<RawGenerator>(RawGenerator::LineFeedStyle::LFS_NONE);
    ASSERT_TRUE(static_cast<bool>(gen));

    auto write_cb = [](int level, char const * msg, int size, void * user) -> bool {
        auto * buffer = (std::string*)user;
        buffer->assign(msg, msg + size);
        return true;
    };
    auto flush_cb = [](void * user){
        auto * buffer = (std::string*)user;
        buffer->assign("flush");
    };
    std::string buffer;
    auto sink = std::make_shared<FunctionalSink>(write_cb, flush_cb, &buffer);
    ASSERT_TRUE(static_cast<bool>(sink));

    char const * const TEST_LOGGER_NAME = "test";
    Logger logger(TEST_LOGGER_NAME, sink, gen, ERROR_LEVEL, false);
    ASSERT_STREQ(TEST_LOGGER_NAME, logger.NAME.c_str());

    ASSERT_TRUE(logger.format(DEBUG_SEVERITY, "debug"));
    ASSERT_TRUE(buffer.empty());

    ASSERT_TRUE(logger.format(ERROR_SEVERITY, "{}", "error"));
    ASSERT_STREQ("error", buffer.c_str());

    logger.flush();
    ASSERT_STREQ("flush", buffer.c_str());

    COMMENT("OFF Severity Testing") {
        logger.setSeverity(OFF_SEVERITY);
        buffer.clear();
        // clang-format off
        ASSERT_TRUE(logger.write(      OFF_SEVERITY, "0")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(EMERGENCY_SEVERITY, "1")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(    ALERT_SEVERITY, "2")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write( CRITICAL_SEVERITY, "3")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(    ERROR_SEVERITY, "4")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(  WARNING_SEVERITY, "5")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(   NOTICE_SEVERITY, "6")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(     INFO_SEVERITY, "7")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(    DEBUG_SEVERITY, "8")); ASSERT_TRUE(buffer.empty());
        ASSERT_TRUE(logger.write(  UNKNOWN_SEVERITY, "9")); ASSERT_TRUE(buffer.empty());
        // clang-format on
    }
}

