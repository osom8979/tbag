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
#include <libtbag/filesystem/details/CommonFileSystem.hpp>

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

/**
 * @ref <https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181.aspx>
 *
 * @remarks
 *  - Header: Shlobj.h
 *  - Library: Shell32.lib
 */
TBAG_API std::string getHomeDir();

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms683197(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Winbase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API std::string getExePath(std::size_t extend_buffer_size = MAX_PATH_LENGTH + 1);

/**
 * @ref <https://msdn.microsoft.com/en-us/library/windows/desktop/aa364963(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API std::string getRealPath(std::string const & path);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363855(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API bool createDirectory(std::string const & path, int mode = 0);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365488(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API bool removeDirectory(std::string const & path);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa363915(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API bool removeFile(std::string const & path);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365239(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: WinBase.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API bool rename(std::string const & from, std::string const & to);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb773584(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: Shlwapi.h
 *  - Library: Shlwapi.lib
 */
TBAG_API bool exists(std::string const & path);

TBAG_API bool isDirectory(std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

TBAG_API bool isExecutable(std::string const & path);
TBAG_API bool isWritable(std::string const & path);
TBAG_API bool isReadable(std::string const & path);

/**
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa364418(v=vs.85).aspx>
 * @ref <https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365200(v=vs.85).aspx>
 *
 * @remarks
 *  - Header: FileAPI.h (include Windows.h)
 *  - Library: Kernel32.lib
 */
TBAG_API std::vector<std::string> scanDir(std::string const & path);

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWSFILESYSTEM_HPP__

