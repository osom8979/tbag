/**
 * @file   FsWinUtils.hpp
 * @brief  FsWinUtils class prototype.
 * @author zer0
 * @date   2017-03-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINUTILS_HPP__

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
namespace windows    {

TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePathEx(std::size_t extend_buffer_size);
TBAG_API std::string getExePath();

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINUTILS_HPP__

