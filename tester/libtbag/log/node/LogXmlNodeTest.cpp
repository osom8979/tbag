/**
 * @file   LogXmlNodeTest.cpp
 * @brief  LogXmlNode class tester.
 * @author zer0
 * @date   2017-04-29
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::node;

TEST(LogXmlNodeTest, Parse)
{
    ASSERT_TRUE(LogXmlNode::parseAutoFlush(LogXmlNode::AUTO_FLUSH_ON));
    ASSERT_FALSE(LogXmlNode::parseAutoFlush(LogXmlNode::AUTO_FLUSH_OFF));
    ASSERT_FALSE(LogXmlNode::parseAutoFlush(""));

    ASSERT_TRUE(LogXmlNode::parseMultiThread(LogXmlNode::MULTITHREAD_ON));
    ASSERT_FALSE(LogXmlNode::parseMultiThread(LogXmlNode::MULTITHREAD_OFF));
    ASSERT_FALSE(LogXmlNode::parseMultiThread(""));

    ASSERT_TRUE(LogXmlNode::parseMutex(LogXmlNode::MUTEX_ON));
    ASSERT_FALSE(LogXmlNode::parseMutex(LogXmlNode::MUTEX_OFF));
    ASSERT_FALSE(LogXmlNode::parseMutex(""));

    static Severity const S_OFF       = libtbag::log::level::          OFF_SEVERITY;
    static Severity const S_EMERGENCY = libtbag::log::level::    EMERGENCY_SEVERITY;
    static Severity const S_ALERT     = libtbag::log::level::        ALERT_SEVERITY;
    static Severity const S_CRITICAL  = libtbag::log::level::     CRITICAL_SEVERITY;
    static Severity const S_ERROR     = libtbag::log::level::        ERROR_SEVERITY;
    static Severity const S_WARNING   = libtbag::log::level::      WARNING_SEVERITY;
    static Severity const S_MOTICE    = libtbag::log::level::       NOTICE_SEVERITY;
    static Severity const S_INFO      = libtbag::log::level::INFORMATIONAL_SEVERITY;
    static Severity const S_DEBUG     = libtbag::log::level::        DEBUG_SEVERITY;

    ASSERT_EQ(S_OFF      , LogXmlNode::parseSeverity(S_OFF      .text));
    ASSERT_EQ(S_EMERGENCY, LogXmlNode::parseSeverity(S_EMERGENCY.text));
    ASSERT_EQ(S_ALERT    , LogXmlNode::parseSeverity(S_ALERT    .text));
    ASSERT_EQ(S_CRITICAL , LogXmlNode::parseSeverity(S_CRITICAL .text));
    ASSERT_EQ(S_ERROR    , LogXmlNode::parseSeverity(S_ERROR    .text));
    ASSERT_EQ(S_WARNING  , LogXmlNode::parseSeverity(S_WARNING  .text));
    ASSERT_EQ(S_MOTICE   , LogXmlNode::parseSeverity(S_MOTICE   .text));
    ASSERT_EQ(S_INFO     , LogXmlNode::parseSeverity(S_INFO     .text));
    ASSERT_EQ(S_DEBUG    , LogXmlNode::parseSeverity(S_DEBUG    .text));
    ASSERT_EQ(S_OFF      , LogXmlNode::parseSeverity(""));

    ASSERT_TRUE(MakeType::DEFAULT       == LogXmlNode::parseGeneratorType(LogXmlNode::GENERATOR_DEFAULT));
    ASSERT_TRUE(MakeType::DEFAULT_COLOR == LogXmlNode::parseGeneratorType(LogXmlNode::GENERATOR_DEFAULT_COLOR));
    ASSERT_TRUE(MakeType::DEFAULT       == LogXmlNode::parseGeneratorType(""));
}

TBAG_CONSTEXPR static char const * const TEST_TLOG_XML_STRING = R"XML(
<tlog>
    <logger>
        <name>test-logger-cout</name>
        <sink>cout</sink>
        <destination></destination>
        <multithread>false</multithread>
        <mutex>true</mutex>
        <generator>default_color</generator>
        <severity>debug</severity>
        <auto_flush>false</auto_flush>
    </logger>
    <logger>
        <name>test-logger-file</name>
        <sink>file</sink>
        <destination>tbag-logger-test.log</destination>
        <multithread>false</multithread>
        <mutex>false</mutex>
        <generator>default</generator>
        <severity>critical</severity>
        <auto_flush>true</auto_flush>
    </logger>
</tlog>
)XML";

TEST(LogXmlNodeTest, XmlString)
{
    char const * const COUT_LOGGER = "test-logger-cout";
    char const * const FILE_LOGGER = "test-logger-file";

    ASSERT_EQ(2, createLoggerWithXmlString(TEST_TLOG_XML_STRING));
    auto * cout_logger = getLogger(COUT_LOGGER);
    auto * file_logger = getLogger(FILE_LOGGER);

    ASSERT_NE(nullptr, cout_logger);
    ASSERT_NE(nullptr, file_logger);

    auto * unknown_logger = getLogger("");
    ASSERT_EQ(nullptr, unknown_logger);

    ASSERT_EQ(DEBUG_SEVERITY, cout_logger->getSeverity());
    ASSERT_EQ(CRITICAL_SEVERITY, file_logger->getSeverity());

    ASSERT_TRUE(Logger::Generator::MakeType::DEFAULT_COLOR == cout_logger->getGeneratorMakeType());
    ASSERT_TRUE(Logger::Generator::MakeType::DEFAULT == file_logger->getGeneratorMakeType());

    ASSERT_NE(nullptr, cout_logger->getSink());
    ASSERT_NE(nullptr, file_logger->getSink());

    ASSERT_TRUE(removeLogger(COUT_LOGGER));
    ASSERT_TRUE(removeLogger(FILE_LOGGER));
}

