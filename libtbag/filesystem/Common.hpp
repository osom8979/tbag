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

#include <fcntl.h>

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
bool isExecuteFile(std::string const & path);
bool isWriteFile(std::string const & path);
bool isReadFile(std::string const & path);

/**
 * @see <http://linux.die.net/man/2/stat>
 */
uint64_t getStatus(std::string const & path);
uint64_t getPermission(std::string const & path);

/**
 * @defgroup __DOXYGEN_GROUP__FILE_TYPE__ List of file type.
 * @remarks
 *  POSIX: include <sys/stat.h> or <fcntl.h>
 * @{
 */

uint32_t const FILE_TYPE_S_IFMT   = S_IFMT;     ///< type of file.
uint32_t const FILE_TYPE_S_IFIFO  = S_IFIFO;    ///< named pipe (fifo).
uint32_t const FILE_TYPE_S_IFCHR  = S_IFCHR;    ///< character special.
uint32_t const FILE_TYPE_S_IFDIR  = S_IFDIR;    ///< directory.
uint32_t const FILE_TYPE_S_IFBLK  = S_IFBLK;    ///< block special.
uint32_t const FILE_TYPE_S_IFREG  = S_IFREG;    ///< regular.
uint32_t const FILE_TYPE_S_IFLNK  = S_IFLNK;    ///< symbolic link.
uint32_t const FILE_TYPE_S_IFSOCK = S_IFSOCK;   ///< socket.

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
uint32_t const FILE_MODE_OWNER_ALL      = S_IRWXU;  ///< [XSI] RWX mask for owner.
uint32_t const FILE_MODE_OWNER_READ     = S_IRUSR;  ///< [XSI] R for owner.
uint32_t const FILE_MODE_OWNER_WRITE    = S_IWUSR;  ///< [XSI] W for owner.
uint32_t const FILE_MODE_OWNER_EXECUTE  = S_IXUSR;  ///< [XSI] X for owner.

// Read, write, execute/search by group.
uint32_t const FILE_MODE_GROUP_ALL      = S_IRWXG;  ///< [XSI] RWX mask for group.
uint32_t const FILE_MODE_GROUP_READ     = S_IRGRP;  ///< [XSI] R for group.
uint32_t const FILE_MODE_GROUP_WRITE    = S_IWGRP;  ///< [XSI] W for group.
uint32_t const FILE_MODE_GROUP_EXECUTE  = S_IXGRP;  ///< [XSI] X for group.

// Read, write, execute/search by others.
uint32_t const FILE_MODE_OTHER_ALL      = S_IRWXO;  ///< [XSI] RWX mask for other.
uint32_t const FILE_MODE_OTHER_READ     = S_IROTH;  ///< [XSI] R for other.
uint32_t const FILE_MODE_OTHER_WRITE    = S_IWOTH;  ///< [XSI] W for other.
uint32_t const FILE_MODE_OTHER_EXECUTE  = S_IXOTH;  ///< [XSI] X for other.

#if 0
uint32_t const FILE_MODE_S_ISUID        = S_ISUID;  ///< [XSI] set user id on execution.
uint32_t const FILE_MODE_S_ISGID        = S_ISGID;  ///< [XSI] set group id on execution.
uint32_t const FILE_MODE_S_ISVTX        = S_ISVTX;  ///< [XSI] directory restrcted delete.
#endif

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
 * @remarks
 *  include <fcntl.h>
 * @{
 */

uint32_t const FILE_OPEN_FLAG_READ_ONLY  = O_RDONLY;    ///< open for reading only.
uint32_t const FILE_OPEN_FLAG_WRITE_ONLY = O_WRONLY;    ///< open for writing only.
uint32_t const FILE_OPEN_FLAG_READ_WRITE = O_RDWR;      ///< open for reading and writing.
uint32_t const FILE_OPEN_FLAG_ACCESS_ALL = O_ACCMODE;   ///< mask for above modes.

uint32_t const FILE_OPEN_NON_BLOCK      = O_NONBLOCK;   ///< no delay.
uint32_t const FILE_OPEN_APPEND         = O_APPEND;     ///< set append mode.

uint32_t const FILE_OPEN_CREATE         = O_CREAT;      ///< create if nonexistant.
uint32_t const FILE_OPEN_TRUNCATE       = O_TRUNC;      ///< truncate to zero length.
uint32_t const FILE_OPEN_EXISTS_ERROR   = O_EXCL;       ///< error if already exists.

#if 0 // !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
uint32_t const FILE_OPEN_SHARED_LOCK    = O_SHLOCK;     ///< open with shared file lock.
uint32_t const FILE_OPEN_EXCLUSIVE_LOCK = O_EXLOCK;     ///< open with exclusive file lock.
uint32_t const FILE_OPEN_ASYNC          = O_ASYNC;      ///< signal pgrp when data ready.
uint32_t const FILE_OPEN_SYNC           = O_FSYNC;      ///< synch I/O file integrity. (source compatibility: do not use)
uint32_t const FILE_OPEN_NOFOLLOW       = O_NOFOLLOW;   ///< don't follow symlinks.
#endif // (_POSIX_C_SOURCE && !_DARWIN_C_SOURCE)

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
 * @see <http://linux.die.net/man/2/open>
 */
int open(std::string const & path, int flags, int mode);

/**
 * @return
 *  Returns zero on success.
 *
 * @see <http://docs.libuv.org/en/v1.x/fs.html>
 * @see <http://linux.die.net/man/2/close>
 */
bool close(int fd);

/**
 * @return
 *  On success, return the number of bytes written.
 *
 * @see <http://docs.libuv.org/en/v1.x/fs.html>
 * @see <http://linux.die.net/man/2/pwritev>
 */
int write(int fd, char const * buffer, std::size_t buffer_size, int64_t offset = -1);

/**
 * @return
 *  On success, return the number of bytes read.
 *
 * @see <http://docs.libuv.org/en/v1.x/fs.html>
 * @see <http://linux.die.net/man/2/preadv>
 */
int read(int fd, char * buffer, std::size_t buffer_size, int64_t offset = -1);

} // namespace common
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

