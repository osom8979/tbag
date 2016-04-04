/**
 * @file   Strings.cpp
 * @brief  Strings class tester.
 * @author zer0
 * @date   2016-04-04
 */

#include <gtest/gtest.h>
#include <libtbag/Strings.hpp>

using namespace libtbag;

TEST(Strings, splitTokens)
{
    std::string delimiter = "//";
    std::string temp1 = std::string("1TEMP1");
    std::string temp2 = std::string("2TEMP2");

    std::set<std::string> path_set;
    std::string content;

    content = std::string() + delimiter;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 0U);

    content = std::string() + delimiter + delimiter;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 0U);

    content = std::string() + delimiter + delimiter + temp1;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 1U);
    ASSERT_EQ(*path_set.find(temp1), temp1);

    content = std::string() + delimiter + delimiter + temp1;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 1U);
    ASSERT_EQ(*path_set.find(temp1), temp1);

    content = temp1;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 1U);
    ASSERT_EQ(*path_set.find(temp1), temp1);

    content = temp1 + delimiter;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 1U);
    ASSERT_EQ(*path_set.find(temp1), temp1);

    content = temp1 + delimiter + temp2;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 2U);
    ASSERT_EQ(*path_set.find(temp1), temp1);
    ASSERT_EQ(*path_set.find(temp2), temp2);

    content = temp1 + delimiter + temp2 + delimiter;
    path_set = strings::splitTokens(content, delimiter);
    ASSERT_EQ(path_set.size(), 2U);
    ASSERT_EQ(*path_set.find(temp1), temp1);
    ASSERT_EQ(*path_set.find(temp2), temp2);
}
