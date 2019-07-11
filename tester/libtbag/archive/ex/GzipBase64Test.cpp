/**
 * @file   GzipBase64Test.cpp
 * @brief  GzipBase64 class tester.
 * @author zer0
 * @date   2019-07-11
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/archive/ex/GzipBase64.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::archive;
using namespace libtbag::archive::ex;

TEST(GzipBase64Test, FileToFile)
{
    tttDir(true, true);
    auto const IMAGE_PATH = DemoAsset::get_tester_dir_image() / "lena.png";
    auto const ENCODE_PATH = tttDir_Get() / "lena.png.zb";
    auto const DECODE_PATH = tttDir_Get() / "lena.png";
}

TEST(GzipBase64Test, AutoEncode)
{
    auto const EXE_DIR = libtbag::filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "encode.gzip.source";
    auto const DESTINATION_PATH = EXE_DIR / "encode.gzip.destination";

    if (!SOURCE_PATH.exists()) {
        std::cout << "If you want to encode, move the file to the \""
                  << SOURCE_PATH.toString() << "\" path." << std::endl;
        return;
    }
}

TEST(GzipBase64Test, AutoDecode)
{
    auto const EXE_DIR = libtbag::filesystem::Path::getExeDir();
    auto const SOURCE_PATH      = EXE_DIR / "decode.gzip.source";
    auto const DESTINATION_PATH = EXE_DIR / "decode.gzip.destination";

    if (!SOURCE_PATH.exists()) {
        std::cout << "If you want to decode, move the file to the \""
                  << SOURCE_PATH.toString() << "\" path." << std::endl;
        return;
    }
}

