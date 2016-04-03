/**
 * @file   basic.cpp
 * @brief  Basic tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>
#include <libtbag/Asset.hpp>

using namespace libtbag;

class AssetTest : public ::testing::Test
{
public:
    Asset _asset;
    std::string _temp1;
    std::string _temp2;
    std::string _path_env;

public:
    virtual void SetUp() {
        _temp1 = std::string("TEMP1");
        _temp2 = std::string("TEMP2");
    }

    virtual void TearDown() {
    }
};

TEST_F(AssetTest, PathOperators)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(), ';');
    EXPECT_STREQ(GetHomeEnvName(), "USERPROFILE");
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(), ':');
    EXPECT_STREQ(GetHomeEnvName(), "HOME");
# endif
}

TEST_F(AssetTest, parsePathVariable1)
{
    Asset::PathSet set;
    std::string path_env;

    path_env = std::string() + GetPathSplitter();
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 0U);

    path_env = std::string() + GetPathSplitter() + GetPathSplitter();
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 0U);

    path_env = std::string() + GetPathSplitter() + GetPathSplitter() + _temp1;
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 1U);
    ASSERT_EQ(*set.find(_temp1), _temp1);

    path_env = std::string() + GetPathSplitter() + GetPathSplitter() + _temp1;
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 1U);
    ASSERT_EQ(*set.find(_temp1), _temp1);

    path_env = _temp1;
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 1U);
    ASSERT_EQ(*set.find(_temp1), _temp1);

    path_env = _temp1 + GetPathSplitter();
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 1U);
    ASSERT_EQ(*set.find(_temp1), _temp1);

    path_env = _temp1 + GetPathSplitter() + _temp2;
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 2U);
    ASSERT_EQ(*set.find(_temp1), _temp1);
    ASSERT_EQ(*set.find(_temp2), _temp2);

    path_env = _temp1 + GetPathSplitter() + _temp2 + GetPathSplitter();
    set = Asset::parsePathVariable(path_env);
    ASSERT_EQ(set.size(), 2U);
    ASSERT_EQ(*set.find(_temp1), _temp1);
    ASSERT_EQ(*set.find(_temp2), _temp2);
}

