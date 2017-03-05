/**
 * @file   String.hpp
 * @brief  String class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROXY_WINDOWS_STRING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROXY_WINDOWS_STRING_HPP__

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

namespace proxy   {
namespace windows {

/**
 * Multi-Byte-String -> Wide-Char-String.
 *
 * @remarks
 *  ACP: The system default Windows ANSI code page.
 */
TBAG_API std::wstring mbsToWcsWithAcp(std::string const & path);

/**
 * Wide-Char-String -> Multi-Byte-String.
 *
 * @remarks
 *  ACP: The system default Windows ANSI code page.
 */
TBAG_API std::string wcsToMbsWithAcp(std::wstring const & path);

} // namespace windows
} // namespace proxy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROXY_WINDOWS_STRING_HPP__

