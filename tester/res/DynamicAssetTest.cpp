/**
 * @file   DynamicAssetTest.cpp
 * @brief  DynamicAssetTest class tester.
 * @author zer0
 * @date   2016-10-13
 */

#include <gtest/gtest.h>
#include <libtbag/res/DynamicAsset.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(DynamicAssetTest, Default)
{
    char const * const KEY = "KEY";
    DynamicAsset asset;
    DynamicAsset::Path path = DynamicAsset::getExeDirPath() / "dynamic_asset";

    ASSERT_TRUE(asset.addPath(KEY, path));
    ASSERT_TRUE(asset.init());

    ASSERT_TRUE(DynamicAsset::existsDir(asset.getPath(KEY)));
    ASSERT_TRUE(asset.exists(KEY));
    ASSERT_TRUE(asset.scan(KEY).empty());

    ASSERT_TRUE(asset.remove(KEY));
    ASSERT_FALSE(DynamicAsset::existsDir(asset.getPath(KEY)));
}

