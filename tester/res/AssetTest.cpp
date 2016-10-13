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

class AssetDemo : public Asset
{
public:
    AssetDemo() = default;
    ~AssetDemo() = default;

public:
    static Path getTestDirPath1()
    { return Asset::getExeDirPath() / "__asset_demo_test1.dir"; }
    static Path getTestDirPath2()
    { return Asset::getExeDirPath() / "__asset_demo_test2.dir"; }

public:
    CREATE_ASSET_PATH(root_res1,      getTestDirPath1());
    CREATE_ASSET_PATH(root_res1_sub1, getTestDirPath1() / "sub1");
    CREATE_ASSET_PATH(root_res1_sub2, getTestDirPath1() / "sub2");

public:
    CREATE_ASSET_PATH(root_res2, getTestDirPath2());
};

TEST(AssetDemoTest, Default)
{
    AssetDemo demo;

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

