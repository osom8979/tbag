/**
 * @file   PosixFileSystem.hpp
 * @brief  PosixFileSystem helper methods.
 * @author zer0
 * @date   2016-12-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_POSIXFILESYSTEM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_POSIXFILESYSTEM_HPP__

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

namespace filesystem {
namespace details    {
namespace posix      {

TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();

} // namespace posix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_POSIXFILESYSTEM_HPP__

