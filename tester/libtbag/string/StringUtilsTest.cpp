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

TEST(StringUtilsTest, MergeTokens)
{
    ASSERT_STREQ("", mergeTokens({}, ":").c_str());
    ASSERT_STREQ("A", mergeTokens({"A"}, ":").c_str());
    ASSERT_STREQ("A:B", mergeTokens({"A", "B"}, ":").c_str());
    ASSERT_STREQ("A:B:C", mergeTokens({"A", "B", "C"}, ":").c_str());
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

TEST(StringUtilsTest, SplitSize)
{
    auto const tokens0 = splitSize("", 1);
    ASSERT_TRUE(tokens0.empty());

    auto const tokens1 = splitSize("12", 1);
    ASSERT_EQ(2, tokens1.size());
    ASSERT_STREQ("1", tokens1[0].c_str());
    ASSERT_STREQ("2", tokens1[1].c_str());

    auto const tokens2 = splitSize("12345", 3);
    ASSERT_EQ(2, tokens2.size());
    ASSERT_STREQ("123", tokens2[0].c_str());
    ASSERT_STREQ("45", tokens2[1].c_str());

    auto const tokens3 = splitSize("1234567890", 100);
    ASSERT_EQ(1, tokens3.size());
    ASSERT_STREQ("1234567890", tokens3[0].c_str());
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

TEST(StringUtilsTest, SplitMatch_01)
{
    auto const match = R"(TEST[^ ]*)";
    auto const content = "TEST1 TEST10 TEST";
    auto const result = splitMatch(content, match);
    ASSERT_EQ(3u, result.size());
    ASSERT_STREQ("TEST1", result[0].c_str());
    ASSERT_STREQ("TEST10", result[1].c_str());
    ASSERT_STREQ("TEST", result[2].c_str());
}

TEST(StringUtilsTest, SplitMatch_02)
{
    auto const match = R"([0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*)";
    auto const content = R"(Test OK. The remaining time is 21 days
c2core lua(Lua 5.1) storage init: Thu Dec 19 09:48:23 2019
0.3.5
Packet: 1.2
Distribution code: 6)";
    auto const result = splitMatch(content, match);
    ASSERT_EQ(1u, result.size());
    ASSERT_STREQ("0.3.5", result[0].c_str());
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

    ASSERT_STREQ("200", toString(200.0).substr(0, 3).c_str());
    ASSERT_STREQ("201", toString(201.0).substr(0, 3).c_str());
    ASSERT_STREQ("202", toString(202.0).substr(0, 3).c_str());

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

TEST(StringUtilsTest, ToByteSize)
{
    ASSERT_EQ(100, toByteSize(" 100 "));
    ASSERT_EQ(10, toByteSize(" 10 byte"));
    ASSERT_EQ(5, toByteSize("  5b"));

    ASSERT_EQ(1024, toByteSize("1Kb"));
    ASSERT_EQ(1, toByteSize("1---"));
    ASSERT_EQ(1024, toByteSize("1 kbyte"));

    ASSERT_EQ(2048*1024*(std::size_t)1024, toByteSize("2048 mb"));
    ASSERT_EQ(9*1024*(std::size_t)1024, toByteSize("9  Mbyte"));

    ASSERT_EQ(2*1024*1024*(std::size_t)1024, toByteSize("2Gb"));
    ASSERT_EQ(6*1024*1024*(std::size_t)1024, toByteSize("6g"));
}

TEST(StringUtilsTest, ToUpperByteText)
{
#if 0
    // [WARNING] Outbound of 64bit size_t type.
    // ASSERT_STREQ("1y", toUpperByteText(YOTTA_BYTE_TO_BYTE).c_str());
    // ASSERT_STREQ("1z", toUpperByteText(ZETTA_BYTE_TO_BYTE).c_str());
#endif

    ASSERT_STREQ("1EB", toUpperByteText(EXA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1PB", toUpperByteText(PETA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1TB", toUpperByteText(TERA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1GB", toUpperByteText(GIGA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1MB", toUpperByteText(MEGA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1KB", toUpperByteText(KILO_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1B", toUpperByteText(1).c_str());
}

