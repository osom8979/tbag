/**
 * @file   ArgumentParserTest.cpp
 * @brief  ArgumentParser class tester.
 * @author zer0
 * @date   2019-10-09
 */

#include <gtest/gtest.h>
#include <libtbag/string/ArgumentParser.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(ArgumentParserTest, Optional_01)
{
    ArgumentParser parser;
    parser.add({{"-d", "--device"}, "device", ArgumentParser::ActionType::AT_STORE});

    auto const result1 = parser.parse("program -d 0");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(1, result1.value.optional.size());
    ASSERT_STREQ("0", result1.value.optional.at("device").c_str());
    ASSERT_EQ(0, result1.value.positional.size());
    ASSERT_EQ(0, result1.value.remain.size());

    auto const result2 = parser.parse("program --device=1");
    ASSERT_EQ(E_SUCCESS, result2.code);
    ASSERT_EQ(1, result2.value.optional.size());
    ASSERT_STREQ("1", result2.value.optional.at("device").c_str());
    ASSERT_EQ(0, result2.value.positional.size());
    ASSERT_EQ(0, result2.value.remain.size());

    auto const result3 = parser.parse("program --device 2");
    ASSERT_EQ(E_SUCCESS, result3.code);
    ASSERT_EQ(1, result3.value.optional.size());
    ASSERT_STREQ("2", result3.value.optional.at("device").c_str());
    ASSERT_EQ(0, result3.value.positional.size());
    ASSERT_EQ(0, result3.value.remain.size());

    auto const result4 = parser.parse("program --device");
    ASSERT_EQ(E_ILLSTATE, result4.code);

    auto const result5 = parser.parse("program test");
    ASSERT_EQ(E_SUCCESS, result5.code);
    ASSERT_EQ(0, result5.value.optional.size());
    ASSERT_EQ(0, result5.value.positional.size());
    ASSERT_EQ(1, result5.value.remain.size());
    ASSERT_STREQ("test", result5.value.remain[0].c_str());
}

TEST(ArgumentParserTest, Optional_02)
{
    ArgumentParser parser;
    ArgumentParser::Arg arg1;
    arg1.names = {"-d", "--device"};
    arg1.dest = "device";
    arg1.action = ArgumentParser::ActionType::AT_STORE_CONST;
    arg1.default_value = "1";
    arg1.const_value = "2";
    parser.add(arg1);

    auto const result1 = parser.parse("program --device 3 test1 test2");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(1, result1.value.optional.size());
    ASSERT_STREQ("2", result1.value.optional.at("device").c_str());
    ASSERT_EQ(0, result1.value.positional.size());
    ASSERT_EQ(3, result1.value.remain.size());
    ASSERT_STREQ("3", result1.value.remain[0].c_str());
    ASSERT_STREQ("test1", result1.value.remain[1].c_str());
    ASSERT_STREQ("test2", result1.value.remain[2].c_str());

    auto const result2 = parser.parse("program");
    ASSERT_EQ(1, result2.value.optional.size());
    ASSERT_STREQ("1", result2.value.optional.at("device").c_str());
    ASSERT_EQ(0, result2.value.positional.size());
    ASSERT_EQ(0, result2.value.remain.size());

    auto const result3 = parser.parse("program -d");
    ASSERT_EQ(1, result3.value.optional.size());
    ASSERT_STREQ("2", result3.value.optional.at("device").c_str());
    ASSERT_EQ(0, result3.value.positional.size());
    ASSERT_EQ(0, result3.value.remain.size());
}

TEST(ArgumentParserTest, Positional_01)
{
    ArgumentParser parser;
    parser.add({{"type"}, "type"});

    auto const result1 = parser.parse("program bool test1 test2");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(0, result1.value.optional.size());
    ASSERT_EQ(1, result1.value.positional.size());
    ASSERT_STREQ("bool", result1.value.positional.at("type").c_str());
    ASSERT_EQ(2, result1.value.remain.size());
    ASSERT_STREQ("test1", result1.value.remain[0].c_str());
    ASSERT_STREQ("test2", result1.value.remain[1].c_str());

    auto const result2 = parser.parse("program");
    ASSERT_EQ(E_ILLARGS, result2.code);
}

