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

/**
 * Create directory.
 *
 * @see <https://linux.die.net/man/2/mkdir>
 *
 * @warning
 *  mode is currently not implemented on Windows.
 */
TBAG_API bool createDirectoryEx(std::string const & path, int mode);

/** Create a directory in 0755 mode. */
TBAG_API bool createDirectory(std::string const & path);

/**
 * Create a custom temporary directory.
 *
 * @remarks
 *  Create rule: <code>{prefix} + {random_string:unique_size_length} + {suffix}</code>
 */
TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size = 6);

/**
 * Create a custom temporary directory under the OS's temporary directory.
 *
 * @warning
 *  This method is not thread safe.
 */
TBAG_API std::string createDefaultTempDir();

/**
 * Remove directory.
 *
 * @see <https://linux.die.net/man/2/rmdir>
 */
TBAG_API bool removeDirectory(std::string const & path);

/**
 * Remove file.
 *
 * @remarks
 *  On Windows, uses @c DeleteFileW(). @n
 *  On all other operating systems, uses the @c remove() of standard library.
 */
TBAG_API bool removeFile(std::string const & path);

/** Recursively removes all files & directories. */
TBAG_API bool removeAll(std::string const & path);

/**
 * Change the name or location of a file.
 *
 * @see <https://linux.die.net/man/2/rename>
 */
TBAG_API bool rename(std::string const & from, std::string const & to);

/**
 * Set file mode creation mask.
 *
 * @return
 *  This system call always succeeds and the previous value of the mask is returned.
 *
 * @see <https://linux.die.net/man/2/umask>
 */
TBAG_API int setUserMask(int mode);

/**
 * Check real user's permissions for a file.
 *
 * @param[in] path
 *  File path string.
 * @param[in] mode
 *  See the @c AccessModeTable enumeration.
 *
 * @see <https://linux.die.net/man/2/access>
 *
 * @remarks
 *  Equivalent to @c access() on Unix. Windows uses @c GetFileAttributesW().
 */
TBAG_API bool checkAccessMode(std::string const & path, int mode);
TBAG_API bool exists         (std::string const & path);
TBAG_API bool isExecutable   (std::string const & path);
TBAG_API bool isWritable     (std::string const & path);
TBAG_API bool isReadable     (std::string const & path);

/**
 * Get file status.
 *
 * @see <http://linux.die.net/man/2/stat>
 */
TBAG_API bool getState(std::string const & path, FileState * state = nullptr);
TBAG_API bool getStateWithFile(ufile file, FileState * state = nullptr);
TBAG_API bool getStateWithLink(std::string const & path, FileState * state = nullptr);

/** Obtain the file mode. */
TBAG_API uint64_t getMode(std::string const & path);
TBAG_API uint64_t getPermission(std::string const & path);
TBAG_API uint64_t getFixedPermission(uint64_t mode);

/**
 * Change permissions of a file.
 *
 * @see <https://linux.die.net/man/2/chmod>
 */
TBAG_API bool setMode(std::string const & path, int mode);
TBAG_API bool setModeWithFile(ufile file, int mode);

/** Check the file type. */
TBAG_API bool checkFileType(std::string const & path, uint64_t type);
TBAG_API bool isDirectory  (std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

/** @see <https://linux.die.net/man/3/realpath> */
TBAG_API std::string getRealPath(std::string const & path);

/** @see <https://linux.die.net/man/2/link> */
TBAG_API bool link(std::string const & path, std::string const & new_path);

/** @see <https://linux.die.net/man/2/symlink> */
TBAG_API bool symlink(std::string const & path, std::string const & new_path, int flags);

/** @see <https://linux.die.net/man/2/unlink> */
TBAG_API bool unlink(std::string const & path);

/**
 * Scan a directory for matching entries.
 *
 * @see <https://linux.die.net/man/3/scandir>
 *
 * @warning
 *  This function does not return the "." and ".." entries.
 */
TBAG_API std::vector<std::string> scanDir(std::string const & path, DirentType type = DIRENT_ALL);

/**
 * Open and possibly create a file or device.
 *
 * @see <https://linux.die.net/man/2/open>
 *
 * @warning
 *  Note On Windows libuv uses @c CreateFileW() and thus the file is always opened in binary mode.
 *  Because of this the @c O_BINARY and @c O_TEXT flags are not supported.
 */
TBAG_API ufile open(std::string const & path, int flags = FILE_OPEN_CREATE | FILE_OPEN_FLAG_READ_WRITE, int mode = 0664);

/**
 * Close a file descriptor.
 *
 * @see <https://linux.die.net/man/2/close>
 */
TBAG_API bool close(ufile file);

/**
 * Read or write data into multiple buffers.
 *
 * @see <https://linux.die.net/man/2/preadv>
 */
TBAG_API int  read(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);
TBAG_API int read2(ufile file, char const * buffer, std::size_t size, int64_t offset);

/**
 * Read or write data into multiple buffers.
 *
 * @see <https://linux.die.net/man/2/pwritev>
 */
TBAG_API int  write(ufile file, binf const * infos, std::size_t infos_size, int64_t offset);
TBAG_API int write2(ufile file, char const * buffer, std::size_t size, int64_t offset);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCOMMON_HPP__

