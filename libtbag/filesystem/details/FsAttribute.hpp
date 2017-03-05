/**
 * @file   FsAttribute.hpp
 * @brief  FsAttribute class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__

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
TBAG_API bool exists       (std::string const & path);
TBAG_API bool isDirectory  (std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);
TBAG_API bool isExecutable (std::string const & path);
TBAG_API bool isWritable   (std::string const & path);
TBAG_API bool isReadable   (std::string const & path);
} // namespace uv

namespace windows {
TBAG_API bool exists       (std::string const & path);
TBAG_API bool isDirectory  (std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);
TBAG_API bool isExecutable (std::string const & path);
TBAG_API bool isWritable   (std::string const & path);
TBAG_API bool isReadable   (std::string const & path);
} // namespace windows

inline bool exists       (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::exists       (path); }
inline bool isDirectory  (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::isDirectory  (path); }
inline bool isRegularFile(std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::isRegularFile(path); }
inline bool isExecutable (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::isExecutable (path); }
inline bool isWritable   (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::isWritable   (path); }
inline bool isReadable   (std::string const & path) { return TBAG_FS_PLATFORM_NAMESPACE::isReadable   (path); }

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__

