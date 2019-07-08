/**
 * @file   CommanderTest.cpp
 * @brief  Commander class tester.
 * @author zer0
 * @date   2016-10-01
 */

#include <gtest/gtest.h>
#include <libtbag/string/Commander.hpp>

#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::string;

TEST(CommanderTest, OnlyCoverage)
{
    Commander commander;
    commander.clear();
    commander.empty();
    commander.size();
}

TEST(CommanderTest, Default)
{
    std::string default_result;
    std::string test1_result;
    std::string test2_result;

    Commander commander([&](Arguments const & args){
        default_result = args.get(0);
    });
    commander.insert("test1", [&](Arguments const & args){
        test1_result = args.get(0);
    });
    commander.insert("test2", [&](Arguments const & args){
        test2_result = args.get(1);
    });

    commander.request(" --test2=1,temp --test1=result default");
    ASSERT_STREQ("default", default_result.c_str());
    ASSERT_STREQ( "result", test1_result.c_str());
    ASSERT_STREQ(   "temp", test2_result.c_str());
}

TEST(CommanderTest, OnlyPrefix)
{
    std::vector<std::string> args_result;

    Commander commander([&](Arguments const & args){
        if (!args.empty()) {
            args_result.push_back(args.get(0));
        }
    });

    commander.request("-- --t");
    ASSERT_EQ(1, args_result.size());
    ASSERT_STREQ("--", args_result[0].c_str());
}

TEST(CommanderTest, WindowsPath)
{
    std::string s_val;
    std::string c_val;

    auto commander = Commander("", "=");
    commander.insert("s", [&](Arguments const & args){
        s_val = args.toString();
    });
    commander.insert("c", [&](Arguments const & args){
        c_val = args.toString();
    });
    ASSERT_EQ(2, commander.request("u=a \"c=c:\\\\Program Files\\\\Test File\" s=m"));
    ASSERT_STREQ("m", s_val.c_str());
    ASSERT_STREQ("c:\\Program Files\\Test File", c_val.c_str());
}

