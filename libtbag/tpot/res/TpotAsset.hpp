/**
 * @file   TpotAsset.hpp
 * @brief  TpotAsset class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/res/Asset.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

/**
 * TpotAsset class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
struct TpotAsset : public libtbag::res::Asset
{
    using Asset = libtbag::res::Asset;
    using Path  = Asset::Path;

    static Path getGlobalDir()
    {
#if defined(TBAG_PLATFORM_WINDOWS)
        return Path("C:\\Program Files");
#else
        return Path("/etc");
#endif
    }

    CREATE_ASSET_PATH(global_config, getGlobalDir() / "tpot");
    CREATE_ASSET_PATH(  home_config, Path::getHomeDir() / ".tpot");
    CREATE_ASSET_PATH( local_config, Path::getExeDir());
};

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTASSET_HPP__

