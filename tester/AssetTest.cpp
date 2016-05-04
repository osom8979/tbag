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

TEST(AssetTest, CopyOperators)
{
    Asset asset1 = Asset(Asset::default_setting());
    Asset asset2 = asset1;
    Asset asset3;
    asset3 = asset1;

    ASSERT_EQ(asset2.size(), asset1.size());
    ASSERT_EQ(asset3.size(), asset1.size());
}

TEST(AssetTest, MoveOperators)
{
    auto rvalue_test = []() -> Asset {
        return Asset();
    };

    Asset asset0;
    Asset asset1 = std::move(asset0);
    Asset asset2;

    asset2 = rvalue_test();
    ASSERT_EQ(asset1.size(), asset2.size());
}

TEST(AssetTest, insertDir_getDir)
{
    std::string key = "key";
    std::string value = "value";

    Asset asset;
    asset.insertDir(key, value);
    ASSERT_EQ(asset.size(), 1U);
    ASSERT_EQ(value, asset.getDirString(key));
}

class AssetFixtureTest : public ::testing::Test
{
public:
    std::string temp1;
    std::string temp2;
    std::string temp3;

    Asset asset;

public:
    AssetFixtureTest() = default;
    ~AssetFixtureTest() = default;

public:
    virtual void SetUp() {
        temp1 = std::string("1TEMP1");
        temp2 = std::string("2TEMP2");
        temp3 = std::string("3TEMP3");

        asset = Asset(Asset::default_setting());
    }

    virtual void TearDown() {
        __EMPTY_BLOCK__
    }
};

TEST_F(AssetFixtureTest, getHomeDir)
{
    std::string dir = filesystem::Common::getHomeDir();
    ASSERT_GT(dir.size(), 0U);
    ASSERT_EQ(dir, asset.getDirString(Asset::getHomeDirKeyName()));
}

TEST_F(AssetFixtureTest, getExeDir)
{
    std::string dir = filesystem::Common::getExeDir();
    ASSERT_GE(dir.size(), 1U);
    ASSERT_EQ(dir, asset.getDirString(Asset::getExeDirKeyName()));
}

TEST_F(AssetFixtureTest, scanDirs)
{
    auto files = asset.scanDirs(".");
    for (auto cursor : files) {
        std::cout << "* Scan file: " << cursor << std::endl;
    }
    ASSERT_GE(files.size(), 1U);
}

class AssetDemo : public Asset
{
public:
    AssetDemo() {
    }

    virtual ~AssetDemo() {
    }

public:
    CREATE_ASSET_PATH(root_res, Asset::getExeDirPath() / "__asset_demo_test.dir");
};

TEST(AssetDemoTest, create_asset_path)
{
    AssetDemo demo;
    demo.insert_root_res();
    demo.get_root_res();
    ASSERT_TRUE(demo.create_root_res());
    ASSERT_TRUE(demo.remove_root_res());
}

