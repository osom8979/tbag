/**
 * @file   MainArgumentsTest.cpp
 * @brief  MainArguments class tester.
 * @author zer0
 * @date   2019-05-22
 */

#include <gtest/gtest.h>
#include <libtbag/string/MainArguments.hpp>

#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::string;

TEST(MainArgumentsTest, Default)
{
    std::string program = "PROGRAM";
    std::vector<std::string> args = {"--help", "--verbose", "--version"};
    std::vector<std::string> envs = {"PATH=/bin", "USER=UNKNOWN"};
    MainArguments main_args(program, args, envs);

    ASSERT_EQ(4, main_args.argc());
    ASSERT_STREQ(program.c_str(), main_args.argv()[0]);
    ASSERT_STREQ(args[0].c_str(), main_args.argv()[1]);
    ASSERT_STREQ(args[1].c_str(), main_args.argv()[2]);
    ASSERT_STREQ(args[2].c_str(), main_args.argv()[3]);

    ASSERT_EQ(2, main_args.envc());
    ASSERT_STREQ(envs[0].c_str(), main_args.envs()[0]);
    ASSERT_STREQ(envs[1].c_str(), main_args.envs()[1]);
}

TEST(MainArgumentsTest, Default_NoProgram)
{
    std::vector<std::string> args = {"--help", "--verbose"};
    std::vector<std::string> envs = {"PATH=/bin"};
    MainArguments main_args(args, envs);

    ASSERT_EQ(2, main_args.argc());
    ASSERT_STREQ(args[0].c_str(), main_args.argv()[0]);
    ASSERT_STREQ(args[1].c_str(), main_args.argv()[1]);

    ASSERT_EQ(1, main_args.envc());
    ASSERT_STREQ(envs[0].c_str(), main_args.envs()[0]);
}

