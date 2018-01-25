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
#include <limits>

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

TEST(StringUtilsTest, IsDigit)
{
    ASSERT_TRUE(isDigit("0"));
    ASSERT_TRUE(isDigit("01234567890123456789"));

    ASSERT_FALSE(isDigit("0123456789-0123456789"));
    ASSERT_FALSE(isDigit(""));
    ASSERT_FALSE(isDigit("-1"));
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

TEST(StringUtilsTest, ToString)
{
    ASSERT_STREQ("10", toString((              char)10).c_str());
    ASSERT_STREQ("11", toString((unsigned      char)11).c_str());
    ASSERT_STREQ("12", toString((             short)12).c_str());
    ASSERT_STREQ("13", toString((unsigned     short)13).c_str());
    ASSERT_STREQ("14", toString((               int)14).c_str());
    ASSERT_STREQ("15", toString((unsigned       int)15).c_str());
    ASSERT_STREQ("16", toString((              long)16).c_str());
    ASSERT_STREQ("17", toString((unsigned      long)17).c_str());
    ASSERT_STREQ("18", toString((         long long)18).c_str());
    ASSERT_STREQ("19", toString((unsigned long long)19).c_str());

    ASSERT_STREQ("200", toString((      float)200).substr(0, 3).c_str());
    ASSERT_STREQ("201", toString((     double)201).substr(0, 3).c_str());
    ASSERT_STREQ("202", toString((long double)202).substr(0, 3).c_str());

    std::string str;
    str = toString(std::numeric_limits<float>::infinity());
    std::cout << "Infinity (float): " << str << std::endl;
    str = toString(std::numeric_limits<float>::quiet_NaN());
    std::cout << "Quiet NaN (float): " << str << std::endl;
    str = toString(std::numeric_limits<float>::signaling_NaN());
    std::cout << "Signaling NaN (float): " << str << std::endl;
    str = toString(std::numeric_limits<float>::denorm_min());
    std::cout << "Denorm min (float): " << str << std::endl;

    str = toString(std::numeric_limits<double>::infinity());
    std::cout << "Infinity (double): " << str << std::endl;
    str = toString(std::numeric_limits<double>::quiet_NaN());
    std::cout << "Quiet NaN (double): " << str << std::endl;
    str = toString(std::numeric_limits<double>::signaling_NaN());
    std::cout << "Signaling NaN (double): " << str << std::endl;
    str = toString(std::numeric_limits<double>::denorm_min());
    std::cout << "Denorm min (double): " << str << std::endl;

    str = toString(std::numeric_limits<long double>::infinity());
    std::cout << "Infinity (long double): " << str << std::endl;
    str = toString(std::numeric_limits<long double>::quiet_NaN());
    std::cout << "Quiet NaN (long double): " << str << std::endl;
    str = toString(std::numeric_limits<long double>::signaling_NaN());
    std::cout << "Signaling NaN (long double): " << str << std::endl;
    str = toString(std::numeric_limits<long double>::denorm_min());
    std::cout << "Denorm min (long double): " << str << std::endl;
}

TEST(StringUtilsTest, ToValue)
{
    ASSERT_EQ(10, toValue<              char>("10"));
    ASSERT_EQ(11, toValue<unsigned      char>("11"));
    ASSERT_EQ(12, toValue<             short>("12"));
    ASSERT_EQ(13, toValue<unsigned     short>("13"));
    ASSERT_EQ(14, toValue<               int>("14"));
    ASSERT_EQ(15, toValue<unsigned       int>("15"));
    ASSERT_EQ(16, toValue<              long>("16"));
    ASSERT_EQ(17, toValue<unsigned      long>("17"));
    ASSERT_EQ(18, toValue<         long long>("18"));
    ASSERT_EQ(19, toValue<unsigned long long>("19"));

    ASSERT_FLOAT_EQ(200, toValue<      float>("200"));
    ASSERT_FLOAT_EQ(201, toValue<     double>("201"));
    ASSERT_FLOAT_EQ(202, toValue<long double>("202"));
}

