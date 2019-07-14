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
#include <libtbag/crypto/Md5.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::archive;
using namespace libtbag::archive::ex;

template <CompressionMethod v>
struct method_value_t
{
    TBAG_CONSTEXPR static CompressionMethod const value = v;
};

using gzip_method_value = method_value_t<CompressionMethod::CM_GZIP>;
using zlib_method_value = method_value_t<CompressionMethod::CM_ZLIB>;

template <typename T>
struct MethodTypeTestFeature : public ::testing::Test
{
    TBAG_CONSTEXPR static CompressionMethod const get_method_value() TBAG_NOEXCEPT
    { return T::value; }
};

using test_method_types = ::testing::Types<
        gzip_method_value,
        zlib_method_value>;
TYPED_TEST_CASE(MethodTypeTestFeature, test_method_types);

TYPED_TEST(MethodTypeTestFeature, FileToFile)
{
    TBAG_TEST_TEMP_DIR("MethodTypeTestFeature", "FileToFile", true, true);

    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";
    auto const ENCODE_PATH = tttDir_Get() / "lena.png.zb";
    auto const DECODE_PATH = tttDir_Get() / "lena.png";

    auto const LEVEL = TBAG_ZIP_DEFAULT_ENCODE_LEVEL;

    ASSERT_EQ(E_SUCCESS, encodeZipBase64FileToFile(IMAGE_PATH.toString(), ENCODE_PATH.toString(),
                                                   LEVEL, this->get_method_value()));
    ASSERT_EQ(E_SUCCESS, decodeZipBase64FileToFile(ENCODE_PATH.toString(), DECODE_PATH.toString()));

    std::string const ORIGINAL_MD5 = libtbag::crypto::getMd5FromFile(IMAGE_PATH.toString());
    std::string const ENCODE_MD5 = libtbag::crypto::getMd5FromFile(ENCODE_PATH.toString());
    std::string const DECODE_MD5 = libtbag::crypto::getMd5FromFile(DECODE_PATH.toString());

    ASSERT_FALSE(ORIGINAL_MD5.empty());
    ASSERT_FALSE(DECODE_MD5.empty());
    ASSERT_EQ(ORIGINAL_MD5, DECODE_MD5);

    std::string encode;
    ASSERT_EQ(E_SUCCESS, encodeZipBase64FromFile(DECODE_PATH, encode, LEVEL, this->get_method_value()));
    ASSERT_FALSE(encode.empty());

    libtbag::util::Buffer decode;
    ASSERT_EQ(E_SUCCESS, decodeZipBase64FromFile(ENCODE_PATH, decode));
    ASSERT_FALSE(decode.empty());

    std::string const FROM_FILE_ENCODE_MD5 = libtbag::crypto::getMd5(encode);
    std::string const FROM_FILE_DECODE_MD5 = libtbag::crypto::getMd5(decode);
    ASSERT_FALSE(FROM_FILE_ENCODE_MD5.empty());
    ASSERT_FALSE(FROM_FILE_DECODE_MD5.empty());
    ASSERT_EQ(ENCODE_MD5, FROM_FILE_ENCODE_MD5);
    ASSERT_EQ(DECODE_MD5, FROM_FILE_DECODE_MD5);

    auto const ENCODE2_PATH = tttDir_Get() / "lena2.png.zb";
    auto const DECODE2_PATH = tttDir_Get() / "lena2.png";
    ASSERT_EQ(E_SUCCESS, encodeZipBase64ToFile(decode, ENCODE2_PATH.toString(), LEVEL, this->get_method_value()));
    ASSERT_EQ(E_SUCCESS, decodeZipBase64ToFile(encode, DECODE2_PATH.toString()));
    std::string const TO_FILE_ENCODE_MD5 = libtbag::crypto::getMd5FromFile(ENCODE2_PATH.toString());
    std::string const TO_FILE_DECODE_MD5 = libtbag::crypto::getMd5FromFile(DECODE2_PATH.toString());
    ASSERT_FALSE(TO_FILE_ENCODE_MD5.empty());
    ASSERT_FALSE(TO_FILE_DECODE_MD5.empty());
    ASSERT_EQ(ENCODE_MD5, TO_FILE_ENCODE_MD5);
    ASSERT_EQ(DECODE_MD5, TO_FILE_DECODE_MD5);
}

using namespace libtbag::filesystem;

static Err _encodePathToPath(Path const & src, Path const & dest, CompressionMethod method,
                             int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL)
{
    if (!src.exists()) {
        std::cout << "If you want to encode, move the file to the \""
                  << src.toString() << "\" path." << std::endl;
        return E_SUCCESS;
    }

    auto const code = encodeZipBase64FileToFile(src.toString(), dest.toString(),
                                                level, method);
    if (isSuccess(code)) {
        std::cout << "Encode success." << std::endl;
        std::cout << "Please check the \"" << dest.toString() << "\" file." << std::endl;
    } else {
        std::cout << "Encode error: " << code << std::endl;
    }
    return code;
}

TEST(ZipBase64Test, AutoZlibEncode)
{
    auto const EXE_DIR = filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "encode.zlib.source";
    auto const DESTINATION_PATH = EXE_DIR / "encode.zlib.destination";
    auto const METHOD = CompressionMethod::CM_ZLIB;
    ASSERT_EQ(E_SUCCESS, _encodePathToPath(SOURCE_PATH, DESTINATION_PATH, METHOD));
}

TEST(ZipBase64Test, AutoGzipEncode)
{
    auto const EXE_DIR = filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "encode.gzip.source";
    auto const DESTINATION_PATH = EXE_DIR / "encode.gzip.destination";
    auto const METHOD = CompressionMethod::CM_GZIP;
    ASSERT_EQ(E_SUCCESS, _encodePathToPath(SOURCE_PATH, DESTINATION_PATH, METHOD));
}

TEST(ZipBase64Test, AutoDecode)
{
    auto const EXE_DIR = filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "decode.source";
    auto const DESTINATION_PATH = EXE_DIR / "decode.destination";

    if (!SOURCE_PATH.exists()) {
        std::cout << "If you want to decode, move the file to the \""
                  << SOURCE_PATH.toString() << "\" path." << std::endl;
        return;
    }

    auto const code = decodeZipBase64FileToFile(SOURCE_PATH.toString(), DESTINATION_PATH.toString());
    if (isSuccess(code)) {
        std::cout << "Decode success." << std::endl;
        std::cout << "Please check the \"" << DESTINATION_PATH.toString() << "\" file." << std::endl;
    } else {
        std::cout << "Decode error: " << code << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, code);
}

