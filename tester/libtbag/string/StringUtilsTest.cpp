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
#include <libtbag/Unit.hpp>

#include <cctype>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace libtbag;
using namespace libtbag::string;
using namespace libtbag::test_data;

TEST(StringUtilsTest, MergeTokens)
{
    ASSERT_STREQ("", mergeTokens({}, ":").c_str());
    ASSERT_STREQ("A", mergeTokens({"A"}, ":").c_str());
    ASSERT_STREQ("A:B", mergeTokens({"A", "B"}, ":").c_str());
    ASSERT_STREQ("A:B:C", mergeTokens({"A", "B", "C"}, ":").c_str());
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

TEST(StringUtilsTest, ReplaceRegex)
{
    ASSERT_STREQ("test", replaceRegex("t2st", "[0-9]s", "es").c_str());
}

TEST(StringUtilsTest, RemoveRegex)
{
    ASSERT_STREQ("test", removeRegex("te7st", "[0-9]").c_str());
}

TEST(StringUtilsTest, Replace)
{
    ASSERT_STREQ("/t/TEMP/", replace("/TEMP/TEMP/", "TEMP", "t").c_str());
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

TEST(StringUtilsTest, CreateRandomString)
{
    const std::size_t BUFFER_SIZE = 5;
    const std::size_t TEST_COUNT  = 100;

    std::string buffer;
    buffer.resize(BUFFER_SIZE);

    for (auto i = 0; i < TEST_COUNT; ++i) {
        createRandomString(&buffer[0], BUFFER_SIZE);
        auto const result = std::all_of(buffer.begin(), buffer.end(), [](char & v)->bool{
            return ::isdigit(v) || ::isalpha(v);
        });
        ASSERT_TRUE(result);
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

    auto const str1 = convertAddressToString(temp1);
    auto const str2 = convertAddressToString(&temp2);

    std::cout << "HEX Address 01: " << str1 << std::endl;
    std::cout << "HEX Address 02: " << str2 << std::endl;

    auto const hex_regex = std::string("^0x[0-9A-F]+$");
    ASSERT_TRUE(isMatch(str1, hex_regex));
    ASSERT_TRUE(isMatch(str2, hex_regex));
}

TEST(StringUtilsTest, ConvertByteVectorToHexStringBox)
{
    std::vector<uint8_t> const DATA1 = {0x01,0x02};
    std::vector<uint8_t> const DATA2 = {0x01,0x02,0x03,0x04};
    std::vector<uint8_t> const DATA3 = {0x01,0x02,0x03,0x04,0x05,0x06};
    std::vector<uint8_t> const DATA4 = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
    std::vector<uint8_t> const DATA5 = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};

    std::string const PREFIX = "0x";
    std::string const SEPARATOR = " ";
    std::string const NEW_LINE = "/";

    std::string const RESULT1 = "0x01 0x02";
    std::string const RESULT2 = "0x01 0x02 0x03 0x04";
    std::string const RESULT3 = "0x01 0x02 0x03 0x04/0x05 0x06";
    std::string const RESULT4 = "0x01 0x02 0x03 0x04/0x05 0x06 0x07 0x08";
    std::string const RESULT5 = "0x01 0x02 0x03 0x04/0x05 0x06 0x07 0x08/0x09 0x0A";

    int const WIDTH = 4;
    ASSERT_EQ(RESULT1, convertByteVectorToHexStringBox(DATA1, WIDTH, PREFIX, SEPARATOR, NEW_LINE));
    ASSERT_EQ(RESULT2, convertByteVectorToHexStringBox(DATA2, WIDTH, PREFIX, SEPARATOR, NEW_LINE));
    ASSERT_EQ(RESULT3, convertByteVectorToHexStringBox(DATA3, WIDTH, PREFIX, SEPARATOR, NEW_LINE));
    ASSERT_EQ(RESULT4, convertByteVectorToHexStringBox(DATA4, WIDTH, PREFIX, SEPARATOR, NEW_LINE));
    ASSERT_EQ(RESULT5, convertByteVectorToHexStringBox(DATA5, WIDTH, PREFIX, SEPARATOR, NEW_LINE));
}

TEST(StringUtilsTest, ConvertByteVectorToPrettyHexStringBox)
{
    int const width = 16;
    int const height = 16;
    int const size = width * height;

    std::vector<uint8_t> buffer;
    for (auto i = 0; i < size; ++i) {
        buffer.push_back(i);
    }

    auto const hex_box = convertByteVectorToPrettyHexStringBox(buffer, width);
    ASSERT_FALSE(hex_box.empty());
    std::cout << hex_box << std::endl;
}

TEST(StringUtilsTest, TrimRightOfRealNumberText)
{
    ASSERT_STREQ("100", trimRightOfRealNumberText("100").c_str());
    ASSERT_STREQ("9.0", trimRightOfRealNumberText("9.").c_str());
    ASSERT_STREQ("9.0", trimRightOfRealNumberText("9.0").c_str());
    ASSERT_STREQ("9.0", trimRightOfRealNumberText("9.00").c_str());
    ASSERT_STREQ("9.0", trimRightOfRealNumberText("9.00000").c_str());
    ASSERT_STREQ("9.001", trimRightOfRealNumberText("9.00100").c_str());
    ASSERT_STREQ("9.00001", trimRightOfRealNumberText("9.00001").c_str());
    ASSERT_STREQ("9.00f", trimRightOfRealNumberText("9.00f").c_str());
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

    ASSERT_STREQ("200.0", toString((      float)200).c_str());
    ASSERT_STREQ("201.0", toString((     double)201).c_str());
    ASSERT_STREQ("202.0", toString((long double)202).c_str());

    ASSERT_STREQ("200.0", toString(200.0).c_str());
    ASSERT_STREQ("201.0", toString(201.0).c_str());
    ASSERT_STREQ("202.0", toString(202.0).c_str());

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

    unsigned int value1;
    unsigned int value2;
    unsigned int value3;
    unsigned int value4;
    ASSERT_TRUE(toVal("0xA1B2C3D4", value1, nullptr, 16));
    ASSERT_TRUE(toVal("FF0011DD", value2, nullptr, 16));
    ASSERT_TRUE(toVal("1", value3, nullptr, 16));
    ASSERT_TRUE(toVal("F0000000", value4, nullptr, 16));
    ASSERT_EQ(0xA1B2C3D4, value1);
    ASSERT_EQ(0xFF0011DD, value2);
    ASSERT_EQ(1, value3);
    ASSERT_EQ(0xF0000000, value4);
}

TEST(StringUtilsTest, ToHexString)
{
    ASSERT_STREQ("FF", toHexString((unsigned char)0xFF, true).c_str());
    ASSERT_STREQ("ff", toHexString((unsigned char)0xFF, false).c_str());

    ASSERT_STREQ("FF", toHexString((unsigned short)0xFF, true).c_str());
    ASSERT_STREQ("ff", toHexString((unsigned short)0xFF, false).c_str());

    ASSERT_STREQ("FF", toHexString((unsigned int)0xFF, true).c_str());
    ASSERT_STREQ("ff", toHexString((unsigned int)0xFF, false).c_str());

    ASSERT_STREQ("FF", toHexString((unsigned long)0xFF, true).c_str());
    ASSERT_STREQ("ff", toHexString((unsigned long)0xFF, false).c_str());

    ASSERT_STREQ("FF", toHexString((unsigned long long)0xFF, true).c_str());
    ASSERT_STREQ("ff", toHexString((unsigned long long)0xFF, false).c_str());
}

