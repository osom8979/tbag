/**
 * @file   UnixFileSystem.hpp
 * @brief  UnixFileSystem helper methods.
 * @author zer0
 * @date   2016-12-02
 *
 * @remarks
 *  ISO/IEC 9945 POSIX API.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UNIXFILESYSTEM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UNIXFILESYSTEM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace unix       {

/**
 * Characters prohibited in unix filename.
 *
 * @remarks
 *  - 0x00
 *  - '/'
 */
template <typename CharType>
inline bool isProhibitedChar(CharType v) TBAG_NOEXCEPT
{
    return v == '\0' || v == '/';
}

template <typename CharType>
inline bool isPathSeparatorChar(CharType v) TBAG_NOEXCEPT
{
    return v == PATH_SEPARATOR_OF_POSIX;
}

TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePath();

TBAG_API std::string getRealPath(std::string const & path);

TBAG_API bool createDirectory(std::string const & path, int mode = 0755);
TBAG_API bool removeDirectory(std::string const & path);
TBAG_API bool removeFile(std::string const & path);

TBAG_API bool rename(std::string const & from, std::string const & to);

TBAG_API bool exists(std::string const & path);

TBAG_API bool isDirectory(std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

TBAG_API bool isExecutable(std::string const & path);
TBAG_API bool isWritable(std::string const & path);
TBAG_API bool isReadable(std::string const & path);

TBAG_API std::vector<std::string> scanDir(std::string const & path);

} // namespace unix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_UNIXFILESYSTEM_HPP__

