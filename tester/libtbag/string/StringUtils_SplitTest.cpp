/**
 * @file   StringUtils_SplitTest.cpp
 * @brief  StringUtils_Split class tester.
 * @author zer0
 * @date   2020-03-22
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

    auto const tokens4 = splitSize("12345,67890,abcde,fg", 6);
    ASSERT_EQ(4, tokens4.size());
    ASSERT_STREQ("12345,", tokens4[0].c_str());
    ASSERT_STREQ("67890,", tokens4[1].c_str());
    ASSERT_STREQ("abcde,", tokens4[2].c_str());
    ASSERT_STREQ("fg", tokens4[3].c_str());
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

