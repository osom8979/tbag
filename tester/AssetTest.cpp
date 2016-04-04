/**
 * @file   basic.cpp
 * @brief  Asset class tester.
 * @author zer0
 * @date   2016-04-03
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
    std::string _temp3;

public:
    virtual void SetUp() {
        _temp1 = std::string("1TEMP1");
        _temp2 = std::string("2TEMP2");
        _temp3 = std::string("3TEMP3");
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

TEST_F(AssetTest, splitPaths)
{
    Asset::PathSet set;
    std::string paths;

    paths = _temp1 + GetPathSplitter() + _temp2 + GetPathSplitter() + GetPathSplitter() + _temp3;
    set = Asset::splitPaths(paths);

    ASSERT_EQ(set.size(), 3U);
    ASSERT_EQ(*set.find(_temp1), _temp1);
    ASSERT_EQ(*set.find(_temp2), _temp2);
    ASSERT_EQ(*set.find(_temp3), _temp3);
}

