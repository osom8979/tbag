/**
 * @file   TpotConfig.cpp
 * @brief  TpotConfig class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/res/TpotConfig.hpp>
#include <libtbag/tpot/res/TpotAsset.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

TpotConfig::TpotConfig()
{
    // EMPTY.
}

TpotConfig::~TpotConfig()
{
    // EMPTY.
}

TpotConfig::String TpotConfig::getRootName() const
{
    return TPOT_CONFIG_XML_ROOT_NAME;
}

TpotConfig::String TpotConfig::getFileName() const
{
    return TPOT_CONFIG_XML_FILE_NAME;
}

TpotConfig::Path TpotConfig::getWorkDir() const
{
    return Path::getWorkDir();
}

TpotConfig::Path TpotConfig::getExeDir() const
{
    return TpotAsset::get_local_config();
}

TpotConfig::Path TpotConfig::getHomeDir() const
{
    return TpotAsset::get_home_config();
}

TpotConfig::Path TpotConfig::getGlobalDir() const
{
    return TpotAsset::get_global_config();
}

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

