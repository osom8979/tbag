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
#include <libtbag/dom/xml/XmlModel.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::node;

TEST(LogXmlNodeTest, XmlString)
{
    char const * const TEST_XML_STRING = R"XML(
        <loggers>
            <logger>
                <name>test-logger-cout</name>
                <sink>console</sink>
                <arguments>stdout</arguments>
                <generator>default_color</generator>
                <severity>debug</severity>
                <auto_flush>false</auto_flush>
            </logger>
            <logger>
                <name>test-logger-file</name>
                <sink>file</sink>
                <arguments>${EXE_DIR}/tbag-logger-test.log</arguments>
                <generator>default</generator>
                <severity>critical</severity>
                <auto_flush>true</auto_flush>
            </logger>
        </loggers>)XML";

    char const * const COUT_LOGGER = "test-logger-cout";
    char const * const FILE_LOGGER = "test-logger-file";

    ASSERT_EQ(2, createLoggerWithXmlText(TEST_XML_STRING));
    auto * cout_logger = getLogger(COUT_LOGGER);
    auto * file_logger = getLogger(FILE_LOGGER);

    ASSERT_NE(nullptr, cout_logger);
    ASSERT_NE(nullptr, file_logger);

    auto * unknown_logger = getLogger("");
    ASSERT_EQ(nullptr, unknown_logger);

    ASSERT_EQ(DEBUG_SEVERITY, cout_logger->getSeverity());
    ASSERT_EQ(CRITICAL_SEVERITY, file_logger->getSeverity());

    ASSERT_FALSE(cout_logger->getWeakedSink().expired());
    ASSERT_FALSE(file_logger->getWeakedSink().expired());

    ASSERT_TRUE(removeLogger(COUT_LOGGER));
    ASSERT_TRUE(removeLogger(FILE_LOGGER));
}

TEST(LogXmlNodeTest, RecreateIfExists)
{
    char const * const TEST_XML_STRING = R"XML(
        <loggers>
            <logger>
                <name>test-logger-cout</name>
                <sink>console</sink>
                <arguments>stdout</arguments>
                <generator>default_color</generator>
                <severity>OFF</severity>
                <auto_flush>false</auto_flush>
            </logger>
        </loggers>)XML";

    char const * const COUT_LOGGER = "test-logger-cout";
    ASSERT_NE(nullptr, createStdoutLogger(COUT_LOGGER));
    setSeverity(COUT_LOGGER, WARNING_SEVERITY);

    Logger * logger = getLogger(COUT_LOGGER);
    ASSERT_NE(nullptr, logger);
    ASSERT_EQ(WARNING_SEVERITY, logger->getSeverity());

    ASSERT_EQ(1, createLoggerWithXmlText(TEST_XML_STRING));
    logger = getLogger(COUT_LOGGER);
    ASSERT_NE(nullptr, logger);
    ASSERT_EQ(OFF_SEVERITY, logger->getSeverity());

    ASSERT_TRUE(removeLogger(COUT_LOGGER));
}

struct LogXmlNodeTest : public LogXmlNode
{
    TBAG_CONSTEXPR static char const * const ENV_TEST_DIR  = "TEST_DIR";
    TBAG_CONSTEXPR static char const * const TEST_LOG_NAME = "LogXmlNodeTest";

    LogXmlNodeTest()
    {
        // EMPTY.
    }

    LogXmlNodeTest(std::string const & test_dir)
    {
        envs().push(ENV_TEST_DIR, test_dir);
    }

    virtual std::string name() const override
    {
        return TEST_LOG_NAME;
    }

    virtual void setup() override
    {
        Info param;
        // clang-format off
        param.name        = TEST_LOG_NAME;
        param.sink        = "file";
        param.arguments   = "${TEST_DIR}/test.log";
        param.generator   = "default";
        param.line_feed   = "unix";
        param.severity    = "info";
        param.auto_flush  = "true";
        // clang-format on

        infos().clear();
        infos().push_back(param);
    }
};

TEST(LogXmlNodeTest, Default)
{
    tttDir_Automatic();
    auto const CONFIG_PATH  = tttDir_Get() / "config.xml";
    auto const LOGFILE_PATH = tttDir_Get() / "test.log";

    {   // Create xml file.
        using namespace libtbag::dom::xml;
        XmlModel model;
        model.add(XmlModel::SharedNode(new LogXmlNodeTest(tttDir_Get().getString())));
        model.setup();
        ASSERT_TRUE(model.save(CONFIG_PATH));
    }
    ASSERT_TRUE(CONFIG_PATH.exists());

    // Read xml file.
    using namespace libtbag::dom::xml;
    XmlModel model;
    model.add(XmlModel::SharedNode(new LogXmlNodeTest(tttDir_Get().getString())));
    ASSERT_TRUE(model.load(CONFIG_PATH));

    LogXmlNodeTest * node = model.getPointer<LogXmlNodeTest>();
    ASSERT_NE(nullptr, node);
    ASSERT_EQ(1, node->infos().size());
    ASSERT_STREQ(LogXmlNodeTest::TEST_LOG_NAME, node->infos()[0].name.c_str());

    ASSERT_EQ(nullptr, log::getLogger(LogXmlNodeTest::TEST_LOG_NAME));
    ASSERT_EQ(1, node->createLoggers());
    ASSERT_NE(nullptr, log::getLogger(LogXmlNodeTest::TEST_LOG_NAME));

    ASSERT_EQ(0, LOGFILE_PATH.getState().size);
    tLogD(LogXmlNodeTest::TEST_LOG_NAME, "Default");
    ASSERT_EQ(0, LOGFILE_PATH.getState().size);

    tLogI(LogXmlNodeTest::TEST_LOG_NAME, "Default");
    ASSERT_LT(0, LOGFILE_PATH.getState().size);
}

