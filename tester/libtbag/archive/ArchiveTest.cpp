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

TEST(ArchiveTest, WriteArchive)
{
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";

    // Save & Load.
    tttDir_Automatic();
    auto const OUTPUT_PATH = tttDir_Get() / "save.7z";

    ASSERT_EQ(1, compressArchive(OUTPUT_PATH, {IMAGE_PATH.toString()}, COMPRESS_FORMAT_7ZIP));
    ASSERT_TRUE(OUTPUT_PATH.exists());
}

