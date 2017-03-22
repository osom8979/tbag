/**
 * @file   FsCommon.hpp
 * @brief  FsCommon class prototype.
 * @author zer0
 * @date   2017-03-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCOMMON_HPP__

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

/**
 * Gets the temp directory.
 *
 * @warning
 *  This method is not thread safe.
 */
TBAG_API std::string getTempDir();

/**
 * Gets the current working directory.
 */
TBAG_API std::string getWorkDir();

/**
 * Gets the current user's home directory.
 *
 * @warning
 *  This method is not thread safe.
 */
TBAG_API std::string getHomeDir();

/**
 * Gets the executable path.
 */
TBAG_API std::string getExePath();

TBAG_API bool createDirectoryEx(std::string const & path, int mode);
TBAG_API bool createDirectory(std::string const & path);

TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size = 6);
TBAG_API std::string createDefaultTempDir();

TBAG_API bool removeDirectory(std::string const & path);
TBAG_API bool removeFile(std::string const & path);
TBAG_API bool removeAll(std::string const & path);

TBAG_API bool rename(std::string const & from, std::string const & to);

/**
 * set file mode creation mask.
 *
 * @return
 *  This system call always succeeds and the previous value of the mask is returned.
 *
 * @see <https://linux.die.net/man/2/umask>
 */
TBAG_API int setUserMask(int mode);

TBAG_API bool checkAccessMode(std::string const & path, int mode);

/**
 * @see <http://linux.die.net/man/2/stat>
 */
TBAG_API bool getState(std::string const & path, FileState * state = nullptr);
TBAG_API bool getStateWithFile(ufile file, FileState * state = nullptr);
TBAG_API bool getStateWithLink(std::string const & path, FileState * state = nullptr);

TBAG_API bool setMode(std::string const & path, int mode);
TBAG_API bool setModeWithFile(ufile file, int mode);

TBAG_API uint64_t getMode(std::string const & path);
TBAG_API uint64_t getPermission(std::string const & path);
TBAG_API uint64_t getFixedPermission(uint64_t mode);

TBAG_API bool checkFileType(std::string const & path, uint64_t type);

TBAG_API bool exists      (std::string const & path);
TBAG_API bool isExecutable(std::string const & path);
TBAG_API bool isWritable  (std::string const & path);
TBAG_API bool isReadable  (std::string const & path);

TBAG_API bool isDirectory  (std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

TBAG_API std::string getRealPath(std::string const & path);

/** @see <https://linux.die.net/man/2/link> */
TBAG_API bool link(std::string const & path, std::string const & new_path);

/** @see <https://linux.die.net/man/2/symlink> */
TBAG_API bool symlink(std::string const & path, std::string const & new_path, int flags);

/** @see <https://linux.die.net/man/2/unlink> */
TBAG_API bool unlink(std::string const & path);

TBAG_API std::vector<std::string> scanDir(std::string const & path, DirentType type = DIRENT_ALL);

TBAG_API ufile open(std::string const & path, int flags = FILE_OPEN_CREATE | FILE_OPEN_FLAG_READ_WRITE, int mode = 0664);
TBAG_API bool close(ufile file);

TBAG_API int  read(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);
TBAG_API int write(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);

TBAG_API int  read2(ufile file, char const * buffer, std::size_t size, int64_t offset);
TBAG_API int write2(ufile file, char const * buffer, std::size_t size, int64_t offset);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCOMMON_HPP__

