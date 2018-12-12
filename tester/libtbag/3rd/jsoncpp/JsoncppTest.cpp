/**
 * @file   JsoncppTest.cpp
 * @brief  Jsoncpp library tester.
 * @author zer0
 * @date   2017-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/3rd/jsoncpp/json.h>
#include <libtbag/dom/json/JsonUtils.hpp>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::json;

TEST(JsoncppTest, Default)
{
    char const * TEST_JSON = R"([{"index":1,"name":"first"}, {"index":2,"name":"second"}])";

    Json::Value root;

    ASSERT_TRUE(parse(TEST_JSON, root));
    ASSERT_EQ(2, root.size());
    ASSERT_EQ(1, root[0]["index"].asInt());
    ASSERT_EQ(2, root[1]["index"].asInt());

    ASSERT_FALSE(existsInt(root, "unknown"));
    ASSERT_TRUE(existsInt(root[0], "index"));
    ASSERT_FALSE(existsString(root[0], "index"));
    ASSERT_FALSE(existsInt(root[1], "name"));
    ASSERT_TRUE(existsString(root[1], "name"));
}

