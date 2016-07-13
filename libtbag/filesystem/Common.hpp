/**
 * @file   Common.hpp
 * @brief  Common filesystem method prototypes.
 * @author zer0
 * @date   2016-04-08
 *
 * @warning
 *  Supports multibyte-string only.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace common     {

std::size_t const MAX_PATH_LENGTH_OF_WINDOWS = 32768;
std::size_t const MAX_PATH_LENGTH_OF_WINDOWS_API = 256;

/**
 * Max path buffer size.
 */
std::size_t const MAX_PATH_BUFFER_SIZE = MAX_PATH_LENGTH_OF_WINDOWS_API;

char const PATH_SEPARATOR_OF_WINDOWS = '\\';
char const PATH_SEPARATOR_OF_POSIX   = '/';

constexpr char const getPathSeparator() noexcept
{
#if defined(__OS_WINDOWS__)
    return PATH_SEPARATOR_OF_WINDOWS;
#else
    return PATH_SEPARATOR_OF_POSIX;
#endif
}

constexpr char const getGenericPathSeparator() noexcept
{
    return PATH_SEPARATOR_OF_POSIX;
}

char const PATH_SPLITTER_OF_WINDOWS = ';';
char const PATH_SPLITTER_OF_POSIX   = ':';

constexpr char const getPathSplitter() noexcept
{
#if defined(__OS_WINDOWS__)
    return PATH_SPLITTER_OF_WINDOWS;
#else
    return PATH_SPLITTER_OF_POSIX;
#endif
}

using DirFunction = int (*)(char * buffer, std::size_t * size);

/** For the libuv miscellaneous function. */
std::string getRepresentationDirectory(DirFunction func);

/** Obtain Working directory. */
std::string getWorkDir();

/** Obtain HOME directory. */
std::string getHomeDir();

/** Obtain executable file path. */
std::string getExePath();

/** Obtain executable file directory. */
std::string getExeDir();

/**
 * List of access mode.
 *
 * @remarks
 *  POSIX: include <sys/unistd.h>
 */
enum AccessModeTable
{
    ACCESS_MODE_EXISTS  = (0   ), ///< F_OK: test for existence of file.
    ACCESS_MODE_EXECUTE = (1<<0), ///< X_OK: test for execute or search permission.
    ACCESS_MODE_WRITE   = (1<<1), ///< W_OK: test for write permission
    ACCESS_MODE_READ    = (1<<2), ///< R_OK: test for read permission.
};

bool isAccessFile(std::string const & path, int mode = ACCESS_MODE_EXISTS);
bool existsFile(std::string const & path);

uint64_t getPermission(std::string const & path);

/**
 * @defgroup __DOXYGEN_GROUP__FILE_TYPE__ List of file type.
 * @remarks
 *  POSIX: include <sys/stat.h>
 * @{
 */

uint32_t const FILE_TYPE_S_IFMT   = 0170000; ///< type of file.
uint32_t const FILE_TYPE_S_IFIFO  = 0010000; ///< named pipe (fifo).
uint32_t const FILE_TYPE_S_IFCHR  = 0020000; ///< character special.
uint32_t const FILE_TYPE_S_IFDIR  = 0040000; ///< directory.
uint32_t const FILE_TYPE_S_IFBLK  = 0060000; ///< block special.
uint32_t const FILE_TYPE_S_IFREG  = 0100000; ///< regular.
uint32_t const FILE_TYPE_S_IFLNK  = 0120000; ///< symbolic link.
uint32_t const FILE_TYPE_S_IFSOCK = 0140000; ///< socket.

/**
 * @}
 */

/**
 * @defgroup __DOXYGEN_GROUP__FILE_MODE__ List of file mode flags.
 * @remarks
 *  include <fcntl.h>
 * @{
 */

// Read, write, execute/search by owner.
uint32_t const FILE_MODE_OWNER_ALL      = 0000700;  ///< [XSI] RWX mask for owner.
uint32_t const FILE_MODE_OWNER_READ     = 0000400;  ///< [XSI] R for owner.
uint32_t const FILE_MODE_OWNER_WRITE    = 0000200;  ///< [XSI] W for owner.
uint32_t const FILE_MODE_OWNER_EXECUTE  = 0000100;  ///< [XSI] X for owner.

