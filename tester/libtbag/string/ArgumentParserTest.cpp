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

TEST(ArgumentParserTest, Default)
{
    ArgumentParser parser;
    parser.add({{"-d", "--device"}, "device", ArgumentParser::ActionType::AT_STORE});
    auto const result = parser.parse("program -d 0");
    ASSERT_EQ(E_SUCCESS, result.code);
    ASSERT_STREQ("0", result.value.optional.at("device").c_str());
}

