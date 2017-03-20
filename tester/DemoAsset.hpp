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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

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

    Path dir;

    TempDir(std::string const & case_name, std::string const & name, bool is_create = true)
    {
        dir = DemoAsset().get_temp_dir() / (case_name + name);
        if (is_create) {
            dir.createDir();
        }
    }

    ~TempDir()
    {
        dir.removeAll();
    }

    inline Path getDir() const
    {
        return dir;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_CREATE_TESTER_TEMP_DIR
#define TBAG_CREATE_TESTER_TEMP_DIR \
    auto __temp_dir__ = ::libtbag::TempDir(this->test_info_->test_case_name(), this->test_info_->name()); \
    ASSERT_TRUE(__temp_dir__.getDir().isDirectory());
#endif

#ifndef TBAG_GET_TESTER_TEMP_DIR
#define TBAG_GET_TESTER_TEMP_DIR __temp_dir__.getDir()
#endif

#endif // __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__

