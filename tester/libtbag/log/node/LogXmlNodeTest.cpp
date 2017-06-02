/**
 * @file   LogXmlNodeTest.cpp
 * @brief  LogXmlNode class tester.
 * @author zer0
 * @date   2017-04-29
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/dom/XmlModel.hpp>

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

    ASSERT_EQ(S_OFF      , LogXmlNode::parseSeverity(S_OFF      .getText()));
    ASSERT_EQ(S_EMERGENCY, LogXmlNode::parseSeverity(S_EMERGENCY.getText()));
    ASSERT_EQ(S_ALERT    , LogXmlNode::parseSeverity(S_ALERT    .getText()));
    ASSERT_EQ(S_CRITICAL , LogXmlNode::parseSeverity(S_CRITICAL .getText()));
    ASSERT_EQ(S_ERROR    , LogXmlNode::parseSeverity(S_ERROR    .getText()));
    ASSERT_EQ(S_WARNING  , LogXmlNode::parseSeverity(S_WARNING  .getText()));
    ASSERT_EQ(S_MOTICE   , LogXmlNode::parseSeverity(S_MOTICE   .getText()));
    ASSERT_EQ(S_INFO     , LogXmlNode::parseSeverity(S_INFO     .getText()));
    ASSERT_EQ(S_DEBUG    , LogXmlNode::parseSeverity(S_DEBUG    .getText()));
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

struct LogXmlNodeTest : public LogXmlNode
{
    TBAG_CONSTEXPR static char const * const ENV_TEST_DIR  = "TEST_DIR";
    TBAG_CONSTEXPR static char const * const TEST_LOG_NAME = "LogXmlNodeTest";

    LogXmlNodeTest()
    {
        // EMPTY.
    }

    LogXmlNodeTest(String const & test_dir)
    {
        atEnvs().push(LogXmlNode::EnvFlag(ENV_TEST_DIR, test_dir));
    }

    virtual String name() const override
    {
        return TEST_LOG_NAME;
    }

    virtual void setup() override
    {
        atInfos().clear();
        atInfos().push_back(getLogInfo(
                TEST_LOG_NAME, "file", "${TEST_DIR}/test.log",
                "true", "false", "false", "info", "default"));
    }
};

TEST(LogXmlNodeTest, Default)
{
    tttDir(true, true);
    auto const CONFIG_PATH  = tttDirGet() / "config.xml";
    auto const LOGFILE_PATH = tttDirGet() / "test.log";

    {   // Create xml file.
        using namespace libtbag::dom;
        XmlModel model;
        model.add(XmlModel::SharedNode(new LogXmlNodeTest(tttDirGet().getString())));
        model.setup();
        ASSERT_TRUE(model.save(CONFIG_PATH));
    }
    ASSERT_TRUE(CONFIG_PATH.exists());

    // Read xml file.
    using namespace libtbag::dom;
    XmlModel model;
    model.add(XmlModel::SharedNode(new LogXmlNodeTest(tttDirGet().getString())));
    ASSERT_TRUE(model.load(CONFIG_PATH));

    LogXmlNodeTest * node = model.getPointer<LogXmlNodeTest>();
    ASSERT_NE(nullptr, node);
    ASSERT_EQ(1, node->atInfos().size());
    ASSERT_STREQ(LogXmlNodeTest::TEST_LOG_NAME, node->atInfos()[0].name.c_str());

    ASSERT_EQ(nullptr, log::getLogger(LogXmlNodeTest::TEST_LOG_NAME));
    ASSERT_EQ(1, node->createLoggers());
    ASSERT_NE(nullptr, log::getLogger(LogXmlNodeTest::TEST_LOG_NAME));

    ASSERT_EQ(0, LOGFILE_PATH.getState().size);
    tLogD(LogXmlNodeTest::TEST_LOG_NAME, "Default");
    ASSERT_EQ(0, LOGFILE_PATH.getState().size);

    tLogI(LogXmlNodeTest::TEST_LOG_NAME, "Default");
    ASSERT_LT(0, LOGFILE_PATH.getState().size);
}

