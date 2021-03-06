/**
 * @file   Locale.hpp
 * @brief  Locale utilities.
 * @author zer0
 * @date   2016-12-05
 *
 * @remarks
 *  Locale format:
 *  @code
 *   [language[_country][.codeset][@modifier]]
 *  @endcode
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale {

TBAG_API std::string getDefaultIcuLocaleName();
TBAG_API std::vector<std::string> getAvailableIcuLocale();

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__

