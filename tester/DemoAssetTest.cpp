/**
 * @file   DemoAssetTest.cpp
 * @brief  DemoAsset class tester.
 * @author zer0
 * @date   2016-06-30
 */

#include <gtest/gtest.h>
#include <iostream>
#include <tester/DemoAsset.hpp>

using namespace libtbag;

TEST(DemoAssetTest, Exists)
{
    DemoAsset asset;

    std::cout << "Tester/Asset directory: " << asset.get_tester_dir() << std::endl;
    std::cout << "Tester/Image directory: " << asset.get_tester_dir_image() << std::endl;

    ASSERT_TRUE(asset.exists_tester_dir());
    ASSERT_TRUE(asset.exists_tester_dir_image());

    auto image_list = asset.scan_tester_dir_image();
    ASSERT_EQ(image_list.size(), 1U);
}

TEST(DemoAssetTest, Find)
{
    DemoAsset demo_asset;
    Asset image_asset;

    image_asset.insertDir("image", demo_asset.get_tester_dir_image());
    ASSERT_GT(image_asset.find("lena.png").getString().size(), 0U);
}

