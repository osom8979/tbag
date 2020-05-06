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

TEST(JsonUtilsTest, Example_Foreach_Object)
{
    char const * const TEST_JSON_TEXT = R"(
{
    "key1": { "temp": 1 },
    "key2": { "temp": 2 }
})";

    Json::Value json;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, json));

    auto itr = json.begin();
    auto const end = json.end();
    for (; itr != end; ++itr) {
        std::cout << "Key: " << itr.key() << std::endl;
        assert(itr->isObject());
        std::cout << "Value['temp']: " << (*itr)["temp"].asInt() << std::endl;
    }
}

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

TEST(JsonUtilsTest, GetString)
{
    ASSERT_STREQ("test", getForceString(getJsonValue("\"test\"")).c_str());
    ASSERT_STREQ("0.1", getForceString(getJsonValue("0.1")).c_str());
}

TEST(JsonUtilsTest, GetStringArray)
{
    char const * const TEST_JSON_TEXT = R"(["test", 0.1, 99, "a", null, "b"])";
    Json::Value value;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, value));
    auto const result = getStringArray(value);
    ASSERT_EQ(6, result.size());
    ASSERT_STREQ("test", result[0].c_str());
    ASSERT_STREQ("0.1", result[1].c_str());
    ASSERT_STREQ("99", result[2].c_str());
    ASSERT_STREQ("a", result[3].c_str());
    ASSERT_STREQ("", result[4].c_str());
    ASSERT_STREQ("b", result[5].c_str());
}

TEST(JsonUtilsTest, toJsonArray)
{
    auto const json = toJsonArray(std::vector<int>({10, 20, 30}));
    ASSERT_TRUE(json.isArray());
    ASSERT_EQ(3, json.size());
    ASSERT_EQ(10, json[0].asInt());
    ASSERT_EQ(20, json[1].asInt());
    ASSERT_EQ(30, json[2].asInt());
}

TEST(JsonUtilsTest, toJsonObject_1)
{
    auto const json = toJsonObject(std::map<std::string, std::string>({ {"A","B"}, {"C","D"} }));
    ASSERT_TRUE(json.isObject());
    ASSERT_EQ(2, json.size());
    ASSERT_STREQ("B", json["A"].asCString());
    ASSERT_STREQ("D", json["C"].asCString());
}

TEST(JsonUtilsTest, toJsonObject_2)
{
    auto const json = toJsonObject(std::unordered_map<std::string, std::string>({ {"A","B"}, {"C","D"} }));
    ASSERT_TRUE(json.isObject());
    ASSERT_EQ(2, json.size());
    ASSERT_STREQ("B", json["A"].asCString());
    ASSERT_STREQ("D", json["C"].asCString());
}

TEST(JsonUtilsTest, getStringMap)
{
    char const * const TEST_JSON_TEXT = R"(
{
    "key1": "temp1",
    "key2": "temp2",
    "key3": "temp3"
})";
    Json::Value original;
    ASSERT_TRUE(parse(TEST_JSON_TEXT, original));

    auto result = getStringMap(original);
    ASSERT_EQ(3, result.size());
    ASSERT_STREQ("temp1", result["key1"].c_str());
    ASSERT_STREQ("temp2", result["key2"].c_str());
    ASSERT_STREQ("temp3", result["key3"].c_str());
}

