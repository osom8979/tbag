/**
 * @file   FlagsTest.cpp
 * @brief  Flags class tester.
 * @author zer0
 * @date   2016-05-03
 */

#include <gtest/gtest.h>
#include <libtbag/Flags.hpp>

using namespace libtbag;

TEST(FlagsTest, splitTokens)
{
    std::string args = "      temp                   "
                       "  --arg1=temp                "
                       "  \"--arg2=space temp\"      "
                       "  '--arg3=\"value quoting'   "
                       "  '--arg4=\\'single quotes'  "
                       "  '1''2''3'                  "
                       "  noarg1 noarg2              ";

    auto result = Flags::splitTokens(args);
    ASSERT_EQ(result.size(), 10U);
    ASSERT_STREQ(result[0].c_str(), "temp");
    ASSERT_STREQ(result[1].c_str(), "--arg1=temp");
    ASSERT_STREQ(result[2].c_str(), "--arg2=space temp");
    ASSERT_STREQ(result[3].c_str(), "--arg3=\"value quoting");
    ASSERT_STREQ(result[4].c_str(), "--arg4='single quotes");
    ASSERT_STREQ(result[5].c_str(), "1");
    ASSERT_STREQ(result[6].c_str(), "2");
    ASSERT_STREQ(result[7].c_str(), "3");
    ASSERT_STREQ(result[8].c_str(), "noarg1");
    ASSERT_STREQ(result[9].c_str(), "noarg2");
}

TEST(FlagsTest, convertFlag)
{
    auto t1 = Flags::convertFlag("--arg1=temp");
    ASSERT_STREQ(t1.first.c_str(), "arg1");
    ASSERT_STREQ(t1.second.c_str(), "temp");

    auto t2 = Flags::convertFlag("--arg1_temp");
    ASSERT_STREQ(t2.first.c_str(), "arg1_temp");
    ASSERT_STREQ(t2.second.c_str(), "");

    auto t3 = Flags::convertFlag("value=temp");
    ASSERT_STREQ(t3.first.c_str(), "");
    ASSERT_STREQ(t3.second.c_str(), "value=temp");
}

TEST(FlagsTest, parse_and_find)
{
    std::string args = "temp --arg1 --arg2=temp";
    Flags flags;
    flags.parse(args);

    ASSERT_EQ(flags.size(), 3U);

    auto flag1 = flags.findWithKey("temp");
    ASSERT_STREQ(flag1.first.c_str(), "");
    ASSERT_STREQ(flag1.second.c_str(), "");

    auto flag2 = flags.findWithKey("arg1");
    ASSERT_STREQ(flag2.first.c_str(), "arg1");
    ASSERT_STREQ(flag2.second.c_str(), "");

    auto flag3 = flags.findWithKey("arg2");
    ASSERT_STREQ(flag3.first.c_str(), "arg2");
    ASSERT_STREQ(flag3.second.c_str(), "temp");

    auto flag4 = flags.findWithValue("temp");
    ASSERT_STREQ(flag4.first.c_str(), "");
    ASSERT_STREQ(flag4.second.c_str(), "temp");
}