// Read, write, execute/search by group.
uint32_t const FILE_MODE_GROUP_ALL      = 0000070;  ///< [XSI] RWX mask for group.
uint32_t const FILE_MODE_GROUP_READ     = 0000040;  ///< [XSI] R for group.
uint32_t const FILE_MODE_GROUP_WRITE    = 0000020;  ///< [XSI] W for group.
uint32_t const FILE_MODE_GROUP_EXECUTE  = 0000010;  ///< [XSI] X for group.

// Read, write, execute/search by others.
uint32_t const FILE_MODE_OTHER_ALL      = 0000007;  ///< [XSI] RWX mask for other.
uint32_t const FILE_MODE_OTHER_READ     = 0000004;  ///< [XSI] R for other.
uint32_t const FILE_MODE_OTHER_WRITE    = 0000002;  ///< [XSI] W for other.
uint32_t const FILE_MODE_OTHER_EXECUTE  = 0000001;  ///< [XSI] X for other.

/**
 * @}
 */

bool checkFileType(std::string const & path, uint64_t type);
bool isDirectory(std::string const & path);
bool isRegularFile(std::string const & path);

std::set<std::string> scanDir(std::string const & dir_path);

bool createDir(std::string const & path, int mode = 0755);
bool removeDir(std::string const & path);

bool rename(std::string const & from, std::string const & to);
bool remove(std::string const & path);

/**
 * @defgroup __DOXYGEN_GROUP__FILE_OPEN_FLAGS__ List of file open flags.
 * @{
 */

uint32_t const FILE_OPEN_FLAG_READ_ONLY  = 0x0000;  ///< open for reading only.
uint32_t const FILE_OPEN_FLAG_WRITE_ONLY = 0x0001;  ///< open for writing only.
uint32_t const FILE_OPEN_FLAG_READ_WRITE = 0x0002;  ///< open for reading and writing.
uint32_t const FILE_OPEN_FLAG_ACCESS_ALL = 0x0003;  ///< mask for above modes.

uint32_t const FILE_OPEN_NON_BLOCK      = 0x0004;  ///< no delay.
uint32_t const FILE_OPEN_APPEND         = 0x0008;  ///< set append mode.

// #if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
uint32_t const FILE_OPEN_SHARED_LOCK    = 0x0010;  ///< open with shared file lock.
uint32_t const FILE_OPEN_EXCLUSIVE_LOCK = 0x0020;  ///< open with exclusive file lock.
uint32_t const FILE_OPEN_ASYNC          = 0x0040;  ///< signal pgrp when data ready.
uint32_t const FILE_OPEN_SYNC           = 0x0080;  ///< synch I/O file integrity. (source compatibility: do not use)
uint32_t const FILE_OPEN_NOFOLLOW       = 0x0100;  ///< don't follow symlinks.
// #endif // (_POSIX_C_SOURCE && !_DARWIN_C_SOURCE)

uint32_t const FILE_OPEN_CREATE         = 0x0200;  ///< create if nonexistant.
uint32_t const FILE_OPEN_TRUNCATE       = 0x0400;  ///< truncate to zero length.
uint32_t const FILE_OPEN_EXISTS_ERROR   = 0x0800;  ///< error if already exists.

/**
 * @}
 */

/**
 * open, creat - open and possibly create a file or device.
 *
 * @return
 *  return the new file descriptor, or -1 if an error occurred.
 *
 * @warning
 *  On Windows libuv uses CreateFileW and thus the file is always opened in binary mode. @n
 *  Because of this the O_BINARY and O_TEXT flags are not supported.
 *
 * @see <http://docs.libuv.org/en/v1.x/fs.html>
 */
int open(std::string const & path, int flags, int mode);

/**
 * @return
 *  returns zero on success. On error, -1 is returned, and errno is set appropriately.
 */
bool close(int fd);

int write(int fd, char const * buffer, std::size_t buffer_size, int64_t offset = -1);
int  read(int fd, char       * buffer, std::size_t buffer_size, int64_t offset = -1);

} // namespace common
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

