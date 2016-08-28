/**
 * @file   StringsTest.cpp
 * @brief  Strings class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/string/Strings.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(StringsTest, splitTokens)
{
    std::string delimiter = "//";
    std::string temp1 = std::string("1TEMP1");
    std::string temp2 = std::string("2TEMP2");

    std::vector<std::string> path_set;
    std::string content;

    content = std::string() + delimiter;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(0U, path_set.size());

    content = std::string() + delimiter + delimiter;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(0U, path_set.size());

    content = std::string() + delimiter + delimiter + temp1;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(1U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);

    content = std::string() + delimiter + delimiter + temp1;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(1U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);

    content = temp1;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(1U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);

    content = temp1 + delimiter;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(1U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);

    content = temp1 + delimiter + temp2;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(2U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);
    ASSERT_EQ(temp2, path_set[1]);

    content = temp1 + delimiter + temp2 + delimiter;
    path_set = splitTokens(content, delimiter);
    ASSERT_EQ(2U, path_set.size());
    ASSERT_EQ(temp1, path_set[0]);
    ASSERT_EQ(temp2, path_set[1]);
}

TEST(StringsTest, convertStringWithFloatingPoint)
{
    std::string result = convertStringWithFloatingPoint(10.5542, 2);
    ASSERT_STREQ("10.55", result.c_str());
}

TEST(StringsTest, splitMatch)
{
    std::string match = R"(TEST[^ ]*)";
    std::string content = std::string("TEST1 TEST10 TEST");

    std::vector<std::string> list = Strings::splitMatch(content, std::regex(match));
    ASSERT_EQ(3U, list.size());
    ASSERT_STREQ("TEST1", list[0].c_str());
    ASSERT_STREQ("TEST10", list[1].c_str());
    ASSERT_STREQ("TEST", list[2].c_str());
}

TEST(StringsTest, isMatch)
{
    std::string content = "https://www.blackhole-project.com:8080/test";
    std::string match   = R"(^https?:\/\/[a-zA-Z0-9\.\-]+(:[0-9]+)?\/.*$)";

    ASSERT_TRUE(Strings::isMatch(content, match));
}

TEST(StringsTest, trimLeft)
{
    std::string t1 = "  TEST";
    std::string t2 = "  ";
    std::string t3 = "T  ";
    std::string t4 = "T EST";
    std::string t5 = "";

    ASSERT_STREQ( "TEST", Strings::trimLeft(t1).c_str());
    ASSERT_STREQ(     "", Strings::trimLeft(t2).c_str());
    ASSERT_STREQ(  "T  ", Strings::trimLeft(t3).c_str());
    ASSERT_STREQ("T EST", Strings::trimLeft(t4).c_str());
    ASSERT_STREQ(     "", Strings::trimLeft(t5).c_str());
}

TEST(StringsTest, trimRight)
{
    std::string t1 = "TEST  ";
    std::string t2 = "  ";
    std::string t3 = "  T";
    std::string t4 = "TES T";
    std::string t5 = "";

    ASSERT_STREQ( "TEST", Strings::trimRight(t1).c_str());
    ASSERT_STREQ(     "", Strings::trimRight(t2).c_str());
    ASSERT_STREQ(  "  T", Strings::trimRight(t3).c_str());
    ASSERT_STREQ("TES T", Strings::trimRight(t4).c_str());
    ASSERT_STREQ(     "", Strings::trimRight(t5).c_str());
}

TEST(StringsTest, trim)
{
    std::string t1 = " TEST  ";
    std::string t2 = "  ";
    std::string t3 = "  T";
    std::string t4 = "T  ";
    std::string t5 = "TE ST";
    std::string t6 = "";

    ASSERT_STREQ( "TEST", Strings::trim(t1).c_str());
    ASSERT_STREQ(     "", Strings::trim(t2).c_str());
    ASSERT_STREQ(    "T", Strings::trim(t3).c_str());
    ASSERT_STREQ(    "T", Strings::trim(t4).c_str());
    ASSERT_STREQ("TE ST", Strings::trim(t5).c_str());
    ASSERT_STREQ(     "", Strings::trim(t6).c_str());
}

