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

using ap = ArgumentParser;
using at = ArgumentParser::ActionType;

TEST(ArgumentParserTest, Add)
{
    ArgumentParser parser;
    ASSERT_EQ(E_SUCCESS, parser.add({{"-d", "--device"},
                                     ArgumentParser::ActionType::AT_STORE,
                                     "device"}));
    ASSERT_EQ(E_SUCCESS, parser.add(ap::const_value=2,
                                    ap::default_value=1,
                                    ap::store_const,
                                    ap::name="--input-type",
                                    ap::name="-t",
                                    ap::dest="input_type"));
    ASSERT_EQ(E_SUCCESS, parser.add("type", at::AT_STORE, "type"));
}

TEST(ArgumentParserTest, Optional_01)
{
    ArgumentParser parser;
    ASSERT_EQ(E_SUCCESS, parser.add({{"-d", "--device"},
                                     ArgumentParser::ActionType::AT_STORE,
                                     "device"}));

    auto const result1 = parser.parse("program -d 0");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(1, result1.val.optional.size());
    ASSERT_STREQ("0", result1.val.getOptional("device").c_str());
    ASSERT_EQ(0, result1.val.positional.size());
    ASSERT_EQ(0, result1.val.remain.size());

    auto const result2 = parser.parse("program --device=1");
    ASSERT_EQ(E_SUCCESS, result2.code);
    ASSERT_EQ(1, result2.val.optional.size());
    ASSERT_STREQ("1", result2.val.getOptional("device").c_str());
    ASSERT_EQ(0, result2.val.positional.size());
    ASSERT_EQ(0, result2.val.remain.size());

    auto const result3 = parser.parse("program --device 2");
    ASSERT_EQ(E_SUCCESS, result3.code);
    ASSERT_EQ(1, result3.val.optional.size());
    ASSERT_STREQ("2", result3.val.getOptional("device").c_str());
    ASSERT_EQ(0, result3.val.positional.size());
    ASSERT_EQ(0, result3.val.remain.size());

    auto const result4 = parser.parse("program --device");
    ASSERT_EQ(E_ILLSTATE, result4.code);

    auto const result5 = parser.parse("program test");
    ASSERT_EQ(E_SUCCESS, result5.code);
    ASSERT_EQ(0, result5.val.optional.size());
    ASSERT_EQ(0, result5.val.positional.size());
    ASSERT_EQ(1, result5.val.remain.size());
    ASSERT_STREQ("test", result5.val.remain[0].c_str());
}

TEST(ArgumentParserTest, Optional_02)
{
    ArgumentParser parser;
    ASSERT_EQ(E_SUCCESS, parser.add(ap::const_value=2,
                                    ap::default_value=1,
                                    ap::store_const,
                                    ap::name="--device",
                                    ap::name="-d",
                                    ap::dest="device"));

    auto const result1 = parser.parse("program --device 3 test1 test2");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(1, result1.val.optional.size());
    ASSERT_STREQ("2", result1.val.getOptional("device").c_str());
    ASSERT_EQ(0, result1.val.positional.size());
    ASSERT_EQ(3, result1.val.remain.size());
    ASSERT_STREQ("3", result1.val.remain[0].c_str());
    ASSERT_STREQ("test1", result1.val.remain[1].c_str());
    ASSERT_STREQ("test2", result1.val.remain[2].c_str());

    auto const result2 = parser.parse("program");
    ASSERT_EQ(1, result2.val.optional.size());
    ASSERT_STREQ("1", result2.val.getOptional("device").c_str());
    ASSERT_EQ(0, result2.val.positional.size());
    ASSERT_EQ(0, result2.val.remain.size());

    auto const result3 = parser.parse("program -d");
    ASSERT_EQ(1, result3.val.optional.size());
    ASSERT_STREQ("2", result3.val.getOptional("device").c_str());
    ASSERT_EQ(0, result3.val.positional.size());
    ASSERT_EQ(0, result3.val.remain.size());
}

TEST(ArgumentParserTest, Positional_01)
{
    ArgumentParser parser;
    ASSERT_EQ(E_SUCCESS, parser.add("type", at::AT_STORE, "type"));

    auto const result1 = parser.parse("program bool test1 test2");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(0, result1.val.optional.size());
    ASSERT_EQ(1, result1.val.positional.size());
    ASSERT_STREQ("bool", result1.val.positional.at("type").c_str());
    ASSERT_EQ(2, result1.val.remain.size());
    ASSERT_STREQ("test1", result1.val.remain[0].c_str());
    ASSERT_STREQ("test2", result1.val.remain[1].c_str());

    auto const result2 = parser.parse("program");
    ASSERT_EQ(E_ILLARGS, result2.code);
}

