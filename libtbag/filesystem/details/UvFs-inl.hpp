/**
 * @file   UvFs-inl.hpp
 * @brief  libuv filesystem utilities.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UVFS_INL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UVFS_INL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

inline TimeSpec toTimeSpec(uv_timespec_t const & uv_time) TBAG_NOEXCEPT
{
    return TimeSpec{uv_time.tv_sec, uv_time.tv_nsec};
}

inline FileState toFileState(uv_stat_t const & uv_stat) TBAG_NOEXCEPT
{
    FileState fs;
    fs.dev      = uv_stat.st_dev;
    fs.mode     = uv_stat.st_mode;
    fs.nlink    = uv_stat.st_nlink;
    fs.uid      = uv_stat.st_uid;
    fs.gid      = uv_stat.st_gid;
    fs.rdev     = uv_stat.st_rdev;
    fs.ino      = uv_stat.st_ino;
    fs.size     = uv_stat.st_size;
    fs.blksize  = uv_stat.st_blksize;
    fs.blocks   = uv_stat.st_blocks;
    fs.flags    = uv_stat.st_flags;
    fs.gen      = uv_stat.st_gen;
    fs.atim     = toTimeSpec(uv_stat.st_atim);
    fs.mtim     = toTimeSpec(uv_stat.st_mtim);
    fs.ctim     = toTimeSpec(uv_stat.st_ctim);
    fs.birthtim = toTimeSpec(uv_stat.st_birthtim);
    return fs;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UVFS_INL_HPP__

