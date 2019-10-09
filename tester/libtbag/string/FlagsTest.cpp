/**
 * @file   FlagsTest.cpp
 * @brief  Flags class tester.
 * @author zer0
 * @date   2016-05-03
 */

#include <gtest/gtest.h>
#include <libtbag/string/Flags.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(FlagsTest, SplitTokens)
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
    ASSERT_STREQ("temp", result[0].c_str());
    ASSERT_STREQ("--arg1=temp", result[1].c_str());
    ASSERT_STREQ("--arg2=space temp", result[2].c_str());
    ASSERT_STREQ("--arg3=\"value quoting", result[3].c_str());
    ASSERT_STREQ("--arg4='single quotes", result[4].c_str());
    ASSERT_STREQ("1", result[5].c_str());
    ASSERT_STREQ("2", result[6].c_str());
    ASSERT_STREQ("3", result[7].c_str());
    ASSERT_STREQ("noarg1", result[8].c_str());
    ASSERT_STREQ("noarg2", result[9].c_str());
}

TEST(FlagsTest, ConvertFlag)
{
    auto t1 = Flags::convertStringToFlag("--arg1=temp");
    ASSERT_STREQ("arg1", t1.key.c_str());
    ASSERT_STREQ("temp", t1.value.c_str());

    auto t2 = Flags::convertStringToFlag("--arg1_temp");
    ASSERT_STREQ("arg1_temp", t2.key.c_str());
    ASSERT_STREQ("", t2.value.c_str());

    auto t3 = Flags::convertStringToFlag("value=temp");
    ASSERT_STREQ("", t3.key.c_str());
    ASSERT_STREQ("value=temp", t3.value.c_str());
}

TEST(FlagsTest, At)
{
    Flags flags;
    flags.push(Flags::Flag("arg1", "temp1"));
    flags.push(Flags::Flag("arg2", "temp2"));

    Flags const CONST_FLAGS = flags;

    ASSERT_EQ(2U, CONST_FLAGS.size());
    ASSERT_STREQ( "arg1", CONST_FLAGS.at(0).key.c_str());
    ASSERT_STREQ("temp1", CONST_FLAGS.at(0).value.c_str());
    ASSERT_STREQ( "arg2", CONST_FLAGS.at(1).key.c_str());
    ASSERT_STREQ("temp2", CONST_FLAGS.at(1).value.c_str());
}

TEST(FlagsTest, Parse_Find)
{
    std::string const args = "value --arg1 --arg2=temp";
    Flags flags;
    flags.parse(args);

    ASSERT_EQ(3U, flags.size());

    auto flag1_1 = flags.findByKey("value");
    ASSERT_FALSE(flags.existsByKey("value"));
    ASSERT_STREQ("", flag1_1.key.c_str());
    ASSERT_STREQ("", flag1_1.value.c_str());

    auto flag1_2 = flags.findByValue("value");
    ASSERT_TRUE(flags.existsByValue("value"));
    ASSERT_STREQ("", flag1_2.key.c_str());
    ASSERT_STREQ("value", flag1_2.value.c_str());

    auto unnamed = flags.getUnnamedValues();
    ASSERT_EQ(1, unnamed.size());
    ASSERT_EQ("value", unnamed.at(0));

    auto flag2 = flags.findByKey("arg1");
    ASSERT_TRUE(flags.existsByKey("arg1"));
    ASSERT_STREQ("arg1", flag2.key.c_str());
    ASSERT_STREQ("", flag2.value.c_str());

    auto flag3 = flags.findByKey("arg2");
    ASSERT_TRUE(flags.existsByKey("arg2"));
    ASSERT_STREQ("arg2", flag3.key.c_str());
    ASSERT_STREQ("temp", flag3.value.c_str());

    auto flag4 = flags.findByValue("temp");
    ASSERT_TRUE(flags.existsByValue("temp"));
    ASSERT_STREQ("arg2", flag4.key.c_str());
    ASSERT_STREQ("temp", flag4.value.c_str());
}

TEST(FlagsTest, Remove)
{
    std::string args = "value /arg1 /arg2&arg1";
    Flags flags;
    flags.parse(args, "/", "&");

    ASSERT_EQ(3U, flags.size());
    ASSERT_FALSE(flags.remove("Unknown"));
    ASSERT_FALSE(flags.remove(3));

    ASSERT_TRUE(flags.remove("arg1"));
    ASSERT_EQ(2U, flags.size());

    ASSERT_TRUE(flags.remove(0));
    ASSERT_EQ(1U, flags.size());

    ASSERT_TRUE(flags.remove("arg2"));
    ASSERT_EQ(0U, flags.size());
}

TEST(FlagsTest, Argv)
{
    std::string const args = "value --arg1 --arg2=arg1";
    Flags flags;
    flags.parse(args);

    auto argv_obj = flags.getArgv();
    ASSERT_EQ(3, argv_obj.argc());
    ASSERT_STREQ("value", *(argv_obj.argv() + 0));
    ASSERT_STREQ("--arg1", *(argv_obj.argv() + 1));
    ASSERT_STREQ("--arg2=arg1", *(argv_obj.argv() + 2));

    Flags flags2(argv_obj.argc(), argv_obj.argv());
    ASSERT_EQ(3U, flags.size());
    ASSERT_STREQ("", flags2.at(0).key.c_str());
    ASSERT_STREQ("value", flags2.at(0).value.c_str());
    ASSERT_STREQ("arg1", flags2.at(1).key.c_str());
    ASSERT_STREQ("", flags2.at(1).value.c_str());
    ASSERT_STREQ("arg2", flags2.at(2).key.c_str());
    ASSERT_STREQ("arg1", flags2.at(2).value.c_str());
}

