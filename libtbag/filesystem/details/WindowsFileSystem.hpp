/**
 * @file   WindowsFileSystem.hpp
 * @brief  WindowsFileSystem helper methods.
 * @author zer0
 * @date   2016-12-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFILESYSTEM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFILESYSTEM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace windows    {

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364992(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API std::string getTempDir();

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364934(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API std::string getWorkDir();

TBAG_API std::string getHomeDir();
TBAG_API std::string getExePath();

TBAG_API std::string getRealPath(std::string const & path);

TBAG_API bool createDirectory(std::string const & path, int mode = 0);
TBAG_API bool removeDirectory(std::string const & path);

TBAG_API bool rename(std::string const & from, std::string const & to);
TBAG_API bool remove(std::string const & path);

TBAG_API bool exists(std::string const & path);

TBAG_API bool isDirectory(std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

TBAG_API bool isExecutable(std::string const & path);
TBAG_API bool isWritable(std::string const & path);
TBAG_API bool isReadable(std::string const & path);

TBAG_API std::vector<std::string> scanDir(std::string const & path);

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFILESYSTEM_HPP__

