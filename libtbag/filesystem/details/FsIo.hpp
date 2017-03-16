/**
 * @file   FsIo.hpp
 * @brief  FsIo class prototype.
 * @author zer0
 * @date   2017-03-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSIO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSIO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

using ufile = uvpp::ufile;
using binf  = uvpp::binf;

TBAG_API ufile open(std::string const & path, int flags = FILE_OPEN_CREATE | FILE_OPEN_FLAG_READ_WRITE, int mode = 0664);
TBAG_API bool close(ufile file);

TBAG_API int  read(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);
TBAG_API int write(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSIO_HPP__

