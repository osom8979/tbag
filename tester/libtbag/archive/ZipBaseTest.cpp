/**
 * @file   ZipBaseTest.cpp
 * @brief  ZipBase class tester.
 * @author zer0
 * @date   2017-11-17
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/archive/ZipBase.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/encrypt/Md5.hpp>
#include <libtbag/util/BufferInfo.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ZipBaseTest, Default)
{
    tttDir(true, true);
    auto const SAVE_PATH = tttDirGet() / "lena.png.zip.base64";
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";

    std::string encode_string;
    ASSERT_TRUE(encodeZipBase64File(IMAGE_PATH.toString(), encode_string));
    ASSERT_FALSE(encode_string.empty());
    ASSERT_TRUE(decodeZipBase64File(encode_string, SAVE_PATH.toString()));

    util::Buffer original_buffer;
    ASSERT_EQ(Err::E_SUCCESS, filesystem::readFile(IMAGE_PATH.toString(), original_buffer));
    std::string const ORIGINAL_MD5 = encrypt::getMd5(original_buffer);

    util::Buffer decode_buffer;
    ASSERT_EQ(Err::E_SUCCESS, filesystem::readFile(SAVE_PATH.toString(), decode_buffer));
    std::string const DECODE_MD5 = encrypt::getMd5(decode_buffer);

    ASSERT_EQ(ORIGINAL_MD5, DECODE_MD5);
}


