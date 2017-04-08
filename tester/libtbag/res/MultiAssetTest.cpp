/**
 * @file   MultiAssetTest.cpp
 * @brief  MultiAsset class tester.
 * @author zer0
 * @date   2016-10-14
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/MultiAsset.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(MultiAssetTest, Default)
{
    TBAG_TESTER_TEMP_DIR(true, true);

    using Path    = MultiAsset::Path;
    using String  = MultiAsset::String;
    using DyAsset = MultiAsset::DynamicAsset;

    std::string key1 = "KEY1";
    std::string key2 = "KEY2";

    std::string layout1 = "LAYOUT1";
    std::string layout2 = "LAYOUT2";

    auto path1 = TBAG_TESTER_TEMP_DIR_GET() / key1;
    auto path2 = TBAG_TESTER_TEMP_DIR_GET() / key2;
    namespace fs = ::libtbag::filesystem;

    MultiAsset asset = MultiAsset::create({path1, path2}, {layout1, layout2});
    ASSERT_TRUE(asset.initAll());

    auto keys = asset.getKeys();
    ASSERT_EQ(key1, keys.at(0));
    ASSERT_EQ(key2, keys.at(1));

    auto asset1 = asset.at(key1);
    auto asset2 = asset.at(key2);

    auto asset1_keys = asset1.getKeys();
    auto asset2_keys = asset2.getKeys();

    ASSERT_TRUE(asset1.init());
    ASSERT_TRUE(asset2.init());
}

