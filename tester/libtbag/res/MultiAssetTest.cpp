/**
 * @file   MultiAssetTest.cpp
 * @brief  MultiAsset class tester.
 * @author zer0
 * @date   2016-10-14
 */

#include <gtest/gtest.h>
#include <libtbag/res/MultiAsset.hpp>

#include <fstream>

using namespace libtbag;
using namespace libtbag::res;

TEST(MultiAssetTest, Default)
{
    using Path   = MultiAsset::Path;
    using String = MultiAsset::String;

    Path path1 = Path::getExeDir() / "__multiasset_test_01.dir";
    Path path2 = Path::getExeDir() / "__multiasset_test_02.dir";

    // @formatter:off
    if (path1.exists()) { path1.removeDirWithRecursive(); }
    if (path2.exists()) { path2.removeDirWithRecursive(); }
    // @formatter:on

    String layout1 = "layout1.dir";
    String layout2 = "layout2.dir";

    MultiAsset asset = MultiAsset::create({path1, path2}, {layout1, layout2});

    auto names = asset.getNames();
    ASSERT_EQ(layout1, names.at(0));
    ASSERT_EQ(layout2, names.at(1));

    auto asset1 = asset.getAsset(layout1);
    auto asset2 = asset.getAsset(layout2);

    auto asset1_keys = asset1.getKeys();
    auto asset2_keys = asset2.getKeys();

    ASSERT_TRUE(asset1.init());
    ASSERT_TRUE(asset2.init());

    ASSERT_EQ(2U, asset1_keys.size());
    ASSERT_EQ(2U, asset2_keys.size());

    auto asset1_name1 = asset1.getPath(asset1_keys.at(0)).getName();
    auto asset1_name2 = asset1.getPath(asset1_keys.at(1)).getName();
    auto asset2_name1 = asset2.getPath(asset2_keys.at(0)).getName();
    auto asset2_name2 = asset2.getPath(asset2_keys.at(1)).getName();

    ASSERT_EQ(layout1, asset1_name1);
    ASSERT_EQ(layout1, asset1_name2);
    ASSERT_EQ(layout2, asset2_name1);
    ASSERT_EQ(layout2, asset2_name2);

    ASSERT_TRUE(asset1.getPath(asset1_keys.at(0)).isDirectory());
    ASSERT_TRUE(asset1.getPath(asset1_keys.at(1)).isDirectory());

    auto test_path = (path1 / layout2);
    auto find_path = asset.findWriteableDir(layout2);
    ASSERT_TRUE(test_path == find_path);

    String test_filename = "__file__";
    auto test_file = (path1 / layout2 / test_filename);

    std::string const content = "__blah_blah_blah__";
#if 0
    // TODO: Refactoring this code:
    ASSERT_EQ(content.size(), filesystem::createSimpleTextFile(test_file.getCanonicalString(), &content[0], content.size()));
#endif

    auto find_file = asset.findFile(layout2, test_filename);
    ASSERT_TRUE(test_file == find_file);

    ASSERT_TRUE(path1.removeDirWithRecursive());
    ASSERT_TRUE(path2.removeDirWithRecursive());
}

