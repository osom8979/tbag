/**
 * @file   Common.hpp
 * @brief  Common filesystem method prototypes.
 * @author zer0
 * @date   2016-04-08
 *
 * @warning
 *  Supports multibyte-string only.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/UnixFileSystem.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#if defined(__PLATFORM_WINDOWS__)
namespace __imp = ::libtbag::filesystem::details::windows;
#else
namespace __imp = ::libtbag::filesystem::details::unix;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

using namespace ::libtbag::filesystem::details;
using namespace __imp;

TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix);
TBAG_API std::string createDefaultTempDir();

TBAG_API bool removeAll(std::string const & path);

TBAG_API void printInfos(std::ostream * stream = nullptr);

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

