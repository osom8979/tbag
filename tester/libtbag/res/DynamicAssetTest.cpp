/**
 * @file   DynamicAssetTest.cpp
 * @brief  DynamicAssetTest class tester.
 * @author zer0
 * @date   2016-10-13
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/DynamicAsset.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(DynamicAssetTest, Default)
{
    TBAG_TESTER_TEMP_DIR(true, true);

    char const * const KEY = "KEY";
    using Path = DynamicAsset::Path;
    DynamicAsset asset;
    Path path = TBAG_TESTER_TEMP_DIR_GET() / "dynamic_asset";

    ASSERT_TRUE(asset.add(KEY, path));
    ASSERT_TRUE(asset.init());

    ASSERT_TRUE(asset.get(KEY).isDirectory());
    ASSERT_TRUE(asset.exists(KEY));
    ASSERT_TRUE(asset.scan(KEY).empty());

    ASSERT_TRUE(asset.removeAll(KEY));
    ASSERT_FALSE(asset.get(KEY).isDirectory());
}

