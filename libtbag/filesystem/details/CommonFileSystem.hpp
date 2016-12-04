/**
 * @file   CommonFileSystem.hpp
 * @brief  CommonFileSystem class prototype.
 * @author zer0
 * @date   2016-12-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_COMMONFILESYSTEM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_COMMONFILESYSTEM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

#if defined(__PLATFORM_WINDOWS__)
std::size_t const MAX_PATH_LENGTH =  256; // MAX_PATH define in windows.
#else
std::size_t const MAX_PATH_LENGTH = 1024; // PATH_MAX define in posix.
#endif

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_COMMONFILESYSTEM_HPP__

