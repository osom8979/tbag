/**
 * @file   GlobalPref.cpp
 * @brief  GlobalPref class implementation.
 * @author zer0
 * @date   2019-11-06
 */

#include <libtbag/pref/GlobalPref.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

static std::size_t const MAX_PATH_LENGTH = libtbag::filesystem::details::MAX_PATH_LENGTH;
static char g_global_preferences_xml_path[MAX_PATH_LENGTH+1] = {0,};

std::string getGlobalPreferencesXmlPathFromEnv()
{
    std::string result;
    if (isSuccess(libtbag::uvpp::getEnv(TBAG_GLOBAL_PREFERENCES_ENV, result))) {
        return result;
    }
    return {};
}

bool setGlobalPreferencesXmlPathToEnv(std::string const & path)
{
    return isSuccess(libtbag::uvpp::setEnv(TBAG_GLOBAL_PREFERENCES_ENV, path));
}

std::string getGlobalPreferencesXmlPath()
{
    return g_global_preferences_xml_path;
}

bool setGlobalPreferencesXmlPath(std::string const & path)
{
    auto const path_size = path.size();
    if (path_size > MAX_PATH_LENGTH) {
        return false;
    }
    ::strncpy(g_global_preferences_xml_path, path.c_str(), path_size);
    g_global_preferences_xml_path[path_size] = '\0';
    return true;
}

std::weak_ptr<Preferences> getGlobalPreferences()
{
    auto weak = libtbag::container::findGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    if (!weak.expired()) {
        return weak;
    }

    auto shared = libtbag::container::newGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    if (shared) {
        return shared;
    } else {
        // Perhaps generated in another thread.
        return libtbag::container::findGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    }
}

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

