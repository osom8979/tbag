/**
 * @file   DemoAssetTest.cpp
 * @brief  DemoAsset class tester.
 * @author zer0
 * @date   2016-06-30
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::res;

TEST(DemoAssetTest, Exists)
{
    std::cout << "Tester/Asset directory: " << DemoAsset::get_tester_dir() << std::endl;
    std::cout << "Tester/Image directory: " << DemoAsset::get_tester_dir_image() << std::endl;

    ASSERT_TRUE(DemoAsset::exists_tester_dir());
    ASSERT_TRUE(DemoAsset::exists_tester_dir_image());

    auto const IMAGE_LIST = DemoAsset::scan_tester_dir_image();
    ASSERT_EQ(1U, IMAGE_LIST.size());
}

