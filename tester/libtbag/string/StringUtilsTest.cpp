/**
 * @file   StringUtilsTest.cpp
 * @brief  String utilities tester.
 * @author zer0
 * @date   2016-04-04
 * @date   2016-12-05 (Rename: StringUtilsTest -> StringUtilsTest)
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cctype>
#include <algorithm>
#include <iostream>

using namespace libtbag;
using namespace libtbag::string;
using namespace libtbag::test_data;

TEST(StringUtilsTest, SplitTokens)
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

TEST(StringUtilsTest, splitUtf8Tokens)
{
    std::vector<std::string> tokens;

    // 1st.
    tokens = splitUtf8Tokens(UTF8_SOURCE, UTF8_NA);
    ASSERT_EQ(2U, tokens.size());
    ASSERT_EQ(UTF8_GA, tokens[0]);
    ASSERT_EQ(UTF8_DA, tokens[1]);

    // 2nd.
    tokens = splitUtf8Tokens(UTF8_SOURCE, UTF8_GA);
    ASSERT_EQ(1U, tokens.size());
    ASSERT_EQ(UTF8_NA + UTF8_DA, tokens[0]);

    // 3rd.
    tokens = splitUtf8Tokens(UTF8_SOURCE, UTF8_DA);
    ASSERT_EQ(1U, tokens.size());
    ASSERT_EQ(UTF8_GA + UTF8_NA, tokens[0]);
}

TEST(StringUtilsTest, ConvertStringWithFloatingPoint)
{
    std::string result = convertStringWithFloatingPoint(10.5542, 2);
    ASSERT_STREQ("10.55", result.c_str());
}

TEST(StringUtilsTest, ConvertStringWithThreadId)
{
    std::string result = convertStringWithThreadId(std::this_thread::get_id());
    ASSERT_FALSE(result.empty());
}

TEST(StringUtilsTest, SplitMatch)
{
    std::string match = R"(TEST[^ ]*)";
    std::string content = std::string("TEST1 TEST10 TEST");

    std::vector<std::string> list = splitMatch(content, std::regex(match));
    ASSERT_EQ(3U, list.size());
    ASSERT_STREQ("TEST1", list[0].c_str());
    ASSERT_STREQ("TEST10", list[1].c_str());
    ASSERT_STREQ("TEST", list[2].c_str());
}

TEST(StringUtilsTest, IsMatch)
{
    std::string content = "https://www.blackhole-project.com:8080/test";
    std::string match   = R"(^https?:\/\/[a-zA-Z0-9\.\-]+(:[0-9]+)?\/.*$)";
    ASSERT_TRUE(isMatch(content, match));
}

TEST(StringUtilsTest, IsUtf8Match)
{
    std::string content1 = UTF8_SOURCE;
    std::string match1 = "^??" + UTF8_DA + "$";
    std::string match2 = "^????" + UTF8_DA + "$";

    ASSERT_TRUE (isUtf8Match(content1, match1));
    ASSERT_FALSE(isUtf8Match(content1, match2));

    std::string content2 = UTF8_GA + "A" + UTF8_NA + "B" + UTF8_DA;
    std::string match3 = "^?A?B?$";
    ASSERT_TRUE(isUtf8Match(content2, match3));
}

TEST(StringUtilsTest, TrimLeft)
{
    std::string t1 = "  TEST";
    std::string t2 = "  ";
    std::string t3 = "T  ";
    std::string t4 = "T EST";
    std::string t5 = "";

    ASSERT_STREQ( "TEST", trimLeft(t1).c_str());
    ASSERT_STREQ(     "", trimLeft(t2).c_str());
    ASSERT_STREQ(  "T  ", trimLeft(t3).c_str());
    ASSERT_STREQ("T EST", trimLeft(t4).c_str());
    ASSERT_STREQ(     "", trimLeft(t5).c_str());
}

TEST(StringUtilsTest, TrimRight)
{
    std::string t1 = "TEST  ";
    std::string t2 = "  ";
    std::string t3 = "  T";
    std::string t4 = "TES T";
    std::string t5 = "";

    ASSERT_STREQ( "TEST", trimRight(t1).c_str());
    ASSERT_STREQ(     "", trimRight(t2).c_str());
    ASSERT_STREQ(  "  T", trimRight(t3).c_str());
    ASSERT_STREQ("TES T", trimRight(t4).c_str());
    ASSERT_STREQ(     "", trimRight(t5).c_str());
}

TEST(StringUtilsTest, Trim)
{
    std::string t1 = " TEST  ";
    std::string t2 = "  ";
    std::string t3 = "  T";
    std::string t4 = "T  ";
    std::string t5 = "TE ST";
    std::string t6 = "";

    ASSERT_STREQ( "TEST", trim(t1).c_str());
    ASSERT_STREQ(     "", trim(t2).c_str());
    ASSERT_STREQ(    "T", trim(t3).c_str());
    ASSERT_STREQ(    "T", trim(t4).c_str());
    ASSERT_STREQ("TE ST", trim(t5).c_str());
    ASSERT_STREQ(     "", trim(t6).c_str());
}

TEST(StringUtilsTest, createRandomString)
{
    const std::size_t BUFFER_SIZE = 5;
    const std::size_t TEST_COUNT  = 100;

    std::string buffer;
    buffer.resize(BUFFER_SIZE);

    for (std::size_t i = 0; i < TEST_COUNT; ++i) {
        ASSERT_TRUE(createRandomString(&buffer[0], BUFFER_SIZE));
        ASSERT_TRUE(std::all_of(buffer.begin(), buffer.end(), [](char & v)->bool{return ::isdigit(v) || ::isalpha(v);}));
    }
}

TEST(StringUtilsTest, TranslateCase)
{
    ASSERT_EQ(std::string("abcdefg1234"), string::lower("abcdEFG1234"));
    ASSERT_EQ(std::string("ABCDEFG1234"), string::upper("abcdEFG1234"));
}

TEST(StringUtilsTest, ConvertAddressToHexString)
{
    void * temp1 = nullptr;
    int    temp2 = 0;

    auto result1 = convertAddressToHexString(temp1);
    auto result2 = convertAddressToHexString(&temp2);

    auto str1 = convertAddressHexStringToString(result1);
    auto str2 = convertAddressHexStringToString(result2);

    std::cout << "HEX Address 01: " << str1 << std::endl;
    std::cout << "HEX Address 02: " << str2 << std::endl;

    std::string const REGEX = std::string("^0x[0-9A-F]+$");
    ASSERT_TRUE(isMatch(str1, REGEX));
    ASSERT_TRUE(isMatch(str2, REGEX));
}

TEST(StringUtilsTest, ConvertByteArrayToHexStringBox)
{
    int const WIDTH = 16;
    int const HEIGHT = 5;
    int const SIZE = WIDTH * HEIGHT;

    int i = 0;
    std::vector<uint8_t> buffer;
    for (i = 0; i < SIZE - 1; ++i) {
        buffer.push_back(i);
    }
    std::cout << convertByteArrayToHexStringBox(buffer, WIDTH) << std::endl;

    buffer.push_back(i++);
    std::cout << convertByteArrayToHexStringBox(buffer, WIDTH) << std::endl;

    buffer.push_back(i++);
    std::cout << convertByteArrayToHexStringBox(buffer, WIDTH) << std::endl;
}