TEST(ArgumentParserTest, Positional_02)
{
    ArgumentParser parser;
    ASSERT_EQ(E_SUCCESS, parser.add(ap::name="aa1"));
    ASSERT_EQ(E_SUCCESS, parser.add(ap::name="aa2", ap::default_value=100));

    auto const result1 = parser.parse("program val1");
    ASSERT_EQ(E_SUCCESS, result1.code);
    ASSERT_EQ(0, result1.val.optional.size());
    ASSERT_EQ(2, result1.val.positional.size());
    ASSERT_STREQ("val1", result1.val.positional.at("aa1").c_str());
    ASSERT_STREQ("100", result1.val.positional.at("aa2").c_str());
    ASSERT_EQ(0, result1.val.remain.size());

    auto const result2 = parser.parse("program val1 val2");
    ASSERT_EQ(E_SUCCESS, result2.code);
    ASSERT_EQ(0, result2.val.optional.size());
    ASSERT_EQ(2, result2.val.positional.size());
    ASSERT_STREQ("val1", result2.val.positional.at("aa1").c_str());
    ASSERT_STREQ("val2", result2.val.positional.at("aa2").c_str());
    ASSERT_EQ(0, result2.val.remain.size());

    auto const result3 = parser.parse("program");
    ASSERT_EQ(E_ILLARGS, result3.code);
}

struct ArgumentParserTestFixture : public testing::Test
{
    ArgumentParser parser;

    void SetUp() override
    {
        parser.add(ap::name="--input", ap::name="-i");
        parser.add(ap::name="--output", ap::name="-o");
        parser.add(ap::name="--threshold", ap::name="-t", ap::default_value=0.5);
        parser.add(ap::name="--verbose", ap::name="-v", ap::store_const, ap::const_value=true);
        parser.add(ap::name="cmd1");
        parser.add(ap::name="cmd2", ap::default_value="empty");
    }

    void TearDown() override
    {
        // EMPTY.
    }
};

TEST_F(ArgumentParserTestFixture, Parse_Complex)
{
    auto const result = parser.parse("program -i test kkk -o result zzz -v xxx -t 0.7 vvv");
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_EQ(4, result.val.optional.size());
    ASSERT_STREQ("test", result.val.getOptional("input").c_str());
    ASSERT_STREQ("result", result.val.getOptional("output").c_str());
    ASSERT_STREQ("1", result.val.getOptional("verbose").c_str());
    ASSERT_STREQ("0.7", result.val.getOptional("threshold").c_str());
    ASSERT_EQ(2, result.val.positional.size());
    ASSERT_STREQ("kkk", result.val.positional.at("cmd1").c_str());
    ASSERT_STREQ("zzz", result.val.positional.at("cmd2").c_str());
    ASSERT_EQ(2, result.val.remain.size());
    ASSERT_STREQ("xxx", result.val.remain[0].c_str());
    ASSERT_STREQ("vvv", result.val.remain[1].c_str());
}

TEST_F(ArgumentParserTestFixture, StopParsing)
{
    auto const result = parser.parse("program kkk -o result -- -v -t 0.7 kkk zzz xxx");
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_EQ(2, result.val.optional.size());
    ASSERT_STREQ("result", result.val.getOptional("output").c_str());
    ASSERT_STREQ("0.5", result.val.getOptional("threshold").substr(0, 3).c_str());
    ASSERT_EQ(2, result.val.positional.size());
    ASSERT_STREQ("kkk", result.val.positional.at("cmd1").c_str());
    ASSERT_STREQ("empty", result.val.positional.at("cmd2").c_str());
    ASSERT_EQ(6, result.val.remain.size());
    ASSERT_STREQ("-v", result.val.remain[0].c_str());
    ASSERT_STREQ("-t", result.val.remain[1].c_str());
    ASSERT_STREQ("0.7", result.val.remain[2].c_str());
    ASSERT_STREQ("kkk", result.val.remain[3].c_str());
    ASSERT_STREQ("zzz", result.val.remain[4].c_str());
    ASSERT_STREQ("xxx", result.val.remain[5].c_str());
}

