/**
 * @file   FsUtils.hpp
 * @brief  FsUtils class prototype.
 * @author zer0
 * @date   2017-03-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSUTILS_HPP__

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
TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePath();
} // namespace uv

namespace windows {
TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePathEx(std::size_t extend_buffer_size);
TBAG_API std::string getExePath();
} // namespace windows

inline std::string getTempDir() { return TBAG_FS_PLATFORM_NAMESPACE::getTempDir(); }
inline std::string getWorkDir() { return TBAG_FS_PLATFORM_NAMESPACE::getWorkDir(); }
inline std::string getHomeDir() { return TBAG_FS_PLATFORM_NAMESPACE::getHomeDir(); }
inline std::string getExePath() { return TBAG_FS_PLATFORM_NAMESPACE::getExePath(); }

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSUTILS_HPP__

