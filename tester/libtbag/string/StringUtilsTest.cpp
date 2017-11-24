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
    std::string const DELIMITER = "//";

    auto tokens0 = splitTokens("", DELIMITER);
    ASSERT_EQ(0, tokens0.size());

    auto tokens1 = splitTokens("//", DELIMITER);
    ASSERT_EQ(0, tokens1.size());

    auto tokens2 = splitTokens("////", DELIMITER);
    ASSERT_EQ(0, tokens2.size());

    auto tokens3 = splitTokens("////A", DELIMITER);
    ASSERT_EQ(1, tokens3.size());
    ASSERT_STREQ("A", tokens3[0].c_str());

    auto tokens4 = splitTokens("A", DELIMITER);
    ASSERT_EQ(1, tokens4.size());
    ASSERT_STREQ("A", tokens4[0].c_str());

    auto tokens5 = splitTokens("A//", DELIMITER);
    ASSERT_EQ(1, tokens5.size());
    ASSERT_STREQ("A", tokens5[0].c_str());

    auto tokens6 = splitTokens("A//B", DELIMITER);
    ASSERT_EQ(2, tokens6.size());
    ASSERT_STREQ("A", tokens6[0].c_str());
    ASSERT_STREQ("B", tokens6[1].c_str());

    auto tokens7 = splitTokens("A//B//", DELIMITER);
    ASSERT_EQ(2, tokens7.size());
    ASSERT_STREQ("A", tokens7[0].c_str());
    ASSERT_STREQ("B", tokens7[1].c_str());

    auto tokens8 = splitTokens("A//B/", DELIMITER);
    ASSERT_EQ(2, tokens8.size());
    ASSERT_STREQ("A", tokens8[0].c_str());
    ASSERT_STREQ("B/", tokens8[1].c_str());
}

TEST(StringUtilsTest, SplitTokens_NotRemoveEmpty)
{
    std::string const DELIMITER = ":";

    auto tokens0 = splitTokens("", DELIMITER, false);
    ASSERT_EQ(0, tokens0.size());

    auto tokens1 = splitTokens("A", DELIMITER, false);
    ASSERT_EQ(1, tokens1.size());
    ASSERT_STREQ("A", tokens1[0].c_str());

    auto tokens2 = splitTokens(":", DELIMITER, false);
    ASSERT_EQ(2, tokens2.size());
    ASSERT_STREQ("", tokens2[0].c_str());
    ASSERT_STREQ("", tokens2[1].c_str());

    auto tokens3 = splitTokens("::", DELIMITER, false);
    ASSERT_EQ(3, tokens3.size());
    ASSERT_STREQ("", tokens3[0].c_str());
    ASSERT_STREQ("", tokens3[1].c_str());
    ASSERT_STREQ("", tokens3[2].c_str());

    auto tokens4 = splitTokens(":::", DELIMITER, false);
    ASSERT_EQ(4, tokens4.size());
    ASSERT_STREQ("", tokens4[0].c_str());
    ASSERT_STREQ("", tokens4[1].c_str());
    ASSERT_STREQ("", tokens4[2].c_str());
    ASSERT_STREQ("", tokens4[3].c_str());

    auto tokens5 = splitTokens(":A::", DELIMITER, false);
    ASSERT_EQ(4, tokens5.size());
    ASSERT_STREQ("", tokens5[0].c_str());
    ASSERT_STREQ("A", tokens5[1].c_str());
    ASSERT_STREQ("", tokens5[2].c_str());
    ASSERT_STREQ("", tokens5[3].c_str());

    auto tokens6 = splitTokens("A:", DELIMITER, false);
    ASSERT_EQ(2, tokens6.size());
    ASSERT_STREQ("A", tokens6[0].c_str());
    ASSERT_STREQ("", tokens6[1].c_str());

    auto tokens7 = splitTokens(":A", DELIMITER, false);
    ASSERT_EQ(2, tokens7.size());
    ASSERT_STREQ("", tokens7[0].c_str());
    ASSERT_STREQ("A", tokens7[1].c_str());

    auto tokens8 = splitTokens("A:B", DELIMITER, false);
    ASSERT_EQ(2, tokens8.size());
    ASSERT_STREQ("A", tokens8[0].c_str());
    ASSERT_STREQ("B", tokens8[1].c_str());
}

TEST(StringUtilsTest, SplitUtf8Tokens)
{
    auto tokens1 = splitUtf8Tokens(UTF8_SOURCE, UTF8_NA);
    ASSERT_EQ(2U, tokens1.size());
    ASSERT_EQ(UTF8_GA, tokens1[0]);
    ASSERT_EQ(UTF8_DA, tokens1[1]);

    auto tokens2 = splitUtf8Tokens(UTF8_SOURCE, UTF8_GA);
    ASSERT_EQ(1U, tokens2.size());
    ASSERT_EQ(UTF8_NA + UTF8_DA, tokens2[0]);

    auto tokens3 = splitUtf8Tokens(UTF8_SOURCE, UTF8_DA);
    ASSERT_EQ(1U, tokens3.size());
    ASSERT_EQ(UTF8_GA + UTF8_NA, tokens3[0]);
}

TEST(StringUtilsTest, SplitUtf8Tokens_NotRemoveEmpty)
{
    auto tokens1 = splitUtf8Tokens(UTF8_SOURCE, UTF8_GA, false);
    ASSERT_EQ(2U, tokens1.size());
    ASSERT_EQ(std::string(), tokens1[0]);
    ASSERT_EQ(UTF8_NA + UTF8_DA, tokens1[1]);

    auto tokens2 = splitUtf8Tokens(UTF8_SOURCE, UTF8_NA, false);
    ASSERT_EQ(2U, tokens2.size());
    ASSERT_EQ(UTF8_GA, tokens2[0]);
    ASSERT_EQ(UTF8_DA, tokens2[1]);

    auto tokens3 = splitUtf8Tokens(UTF8_SOURCE, UTF8_DA, false);
    ASSERT_EQ(2U, tokens3.size());
    ASSERT_EQ(UTF8_GA + UTF8_NA, tokens3[0]);
    ASSERT_EQ(std::string(), tokens3[1]);
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
    std::cout << convertByteVectorToHexStringBox(buffer, WIDTH) << std::endl;

    buffer.push_back(i++);
    std::cout << convertByteVectorToHexStringBox(buffer, WIDTH) << std::endl;

    buffer.push_back(i++);
    std::cout << convertByteVectorToHexStringBox(buffer, WIDTH) << std::endl;
}

