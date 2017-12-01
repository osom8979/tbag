/**
 * @file   ZipBase64Test.cpp
 * @brief  ZipBase64 class tester.
 * @author zer0
 * @date   2017-12-01
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/encrypt/Md5.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::archive;
using namespace libtbag::archive::ex;

TEST(ZipBase64Test, FileToFile)
{
    tttDir(true, true);
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";
    auto const ENCODE_PATH = tttDirGet() / "lena.png.zb";
    auto const DECODE_PATH = tttDirGet() / "lena.png";

    ASSERT_TRUE(encodeZipBase64FileToFile(IMAGE_PATH.toString(), ENCODE_PATH.toString()));
    ASSERT_TRUE(decodeZipBase64FileToFile(ENCODE_PATH.toString(), DECODE_PATH.toString()));

    std::string const ORIGINAL_MD5 = encrypt::getMd5FromFile(IMAGE_PATH.toString());
    std::string const   ENCODE_MD5 = encrypt::getMd5FromFile(ENCODE_PATH.toString());
    std::string const   DECODE_MD5 = encrypt::getMd5FromFile(DECODE_PATH.toString());

    ASSERT_FALSE(ORIGINAL_MD5.empty());
    ASSERT_FALSE(  DECODE_MD5.empty());
    ASSERT_EQ(ORIGINAL_MD5, DECODE_MD5);

    std::string  encode;
    util::Buffer decode;
    ASSERT_TRUE(encodeZipBase64FromFile(DECODE_PATH, encode));
    ASSERT_TRUE(decodeZipBase64FromFile(ENCODE_PATH, decode));
    ASSERT_FALSE(encode.empty());
    ASSERT_FALSE(decode.empty());

    std::string const FROM_FILE_ENCODE_MD5 = encrypt::getMd5(encode);
    std::string const FROM_FILE_DECODE_MD5 = encrypt::getMd5(decode);
    ASSERT_FALSE(FROM_FILE_ENCODE_MD5.empty());
    ASSERT_FALSE(FROM_FILE_DECODE_MD5.empty());
    ASSERT_EQ(ENCODE_MD5, FROM_FILE_ENCODE_MD5);
    ASSERT_EQ(DECODE_MD5, FROM_FILE_DECODE_MD5);

    auto const ENCODE2_PATH = tttDirGet() / "lena2.png.zb";
    auto const DECODE2_PATH = tttDirGet() / "lena2.png";
    ASSERT_TRUE(encodeZipBase64ToFile(decode, ENCODE2_PATH.toString()));
    ASSERT_TRUE(decodeZipBase64ToFile(encode, DECODE2_PATH.toString()));
    std::string const TO_FILE_ENCODE_MD5 = encrypt::getMd5FromFile(ENCODE2_PATH.toString());
    std::string const TO_FILE_DECODE_MD5 = encrypt::getMd5FromFile(DECODE2_PATH.toString());
    ASSERT_FALSE(TO_FILE_ENCODE_MD5.empty());
    ASSERT_FALSE(TO_FILE_DECODE_MD5.empty());
    ASSERT_EQ(ENCODE_MD5, TO_FILE_ENCODE_MD5);
    ASSERT_EQ(DECODE_MD5, TO_FILE_DECODE_MD5);
}

TEST(ZipBase64Test, AutoEncode)
{
    auto const EXE_DIR = filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "encode.source";
    auto const DESTINATION_PATH = EXE_DIR / "encode.destination";

    if (SOURCE_PATH.exists() == false) {
        std::cout << "If you want to encode, move the file to the \"" << SOURCE_PATH.toString() << "\" path." << std::endl;
        return;
    }

    bool const ENCODE_RESULT = encodeZipBase64FileToFile(SOURCE_PATH.toString(), DESTINATION_PATH.toString());
    std::cout << "Encode result: " << ENCODE_RESULT << std::endl;
    if (ENCODE_RESULT) {
        std::cout << "Please check the \"" << DESTINATION_PATH.toString() << "\" file." << std::endl;
    }
}

TEST(ZipBase64Test, AutoDecode)
{
    auto const EXE_DIR = filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "decode.source";
    auto const DESTINATION_PATH = EXE_DIR / "decode.destination";

    if (SOURCE_PATH.exists() == false) {
        std::cout << "If you want to decode, move the file to the \"" << SOURCE_PATH.toString() << "\" path." << std::endl;
        return;
    }

    bool const DECODE_RESULT = decodeZipBase64FileToFile(SOURCE_PATH.toString(), DESTINATION_PATH.toString());
    std::cout << "Decode result: " << DECODE_RESULT << std::endl;
    if (DECODE_RESULT) {
        std::cout << "Please check the \"" << DESTINATION_PATH.toString() << "\" file." << std::endl;
    }
}

