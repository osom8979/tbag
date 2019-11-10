/**
 * @file   ArgvTest.cpp
 * @brief  Argv class tester.
 * @author zer0
 * @date   2019-11-10
 */

#include <gtest/gtest.h>
#include <libtbag/string/Argv.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ArgvTest, Argument_01)
{
    std::vector<std::string> test = {"program", "--verbose", "--version"};
    Argv args;
    args.parse(test);

    ASSERT_EQ(3, args.argc());
    ASSERT_STREQ(test[0].c_str(), args.argv()[0]);
    ASSERT_STREQ(test[1].c_str(), args.argv()[1]);
    ASSERT_STREQ(test[2].c_str(), args.argv()[2]);
    ASSERT_EQ(nullptr, args.argv()[3]);
}

TEST(ArgvTest, Argument_02)
{
    std::vector<std::string> test = {"PATH=/bin", "USER=UNKNOWN"};
    Argv args;
    args.parse(test);

    ASSERT_EQ(2, args.argc());
    ASSERT_STREQ(test[0].c_str(), args.argv()[0]);
    ASSERT_STREQ(test[1].c_str(), args.argv()[1]);
    ASSERT_EQ(nullptr, args.argv()[2]);
}

