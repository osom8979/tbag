/**
 * @file   GlobalPref.hpp
 * @brief  GlobalPref class prototype.
 * @author zer0
 * @date   2019-11-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_GLOBALPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_GLOBALPREF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pref/Preferences.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

TBAG_CONSTEXPR char const * const TBAG_GLOBAL_PREFERENCES_KEY = "libtbag::pref::Preferences";
TBAG_CONSTEXPR char const * const TBAG_GLOBAL_PREFERENCES_ENV = "TBAG_GLOBAL_PREFERENCES_XML_PATH";

TBAG_API std::string getGlobalPreferencesXmlPath();
TBAG_API bool setGlobalPreferencesXmlPath(std::string const & path);
TBAG_API std::weak_ptr<Preferences> getGlobalPreferences(char const * path = nullptr, std::size_t length = 0u);

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_GLOBALPREF_HPP__

