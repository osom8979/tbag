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
    ASSERT_EQ(10U, result.size());
    ASSERT_STREQ(                  "temp", result[0].c_str());
    ASSERT_STREQ(           "--arg1=temp", result[1].c_str());
    ASSERT_STREQ(     "--arg2=space temp", result[2].c_str());
    ASSERT_STREQ("--arg3=\"value quoting", result[3].c_str());
    ASSERT_STREQ( "--arg4='single quotes", result[4].c_str());
    ASSERT_STREQ(                     "1", result[5].c_str());
    ASSERT_STREQ(                     "2", result[6].c_str());
    ASSERT_STREQ(                     "3", result[7].c_str());
    ASSERT_STREQ(                "noarg1", result[8].c_str());
    ASSERT_STREQ(                "noarg2", result[9].c_str());
}

TEST(FlagsTest, convertFlag)
{
    auto t1 = Flags::convertFlag("--arg1=temp");
    ASSERT_STREQ("arg1", t1. first.c_str());
    ASSERT_STREQ("temp", t1.second.c_str());

    auto t2 = Flags::convertFlag("--arg1_temp");
    ASSERT_STREQ("arg1_temp", t2. first.c_str());
    ASSERT_STREQ(         "", t2.second.c_str());

    auto t3 = Flags::convertFlag("value=temp");
    ASSERT_STREQ(          "", t3. first.c_str());
    ASSERT_STREQ("value=temp", t3.second.c_str());
}

TEST(FlagsTest, at)
{
    Flags flags;
    flags.push(std::make_pair("arg1", "temp1"));
    flags.push(std::make_pair("arg2", "temp2"));

    Flags const CONST_FLAGS = flags;

    ASSERT_EQ(2U, CONST_FLAGS.size());
    ASSERT_STREQ( "arg1", CONST_FLAGS.at(0). first.c_str());
    ASSERT_STREQ("temp1", CONST_FLAGS.at(0).second.c_str());
    ASSERT_STREQ( "arg2", CONST_FLAGS.at(1). first.c_str());
    ASSERT_STREQ("temp2", CONST_FLAGS.at(1).second.c_str());
}

TEST(FlagsTest, parse_and_find)
{
    std::string args = "temp --arg1 --arg2=temp";
    Flags flags;
    flags.parse(args);

    ASSERT_EQ(3U, flags.size());

    auto flag1 = flags.findWithKey("temp");
    ASSERT_STREQ("", flag1. first.c_str());
    ASSERT_STREQ("", flag1.second.c_str());

    auto flag2 = flags.findWithKey("arg1");
    ASSERT_STREQ("arg1", flag2. first.c_str());
    ASSERT_STREQ(    "", flag2.second.c_str());

    auto flag3 = flags.findWithKey("arg2");
    ASSERT_STREQ("arg2", flag3. first.c_str());
    ASSERT_STREQ("temp", flag3.second.c_str());

    auto flag4 = flags.findWithValue("temp");
    ASSERT_STREQ(    "", flag4. first.c_str());
    ASSERT_STREQ("temp", flag4.second.c_str());
}

