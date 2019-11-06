/**
 * @file   GlobalPref.cpp
 * @brief  GlobalPref class implementation.
 * @author zer0
 * @date   2019-11-06
 */

#include <libtbag/pref/GlobalPref.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

std::string getGlobalPreferencesXmlPath()
{
    std::string result;
    if (isSuccess(libtbag::uvpp::getEnv(TBAG_GLOBAL_PREFERENCES_ENV, result))) {
        return result;
    }
    return {};
}

bool setGlobalPreferencesXmlPath(std::string const & path)
{
    return isSuccess(libtbag::uvpp::setEnv(TBAG_GLOBAL_PREFERENCES_ENV, path));
}

std::weak_ptr<Preferences> getGlobalPreferences(char const * path, std::size_t length)
{
    auto weak = libtbag::container::findGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    if (!weak.expired()) {
        return weak;
    }

    auto shared = libtbag::container::newGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    if (!shared) {
        // Perhaps generated in another thread.
        return libtbag::container::findGlobalObject<Preferences>(TBAG_GLOBAL_PREFERENCES_KEY);
    }

    assert(static_cast<bool>(shared));

    std::string default_path;
    if (path != nullptr && length >= 1u) {
        default_path = std::string(path, path + length);
        setGlobalPreferencesXmlPath(default_path);
        assert(default_path == getGlobalPreferencesXmlPath());
    } else {
        default_path = getGlobalPreferencesXmlPath();
    }

    auto default_pref = Preferences::loadDefault(default_path);
    assert(!default_pref.empty());

    tDLogI("getGlobalPreferences() Default Preferences loaded: {}", default_path);

    shared->swap(default_pref);
    return shared;
}

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

