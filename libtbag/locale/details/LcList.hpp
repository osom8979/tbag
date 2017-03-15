/**
 * @file   LcList.hpp
 * @brief  LcList class prototype.
 * @author zer0
 * @date   2017-03-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCLIST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCLIST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {
namespace details {

TBAG_API std::size_t const getAllLocaleCount() TBAG_NOEXCEPT;
TBAG_API std::vector<std::string> getAllLocale();

} // namespace details
} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_DETAILS_LCLIST_HPP__

