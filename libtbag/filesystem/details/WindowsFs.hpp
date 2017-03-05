/**
 * @file   WindowsFs.hpp
 * @brief  Windows Filesystem helper methods.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: WindowsFileSystem -> WindowsFs)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFS_HPP__

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
namespace windows    {

TBAG_API bool removeDirectory(std::string const & path);
TBAG_API bool removeFile(std::string const & path);
TBAG_API bool removeAll(std::string const & path);
TBAG_API bool rename(std::string const & from, std::string const & to);

TBAG_API std::vector<std::string> scanDir(std::string const & path);

// --------------------------
// Filesystem path operators.
// --------------------------

TBAG_API bool isProhibitedNameWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastSeparatorWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path);
TBAG_API std::string getNativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string getGenericWithUtf8(std::string const & utf8_path);
TBAG_API std::string getRootDirWithUtf8(std::string const & utf8_path);
TBAG_API bool isAbsoluteWithUtf8(std::string const & utf8_path);
TBAG_API bool isRelativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastNodeWithUtf8(std::string const & utf8_path);
TBAG_API std::string appendParentWithUtf8(std::string const & utf8_path);
TBAG_API std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path);

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFS_HPP__

