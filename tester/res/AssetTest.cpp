/**
 * @file   AssetTest.cpp
 * @brief  Asset class tester.
 * @author zer0
 * @date   2016-04-03
 */

#include <gtest/gtest.h>
#include <libtbag/predef.hpp>
#include <libtbag/res/Asset.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::res;

TEST(AssetTest, CopyOperators)
{
    Asset asset1 = Asset(Asset::default_setting());
    Asset asset2 = asset1;
    Asset asset3;
    asset3 = asset1;

    ASSERT_EQ(asset1.size(), asset2.size());
    ASSERT_EQ(asset1.size(), asset3.size());
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
    ASSERT_EQ(asset2.size(), asset1.size());
}

TEST(AssetTest, insertDir_getDir)
{
    std::string key = "key";
    std::string value = "value";

    Asset asset;
    asset.insertDir(key, value);
    ASSERT_EQ(1U, asset.size());
    ASSERT_EQ(asset.getDirString(key), value);
}

TEST(AssetTest, getDirs)
{
    Asset asset = Asset(Asset::default_setting());
    auto dirs = asset.getDirs();
    ASSERT_EQ(2U, dirs.size());
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
        // EMPTY.
    }
};

TEST_F(AssetFixtureTest, getHomeDir)
{
    std::string dir = filesystem::common::getHomeDir();
    ASSERT_LT(0U, dir.size());
    ASSERT_EQ(asset.getDirString(Asset::getHomeDirKeyName()), dir);
}

TEST_F(AssetFixtureTest, getExeDir)
{
    std::string dir = filesystem::common::getExeDir();
    ASSERT_LE(1U, dir.size());
    ASSERT_EQ(asset.getDirString(Asset::getExeDirKeyName()), dir);
}

class AssetDemo : public Asset
{
public:
    AssetDemo() {
    }

    virtual ~AssetDemo() {
    }

public:
    CREATE_ASSET_PATH(root_res1, Asset::getExeDirPath() / "__asset_demo_test1.dir");
    CREATE_ASSET_PATH_SUB(root_res1, sub1, "sub1");
    CREATE_ASSET_PATH_SUB(root_res1, sub2, "sub2");

public:
    CREATE_ASSET_PATH(root_res2, Asset::getExeDirPath() / "__asset_demo_test2.dir");
};

TEST(AssetDemoTest, Default)
{
    AssetDemo demo;

    demo.insert_root_res1();
    demo.insert_root_res2();

    demo.get_root_res1();
    demo.get_root_res2();

    if (demo.exists_root_res1_sub1()) {
        demo.remove_root_res1_sub1();
    }
    if (demo.exists_root_res1_sub2()) {
        demo.remove_root_res1_sub2();
    }
    if (demo.exists_root_res1()) {
        demo.remove_root_res1();
    }
    if (demo.exists_root_res2()) {
        demo.remove_root_res2();
    }

    ASSERT_TRUE(demo.create_root_res1());
    ASSERT_TRUE(demo.create_root_res2());
    ASSERT_TRUE(demo.create_root_res1_sub1());
    ASSERT_TRUE(demo.create_root_res1_sub2());

    ASSERT_TRUE(demo.exists_root_res1_sub1());
    ASSERT_TRUE(demo.exists_root_res1_sub2());
    ASSERT_TRUE(demo.exists_root_res1());
    ASSERT_TRUE(demo.exists_root_res2());

    auto list1 = demo.scan_root_res1();
    auto list2 = demo.scan_root_res2();
    ASSERT_EQ(2U, list1.size());
    ASSERT_EQ(0U, list2.size());

    ASSERT_TRUE(demo.remove_root_res1_sub1());
    ASSERT_TRUE(demo.remove_root_res1_sub2());
    ASSERT_TRUE(demo.remove_root_res1());
    ASSERT_TRUE(demo.remove_root_res2());

    ASSERT_FALSE(demo.exists_root_res1_sub1());
    ASSERT_FALSE(demo.exists_root_res1_sub2());
    ASSERT_FALSE(demo.exists_root_res1());
    ASSERT_FALSE(demo.exists_root_res2());
}

