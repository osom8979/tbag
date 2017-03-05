/**
 * @file   FsReal.hpp
 * @brief  FsReal class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREAL_HPP__

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
TBAG_API std::string getRealPath(std::string const & path);
} // namespace uv

namespace windows {
TBAG_API std::string getRealPath(std::string const & path);
TBAG_API std::string getLongPathName(std::string const & path);
} // namespace windows

inline std::string getRealPath(std::string const & path)
{
    return TBAG_FS_PLATFORM_NAMESPACE::getRealPath(path);
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSREAL_HPP__

