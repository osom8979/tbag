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

    using Path   = MultiAsset::Path;
    using String = MultiAsset::String;

    auto path1 = TBAG_TESTER_TEMP_DIR_GET() / std::string("test1");
    auto path2 = TBAG_TESTER_TEMP_DIR_GET() / std::string("test2");
    namespace fs = ::libtbag::filesystem;

    String layout1 = "layout1";
    String layout2 = "layout2";

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

    String filename = "file";
    auto file_path = path1 / layout2 / filename;

    std::string const content = "__blah_blah_blah__";
    fs::File file;
    ASSERT_TRUE(file.open(file_path.getString()));
    int write_size = file.write2(content.c_str(), content.size(), 0);
    file.close();
    ASSERT_EQ(content.size(), static_cast<std::size_t>(write_size));

    auto find_file = asset.findFile(layout2, filename);
    ASSERT_TRUE(file_path == find_file);

    ASSERT_TRUE(path1.removeAll());
    ASSERT_TRUE(path2.removeAll());
}

