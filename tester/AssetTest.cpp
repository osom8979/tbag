/**
 * @file   AssetTest.cpp
 * @brief  Asset class tester.
 * @author zer0
 * @date   2016-04-03
 */

#include <gtest/gtest.h>
#include <libtbag/Asset.hpp>
#include <iostream>

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

TEST_F(AssetTest, getHomeDir)
{
    std::string dir = Asset::getHomeDir();
    ASSERT_GT(dir.size(), 0);
}

TEST_F(AssetTest, getExeDir)
{
    std::string dir = Asset::getExeDir();
    ASSERT_GT(dir.size(), 0);
}

