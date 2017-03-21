/**
 * @file   FsCreate.hpp
 * @brief  FsCreate class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCREATE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCREATE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

TBAG_API bool createDirectoryEx(std::string const & path, int mode);
TBAG_API bool createDirectory(std::string const & path);

TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size = 6);
TBAG_API std::string createDefaultTempDir();

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCREATE_HPP__

