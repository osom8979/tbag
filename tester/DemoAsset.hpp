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
#include <libtbag/res/Asset.hpp>
#include <libtbag/locale/Convert.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// ------------------
namespace test_data {
// ------------------

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
    { return getExeDirPath() / "tester.dir"; }

    CREATE_ASSET_PATH(temp_dir        , getBinaryTempDir());
    CREATE_ASSET_PATH(tester_dir      , getTesterAssetDir());
    CREATE_ASSET_PATH(tester_dir_image, getTesterAssetDir() / "image");
};

/**
 * Create & Remove temp directory.
 *
 * @author zer0
 * @date   2017-03-20
 */
struct TempDir
{
    using Path = libtbag::res::Asset::Path;

    bool remove;
    Path dir;

    TempDir(std::string const & case_name, std::string const & name, bool is_create = true, bool is_remove = true)
            : remove(is_remove)
    {
        dir = DemoAsset().get_temp_dir() / (case_name + std::string("-") + name);
        if (is_create) {
            dir.createDir();
        }
    }

    ~TempDir()
    {
        if (remove) {
            dir.removeAll();
        }
    }

    inline Path getDir() const
    {
        return dir;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TABG_TESTER_TEMP_DIR_NAME
#define TABG_TESTER_TEMP_DIR_NAME __tbag_tester_temp_dir__
#endif

#ifndef TBAG_TESTER_TEMP_DIR
#define TBAG_TESTER_TEMP_DIR(create, remove) \
    auto TABG_TESTER_TEMP_DIR_NAME = ::libtbag::TempDir(test_info_->test_case_name(), test_info_->name(), create, remove); \
    ASSERT_TRUE(TABG_TESTER_TEMP_DIR_NAME.getDir().isDirectory());
#endif

#ifndef TBAG_TESTER_TEMP_DIR_GET
#define TBAG_TESTER_TEMP_DIR_GET() TABG_TESTER_TEMP_DIR_NAME.getDir()
#endif

#endif // __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__

