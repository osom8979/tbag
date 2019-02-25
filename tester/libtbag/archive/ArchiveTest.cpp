/**
 * @file   ArchiveTest.cpp
 * @brief  Archive class tester.
 * @author zer0
 * @date   2019-02-25
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/archive/Archive.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ArchiveTest, Write7zip)
{
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";

    // Save & Load.
    //tttDir_Automatic();
    tttDir(true, false);
    auto const OUTPUT_PATH = tttDir_Get() / "save.7z";

    ASSERT_EQ(1, write7zip(OUTPUT_PATH, {IMAGE_PATH.toString()}));
}

