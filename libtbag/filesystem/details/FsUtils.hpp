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

#ifndef TBAG_FS_ASSERT_WINDOWS_NOT_IMPLEMENT
#define TBAG_FS_ASSERT_WINDOWS_NOT_IMPLEMENT(retval)    \
    do {                                                \
        if (::libtbag::isWindowsPlatform() == false) {  \
            assert(0 && "Not implement.");              \
            return retval;                              \
        }                                               \
    } while(0)
#endif // TBAG_FS_ASSERT_WINDOWS_NOT_IMPLEMENT

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

TBAG_API std::string getUvTempDir();
TBAG_API std::string getUvWorkDir();
TBAG_API std::string getUvHomeDir();
TBAG_API std::string getUvExePath();

TBAG_API std::string getWindowsTempDir();
TBAG_API std::string getWindowsWorkDir();
TBAG_API std::string getWindowsHomeDir();
TBAG_API std::string getWindowsExePathEx(std::size_t extend_buffer_size);
TBAG_API std::string getWindowsExePath();

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSUTILS_HPP__

