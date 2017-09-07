/**
 * @file   HelpCommanderTest.cpp
 * @brief  HelpCommander class tester.
 * @author zer0
 * @date   2017-06-02
 */

#include <gtest/gtest.h>
#include <libtbag/string/HelpCommander.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(HelpCommanderTest, Default)
{
    HelpCommander cmd;
    ASSERT_TRUE(cmd.insert("test", [&](Arguments const & args){ /* EMPTY. */ }, "help"));
    ASSERT_EQ(1U, cmd.size());
    ASSERT_EQ(1U, cmd.getHelps().size());
    ASSERT_EQ(0U, cmd.getParams().size());

    cmd.clear();
    ASSERT_EQ(0U, cmd.size());
    ASSERT_EQ(0U, cmd.getHelps().size());
    ASSERT_EQ(0U, cmd.getParams().size());

    ASSERT_TRUE(cmd.insert("test", [&](Arguments const & args){ /* EMPTY. */ }, "help", "param"));
    ASSERT_EQ(1U, cmd.size());
    ASSERT_EQ(1U, cmd.getHelps().size());
    ASSERT_EQ(1U, cmd.getParams().size());

    cmd.clear();
    ASSERT_EQ(0U, cmd.size());
    ASSERT_EQ(0U, cmd.getHelps().size());
    ASSERT_EQ(0U, cmd.getParams().size());
}

TEST(HelpCommanderTest, MacroExample)
{
    std::string const TEST_PREFIX    = "/";
    std::string const TEST_DELIMITER = "=";
    std::string const TEST_SYNOPSIS  = "Synopsis";
    std::string const TEST_REMARKS   = "Remarks";

    HelpCommander cmd;
    cmd.info(TEST_PREFIX, TEST_DELIMITER, TEST_SYNOPSIS, TEST_REMARKS);
    ASSERT_EQ(TEST_PREFIX   , cmd.getPrefix());
    ASSERT_EQ(TEST_DELIMITER, cmd.getDelimiter());
    ASSERT_EQ(TEST_SYNOPSIS , cmd.getSynopsis());
    ASSERT_EQ(TEST_REMARKS  , cmd.getRemarks());

    bool value1 = false;
    int  value2 = 0;
    long value3 = 0;

    cmd.insertDefault("v1", &value1, false, "value1_help");
    cmd.insertDefault("v2", &value2, 10000, "value2_help");
    cmd.insertDefault("v3", &value3, 99999, "value3_help");
    cmd.request("/v1=On /v2");

    ASSERT_EQ( true, value1);
    ASSERT_EQ(10000, value2);
    ASSERT_EQ(    0, value3);
}

