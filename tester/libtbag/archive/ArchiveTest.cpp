/**
 * @file   ArchiveTest.cpp
 * @brief  Archive class tester.
 * @author zer0
 * @date   2019-02-25
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/archive/Archive.hpp>
#include <libtbag/crypto/Md5.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ArchiveTest, WriteArchive)
{
    auto const IMAGE_FILE_NAME = "lena.png";
    auto const ARCHIVE_FILE_NAME = "save.7z";
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / IMAGE_FILE_NAME;

    tttDir_Automatic();
    auto const OUTPUT_PATH = tttDir_Get() / ARCHIVE_FILE_NAME;
    auto const EXTRACT_PATH = tttDir_Get() / "extract";

    ASSERT_EQ(1, compressArchive(OUTPUT_PATH, {IMAGE_PATH.toString()}, COMPRESS_FORMAT_7ZIP));
    ASSERT_TRUE(OUTPUT_PATH.exists());

    auto const FILES = getArchiveFileList(OUTPUT_PATH);
    ASSERT_EQ(1, FILES.size());
    ASSERT_EQ(IMAGE_FILE_NAME, FILES[0]);

    if (EXTRACT_PATH.exists()) {
        ASSERT_TRUE(EXTRACT_PATH.removeAll());
    }
    ASSERT_TRUE(EXTRACT_PATH.createDir());

    ASSERT_EQ(1, decompressArchive(OUTPUT_PATH, EXTRACT_PATH));
    auto const EXTRACT_FILES = EXTRACT_PATH.scanNameOnly();
    ASSERT_EQ(1, EXTRACT_FILES.size());
    ASSERT_EQ(IMAGE_FILE_NAME, EXTRACT_FILES[0]);

    auto const ORIGINAL_MD5 = libtbag::crypto::getMd5FromFile(IMAGE_PATH.getString());
    auto const EXTRACT_MD5 = libtbag::crypto::getMd5FromFile((EXTRACT_PATH / IMAGE_FILE_NAME).toString());
    ASSERT_EQ(ORIGINAL_MD5, EXTRACT_MD5);
}

