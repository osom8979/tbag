/**
 * @file   SysInfo.hpp
 * @brief  SysInfo class prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__

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

namespace system {

TBAG_API int getPageSize();
TBAG_API int getCacheLineSize();

/**
 * Filesystem statistics information.
 *
 * @author zer0
 * @date   2018-01-08
 */
struct FilesystemStatistics
{
    std::size_t    bsize = 0;  ///< Filesystem block size.
    std::size_t   frsize = 0;  ///< Fragment size.
    std::size_t   blocks = 0;  ///< Size of fs in @c frsize units.
    std::size_t    bfree = 0;  ///< Number of free blocks.
    std::size_t   bavail = 0;  ///< Number of free blocks for unprivileged users.
    std::size_t    files = 0;  ///< Number of inodes.
    std::size_t    ffree = 0;  ///< Number of free inodes.
    std::size_t   favail = 0;  ///< Number of free inodes for unprivileged users.
    std::size_t     fsid = 0;  ///< Filesystem ID.
    std::size_t     flag = 0;  ///< Mount flags.
    std::size_t  namemax = 0;  ///< Maximum filename length.

    std::size_t total_byte = 0;
    std::size_t  free_byte = 0;
    std::size_t  used_byte = 0;

    FilesystemStatistics() { /* EMPTY. */ }
    ~FilesystemStatistics() { /* EMPTY. */ }
};

TBAG_API bool getFilesystemInfo(std::string const & path, FilesystemStatistics & result);

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__

