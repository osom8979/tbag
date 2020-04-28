/**
 * @file   VersionCompatible.hpp
 * @brief  VersionCompatible class prototype.
 * @author zer0
 * @date   2020-04-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Test version compatibility.
 *
 * @retval E_SUCCESS
 *  Compatible version.
 * @retval E_VERSION
 *  Incompatible version.
 * @retval E_PARSING
 *  Parsing error.
 */
TBAG_API Err testCompatible(Version const & origin,
                            std::string const & requirement);
TBAG_API bool isCompatible(Version const & origin,
                           std::string const & requirement);

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSIONCOMPATIBLE_HPP__

