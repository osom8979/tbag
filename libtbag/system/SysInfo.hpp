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
    std::size_t  bsize;     ///< Filesystem block size.
    std::size_t  frsize;    ///< Fragment size.
    std::size_t  blocks;    ///< Size of fs in @c frsize units.
    std::size_t  bfree;     ///< Number of free blocks.
    std::size_t  bavail;    ///< Number of free blocks for unprivileged users.
    std::size_t  files;     ///< Number of inodes.
    std::size_t  ffree;     ///< Number of free inodes.
    std::size_t  favail;    ///< Number of free inodes for unprivileged users.
    std::size_t  fsid;      ///< Filesystem ID.
    std::size_t  flag;      ///< Mount flags.
    std::size_t  namemax;   ///< Maximum filename length.

    std::size_t total_byte;
    std::size_t  free_byte;
    std::size_t  used_byte;

    FilesystemStatistics() { clear(); }
    ~FilesystemStatistics() { /* EMPTY. */ }

    inline void clear() TBAG_NOEXCEPT
    {
        bsize      = 0;
        frsize     = 0;
        blocks     = 0;
        bfree      = 0;
        bavail     = 0;
        files      = 0;
        ffree      = 0;
        favail     = 0;
        fsid       = 0;
        flag       = 0;
        namemax    = 0;
        total_byte = 0;
        free_byte  = 0;
        used_byte  = 0;
    }
};

TBAG_API bool getFilesystemInfo(std::string const & path, FilesystemStatistics & result);

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_SYSINFO_HPP__

