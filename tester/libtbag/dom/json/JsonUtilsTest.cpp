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

TEST(JsonUtilsTest, Path)
{
    char const * const TEST_JSON_TEXT = R"(
{
    "key1": { "temp": 1 },
    "key2": { "temp": 2 },
    "key3": { "temp": 3 }
})";
    Json::Value original;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, original));

    Json::Path path1(".key1.temp");
    auto const value1 = path1.make(original);
    ASSERT_TRUE(value1.isInt());
    ASSERT_EQ(1, value1.asInt());

    Json::Path path2(".key2.temp");
    auto const value2 = path2.make(original);
    ASSERT_TRUE(value2.isInt());
    ASSERT_EQ(2, value2.asInt());

    Json::Path path3(".key3.temp");
    auto const value3 = path3.make(original);
    ASSERT_TRUE(value3.isInt());
    ASSERT_EQ(3, value3.asInt());

    Json::Path path_error(".test");
    auto const value_error = path_error.make(original);
    ASSERT_TRUE(value_error.isNull());
}

TEST(JsonUtilsTest, Default)
{
    char const * const TEST_JSON_TEXT = R"({"key":"value"})";
    Json::Value value;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, value));
    auto const JSON_TEXT = writeFast(value);
    ASSERT_STREQ(TEST_JSON_TEXT, JSON_TEXT.c_str());
}

TEST(JsonUtilsTest, DoNotUseTheDropNull)
{
    char const * const TEST_JSON_TEXT = R"({"key":null})";
    Json::Value value;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, value));
    auto const JSON_TEXT = writeFast(value);
    ASSERT_STREQ(TEST_JSON_TEXT, JSON_TEXT.c_str());
}

TEST(JsonUtilsTest, OptErr)
{
    char const * const TEST_JSON_TEXT = R"({"key":"warning"})";
    Json::Value value;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, value));
    ASSERT_EQ(E_WARNING, optErr(value["key"]));
}

