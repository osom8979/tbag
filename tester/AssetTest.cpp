/**
 * @file   AssetTest.cpp
 * @brief  Asset class tester.
 * @author zer0
 * @date   2016-04-03
 */

#include <gtest/gtest.h>
#include <libtbag/predef.hpp>
#include <libtbag/Asset.hpp>

#include <iostream>

using namespace libtbag;

TEST(AssetStaticTest, CopyOperators)
{
    Asset asset1 = Asset(Asset::default_setting());
    Asset asset2 = asset1;
    Asset asset3;
    asset3 = asset1;

    ASSERT_EQ(asset2.size(), asset1.size());
    ASSERT_EQ(asset3.size(), asset1.size());
}

TEST(AssetStaticTest, MoveOperators)
{
    auto rvalue_test = []() -> Asset {
        return Asset();
    };

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif

    Asset asset1 = std::move(rvalue_test());
    Asset asset2;

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic pop
#endif

    asset2 = rvalue_test();
    ASSERT_EQ(asset1.size(), asset2.size());
}

TEST(AssetStaticTest, insertDir_getDir)
{
    std::string key = "key";
    std::string value = "value";

    Asset asset;
    asset.insertDir(key, value);
    ASSERT_EQ(asset.size(), 1U);
    ASSERT_EQ(value, asset.getDir(key));
}

// Fixture.

class AssetTest : public ::testing::Test
{
public:
    Asset _asset;
    std::string _temp1;
    std::string _temp2;
    std::string _temp3;

public:
    AssetTest() : _asset(Asset::default_setting()) {
    }

public:
    virtual void SetUp() {
        _temp1 = std::string("1TEMP1");
        _temp2 = std::string("2TEMP2");
        _temp3 = std::string("3TEMP3");
    }

    virtual void TearDown() {
    }
};

TEST_F(AssetTest, getHomeDir)
{
    std::string dir = filesystem::Common::getHomeDir();
    ASSERT_GT(dir.size(), 0U);
    ASSERT_EQ(dir, _asset.getDir(Asset::getHomeDirKeyName()));
}

TEST_F(AssetTest, getExeDir)
{
    std::string dir = filesystem::Common::getExeDir();
    ASSERT_GT(dir.size(), 0U);
    ASSERT_EQ(dir, _asset.getDir(Asset::getExeDirKeyName()));
}

TEST_F(AssetTest, scanDirs)
{
    using namespace std;
    auto result = _asset.scanDirs(".");
    for (auto cursor : result) {
        cout << "File: " << cursor << endl;
    }
}

