/**
 * @file   DemoAsset.hpp
 * @brief  DemoAsset class prototype.
 * @author zer0
 * @date   2016-06-30
 */

#ifndef __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__
#define __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/res/Asset.hpp>
#include <libtbag/locale/Convert.hpp>
#include <libtbag/filesystem/TempDirGuard.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// ------------------
namespace test_data {
// ------------------

// Don't use u8 literal.
std::string const UTF8_GA = "\xea\xb0\x80"; // "가"
std::string const UTF8_NA = "\xeb\x82\x98"; // "나"
std::string const UTF8_DA = "\xeb\x8b\xa4"; // "다"
std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

// ---------------------
} // namespace test_data
// ---------------------

/**
 * DemoAsset class prototype.
 *
 * @author zer0
 * @date   2016-06-30
 */
struct DemoAsset : public libtbag::res::Asset
{
    static Path getTesterAssetDir()
    { return Path(__FILE__).getParent() / "asset"; }

    static Path getBinaryTempDir()
    { return libtbag::filesystem::TempDirGuard::getDefaultPrefix(); }

    CREATE_ASSET_PATH(temp_dir        , getBinaryTempDir());
    CREATE_ASSET_PATH(tester_dir      , getTesterAssetDir());
    CREATE_ASSET_PATH(tester_dir_image, getTesterAssetDir() / "image");
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------


#endif // __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__

