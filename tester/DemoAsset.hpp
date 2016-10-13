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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * DemoAsset class prototype.
 *
 * @author zer0
 * @date   2016-06-30
 */
class DemoAsset : public libtbag::res::Asset
{
public:
    DemoAsset() = default;
    ~DemoAsset() = default;

public:
    static Path getTesterAssetDir()
    { return (Path(__FILE__).getParent() / "asset"); }

public:
    CREATE_ASSET_PATH(tester_dir,       getTesterAssetDir());
    CREATE_ASSET_PATH(tester_dir_image, getTesterAssetDir() / "image");
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__TESTER_DEMOASSET_HPP__

