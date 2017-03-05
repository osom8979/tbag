/**
 * @file   FsRemove.hpp
 * @brief  FsRemove class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREMOVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREMOVE_HPP__

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

namespace uv {
TBAG_API bool removeDirectory(std::string const & path);
TBAG_API bool removeFile(std::string const & path);
TBAG_API bool removeAll(std::string const & path);
} // namespace uv

namespace windows {
TBAG_API bool removeDirectory(std::string const & path);
TBAG_API bool removeFile(std::string const & path);
TBAG_API bool removeAll(std::string const & path);
} // namespace windows

inline bool removeDirectory(std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::removeDirectory(path); }
inline bool removeFile     (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::removeFile(path);      }
inline bool removeAll      (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::removeAll(path);       }

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREMOVE_HPP__

