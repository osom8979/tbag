/**
 * @file   LogPrefTest.cpp
 * @brief  LogPref class tester.
 * @author zer0
 * @date   2020-02-24
 */

#include <gtest/gtest.h>
#include <libtbag/pref/details/LogPref.hpp>

using namespace libtbag;
using namespace libtbag::pref;
using namespace libtbag::pref::details;

TEST(LogPrefTest, Default)
{
    TBAG_CONSTEXPR static char const * const TEST_XML = R"__xml__(
        <loggers>
          <logger>
            <name>test</name>
            <sink>file</sink>
            <arguments>out.log</arguments>
            <generator>default</generator>
            <severity>debug</severity>
            <auto_flush>true</auto_flush>
          </logger>
        </loggers>)__xml__";

    LogPref pref;

    ASSERT_TRUE(pref.readFromXmlText(TEST_XML));
    ASSERT_EQ(1, pref.list().size());

    auto const & logger = pref.list()[0];
    ASSERT_STREQ("test", logger.name.c_str());
    ASSERT_STREQ("file", logger.sink.c_str());
    ASSERT_STREQ("out.log", logger.arguments.c_str());
    ASSERT_STREQ("default", logger.generator.c_str());
    ASSERT_STREQ("debug", logger.severity.c_str());
    ASSERT_STREQ("true", logger.auto_flush.c_str());
}

