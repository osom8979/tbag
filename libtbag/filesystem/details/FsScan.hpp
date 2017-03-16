/**
 * @file   FsScan.hpp
 * @brief  FsScan class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSSCAN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSSCAN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

namespace windows {
TBAG_API std::vector<std::string> scanDir(std::string const & path);
} // namespace windows

TBAG_API std::vector<std::string> scanDir(std::string const & path, DirentType type = DIRENT_ALL);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSSCAN_HPP__

