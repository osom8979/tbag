/**
 * @file   LcUtils.hpp
 * @brief  LcUtils class prototype.
 * @author zer0
 * @date   2017-03-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {
namespace details {

TBAG_API char * setMinimalCLocale();
TBAG_API char * setSystemDefaultLocale();
TBAG_API char * setAllLocale(char * name);
TBAG_API char * getGlobalLocaleName();

TBAG_API bool isUtf8EncodingName(std::string const & name);

} // namespace details
} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCUTILS_HPP__

