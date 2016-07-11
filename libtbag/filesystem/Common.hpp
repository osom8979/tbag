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

uint64_t const FILE_TYPE_S_IFMT   = 0170000; ///< type of file.
uint64_t const FILE_TYPE_S_IFIFO  = 0010000; ///< named pipe (fifo).
uint64_t const FILE_TYPE_S_IFCHR  = 0020000; ///< character special.
uint64_t const FILE_TYPE_S_IFDIR  = 0040000; ///< directory.
uint64_t const FILE_TYPE_S_IFBLK  = 0060000; ///< block special.
uint64_t const FILE_TYPE_S_IFREG  = 0100000; ///< regular.
uint64_t const FILE_TYPE_S_IFLNK  = 0120000; ///< symbolic link.
uint64_t const FILE_TYPE_S_IFSOCK = 0140000; ///< socket.

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

//
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

int write(int fd, char * buffer, std::size_t buffer_size, int64_t offset = -1);

} // namespace common
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

