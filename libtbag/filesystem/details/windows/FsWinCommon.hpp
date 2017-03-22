/**
 * @file   FsWinCommon.hpp
 * @brief  FsWinCommon class prototype.
 * @author zer0
 * @date   2017-03-21
 *
 * @remarks
 *  Specializations of Windows Platform.
 *
 * @warning
 *  MBS(MultiByte String) is used unless otherwise noted.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINCOMMON_HPP__

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

TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePathEx(std::size_t extend_buffer_size);
TBAG_API std::string getExePath();

TBAG_API bool createDirectory(std::string const & acp_path);
TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size = 6);
TBAG_API std::string createDefaultTempDir();

TBAG_API bool removeDirectory(std::string const & acp_path);
TBAG_API bool removeFile(std::string const & acp_path);
TBAG_API bool removeFileWithUtf8(std::string const & utf8_path);
TBAG_API bool removeAll(std::string const & acp_path);

TBAG_API bool rename(std::string const & acp_path_from, std::string const & acp_path_to);

TBAG_API bool exists       (std::string const & acp_path);
TBAG_API bool isDirectory  (std::string const & acp_path);
TBAG_API bool isRegularFile(std::string const & acp_path);
TBAG_API bool isExecutable (std::string const & acp_path);
TBAG_API bool isWritable   (std::string const & acp_path);
TBAG_API bool isReadable   (std::string const & acp_path);

TBAG_API std::string getRealPath(std::string const & acp_path);
TBAG_API std::string getLongPathName(std::string const & acp_path);

TBAG_API std::vector<std::string> scanDir(std::string const & path);

} // namespace windows
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_WINDOWS_FSWINCOMMON_HPP__

