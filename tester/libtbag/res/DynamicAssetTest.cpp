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
    using Path = DynamicAsset::Path;
    DynamicAsset asset;
    Path path = filesystem::getExeDir() / "dynamic_asset";

    ASSERT_TRUE(asset.addPath(KEY, path));
    ASSERT_TRUE(asset.init());

    ASSERT_TRUE(asset.getPath(KEY).isDirectory());
    ASSERT_TRUE(asset.exists(KEY));
    ASSERT_TRUE(asset.scan(KEY).empty());

    ASSERT_TRUE(asset.remove(KEY));
    ASSERT_FALSE(asset.getPath(KEY).isDirectory());
}

