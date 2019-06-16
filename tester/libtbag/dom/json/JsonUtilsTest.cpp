/**
 * @file   JsonUtilsTest.cpp
 * @brief  JsonUtils class tester.
 * @author zer0
 * @date   2019-06-16
 */

#include <gtest/gtest.h>
#include <libtbag/dom/json/JsonUtils.hpp>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::json;

TEST(JsonUtilsTest, Default)
{
    char const * const TEST_JSON_TEXT = R"({"key":"value"})";
    Json::Value value;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, value));
    auto const JSON_TEXT = writeFast(value);
    ASSERT_STREQ(TEST_JSON_TEXT, JSON_TEXT.c_str());
}

