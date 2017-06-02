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

