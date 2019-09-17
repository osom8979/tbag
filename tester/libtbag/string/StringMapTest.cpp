/**
 * @file   StringMapTest.cpp
 * @brief  StringMap class tester.
 * @author zer0
 * @date   2019-09-17
 */

#include <gtest/gtest.h>
#include <libtbag/string/StringMap.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(StringMapTest, Convert)
{
    StringMap m1 = { {"a", "1"}, {"b", "2"} };
    StringUnorderedMap m2 = convert(m1);

    ASSERT_STREQ("1", m2["a"].c_str());
    ASSERT_STREQ("2", m2["b"].c_str());
}

TEST(StringMapTest, Keys)
{
    StringMap m1 = { {"a", "1"}, {"b", "2"} };
    auto const m1keys = keys(m1);
    ASSERT_STREQ("a", m1keys[0].c_str());
    ASSERT_STREQ("b", m1keys[1].c_str());

    StringUnorderedMap m2 = { {"c", "3"}, {"d", "4"} };
    auto m2keys = keys(m2, true);
    ASSERT_STREQ("c", m2keys[0].c_str());
    ASSERT_STREQ("d", m2keys[1].c_str());
}

TEST(StringMapTest, GetInt)
{
    StringMap m1 = { {"a", "1"}, {"b", "2"} };
    int m1a = 0;
    int m1b = 0;
    ASSERT_TRUE(get(m1, "a", m1a));
    ASSERT_TRUE(get(m1, "b", m1b));
    ASSERT_EQ(1, m1a);
    ASSERT_EQ(2, m1b);

    StringUnorderedMap m2 = { {"c", "3"}, {"d", "4"} };
    int m2a = 0;
    int m2b = 0;
    ASSERT_TRUE(get(m2, "c", m2a));
    ASSERT_TRUE(get(m2, "d", m2b));
    ASSERT_EQ(3, m2a);
    ASSERT_EQ(4, m2b);
}

TEST(StringMapTest, OptBool)
{
    StringMap m1 = { {"a", "True"}, {"b", "False"} };
    ASSERT_TRUE(opt<bool>(m1, "a"));
    ASSERT_FALSE(opt<bool>(m1, "b"));

    StringUnorderedMap m2 = { {"c", "No"}, {"d", "OK"} };
    ASSERT_FALSE(opt<bool>(m2, "c"));
    ASSERT_TRUE(opt<bool>(m2, "d"));
}

