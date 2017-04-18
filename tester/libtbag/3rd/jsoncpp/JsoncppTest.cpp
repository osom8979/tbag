/**
 * @file   JsoncppTest.cpp
 * @brief  Jsoncpp library tester.
 * @author zer0
 * @date   2017-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/3rd/jsoncpp/json.h>

TEST(JsoncppTest, Default)
{
    char const * TEST_JSON = R"([{"index":1,"name":"first"}, {"index":2,"name":"second"}])";

    Json::Value root;
    Json::Reader reader;

    ASSERT_TRUE(reader.parse(TEST_JSON, root));
    ASSERT_EQ(2, root.size());
    ASSERT_EQ(1, root[0]["index"].asInt());
    ASSERT_EQ(2, root[1]["index"].asInt());
}

