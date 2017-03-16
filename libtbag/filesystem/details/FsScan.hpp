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

using DirentType = int;

TBAG_CONSTEXPR DirentType const DIRENT_UNKNOWN = 0x01;
TBAG_CONSTEXPR DirentType const DIRENT_FILE    = 0x02;
TBAG_CONSTEXPR DirentType const DIRENT_DIR     = 0x04;
TBAG_CONSTEXPR DirentType const DIRENT_LINK    = 0x08;
TBAG_CONSTEXPR DirentType const DIRENT_FIFO    = 0x10;
TBAG_CONSTEXPR DirentType const DIRENT_SOCKET  = 0x20;
TBAG_CONSTEXPR DirentType const DIRENT_CHAR    = 0x40;
TBAG_CONSTEXPR DirentType const DIRENT_BLOCK   = 0x80;

DirentType const DIRENT_ALL = DIRENT_UNKNOWN | DIRENT_FILE | DIRENT_DIR | DIRENT_LINK |
                              DIRENT_FIFO | DIRENT_SOCKET | DIRENT_CHAR | DIRENT_BLOCK;

TBAG_API std::vector<std::string> scanDir(std::string const & path, DirentType type = DIRENT_ALL);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSSCAN_HPP__

