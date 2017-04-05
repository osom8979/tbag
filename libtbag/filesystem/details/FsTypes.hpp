/**
 * @file   FsTypes.hpp
 * @brief  Filesystem related types.
 * @author zer0
 * @date   2016-12-04
 * @date   2016-12-13 (Rename: FileSystemTypes -> FsTypes)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdlib>
#include <cstdint>

#include <fcntl.h>
#include <sys/stat.h>

#ifndef _WIN_OR_POSIX
# if defined(TBAG_PLATFORM_WINDOWS)
#  define _WIN_OR_POSIX(v) _##v
# else
#  define _WIN_OR_POSIX(v) v
# endif
#endif // _WIN_OR_POSIX

#if defined(TBAG_PLATFORM_WINDOWS)
# define TBAG_FS_PLATFORM_NAMESPACE ::libtbag::filesystem::details::windows
#else
# define TBAG_FS_PLATFORM_NAMESPACE ::libtbag::filesystem::details::unix
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

std::size_t const MAX_PATH_LENGTH_OF_WINDOWS     = 32767;
std::size_t const MAX_PATH_LENGTH_OF_WINDOWS_API =   260; // MAX_PATH define in windows.
std::size_t const MAX_PATH_LENGTH_OF_POSIX_API   =  1024; // PATH_MAX define in posix.

#if defined(TBAG_PLATFORM_WINDOWS)
std::size_t const MAX_PATH_LENGTH = MAX_PATH_LENGTH_OF_WINDOWS_API;
#else
std::size_t const MAX_PATH_LENGTH = MAX_PATH_LENGTH_OF_POSIX_API;
#endif

/** Max path buffer size. */
std::size_t const MAX_PATH_BUFFER_SIZE = MAX_PATH_LENGTH_OF_WINDOWS_API;

char const PATH_SEPARATOR_OF_WINDOWS = '\\';
char const PATH_SEPARATOR_OF_POSIX   = '/';
char const PATH_SEPARATOR_OF_GENERIC = PATH_SEPARATOR_OF_POSIX;

#if defined(TBAG_PLATFORM_WINDOWS)
char const PATH_SEPARATOR = PATH_SEPARATOR_OF_WINDOWS;
#else
char const PATH_SEPARATOR = PATH_SEPARATOR_OF_POSIX;
#endif

char const PATH_SPLITTER_OF_WINDOWS = ';';
char const PATH_SPLITTER_OF_POSIX   = ':';

#if defined(TBAG_PLATFORM_WINDOWS)
char const PATH_SPLITTER = PATH_SPLITTER_OF_WINDOWS;
#else
char const PATH_SPLITTER = PATH_SPLITTER_OF_POSIX;
#endif

char const * const CURRENT_DIRECTORY_SHORTCUT =  ".";
char const * const  PARENT_DIRECTORY_SHORTCUT = "..";
char const * const    HOME_DIRECTORY_SHORTCUT =  "~";

char const * const TEMP_DIRECTORY_SUFFIX = ".temp";

int const CREATE_TEMPDIR_RETRY_COUNT = 10;

static char const * const  TMPDIR_ENV_NAME = "TMPDIR";
static char const * const     TMP_ENV_NAME = "TMP";
static char const * const    TEMP_ENV_NAME = "TEMP";
static char const * const TEMPDIR_ENV_NAME = "TEMPDIR";
static char const * const    HOME_ENV_NAME = "HOME";

static char const * const LAST_ANDROID_TEMP_VALUE = "/data/local/tmp";
static char const * const   LAST_POSIX_TEMP_VALUE = "/tmp";

// No temp environment variables defined.
#if defined(TBAG_PLATFORM_ANDROID)
static char const * const LAST_TEMP_VALUE = LAST_ANDROID_TEMP_VALUE;
#else
static char const * const LAST_TEMP_VALUE = LAST_POSIX_TEMP_VALUE;
#endif

using DirentType = int;

TBAG_CONSTEXPR DirentType const DIRENT_UNKNOWN = 0x01;
TBAG_CONSTEXPR DirentType const DIRENT_FILE    = 0x02;
TBAG_CONSTEXPR DirentType const DIRENT_DIR     = 0x04;
TBAG_CONSTEXPR DirentType const DIRENT_LINK    = 0x08;
TBAG_CONSTEXPR DirentType const DIRENT_FIFO    = 0x10;
TBAG_CONSTEXPR DirentType const DIRENT_SOCKET  = 0x20;
TBAG_CONSTEXPR DirentType const DIRENT_CHAR    = 0x40;
TBAG_CONSTEXPR DirentType const DIRENT_BLOCK   = 0x80;

TBAG_CONSTEXPR DirentType const DIRENT_ALL = DIRENT_UNKNOWN | DIRENT_FILE | DIRENT_DIR | DIRENT_LINK |
                                             DIRENT_FIFO | DIRENT_SOCKET | DIRENT_CHAR | DIRENT_BLOCK;

using ufile = uvpp::ufile;
using binf  = uvpp::binf;

/**
 * Portable @c timespec structure.
 *
 * @remarks
 *  Structure holding an interval broken down into seconds and nanoseconds.
 *
 * @see <http://en.cppreference.com/w/c/chrono/timespec>
 */
struct TimeSpec
{
    long sec;   ///< whole seconds (valid values are >= 0)
    long nsec;  ///< nanoseconds (valid values are [0, 999999999])
};

/** Portable @c stat structure. */
struct FileState
{
    uint64_t dev;       ///< device.
    uint64_t mode;      ///< protection.
    uint64_t nlink;     ///< number of hard links.
    uint64_t uid;       ///< user ID of owner.
    uint64_t gid;       ///< group ID of owner.
    uint64_t rdev;      ///< device type (if inode device).
    uint64_t ino;       ///< inode
    uint64_t size;      ///< total size, in bytes.
    uint64_t blksize;   ///< blocksize for filesystem I/O.
    uint64_t blocks;    ///< number of blocks allocated
    uint64_t flags;     ///< for the libuv.
    uint64_t gen;       ///< for the libuv.

    TimeSpec atim;      ///< time of last access.
    TimeSpec mtim;      ///< time of last modification.
    TimeSpec ctim;      ///< time of last change.
    TimeSpec birthtim;  ///< for the libuv.
};

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

/**
 * @defgroup __DOXYGEN_GROUP__FILE_TYPE__ List of file type.
 * @remarks
 *  POSIX: include <sys/stat.h> or <fcntl.h>
 * @{
 */

static uint32_t const FILE_TYPE_S_IFMT   = _WIN_OR_POSIX(S_IFMT);   ///< type of file mask.
static uint32_t const FILE_TYPE_S_IFCHR  = _WIN_OR_POSIX(S_IFCHR);  ///< character special.
static uint32_t const FILE_TYPE_S_IFDIR  = _WIN_OR_POSIX(S_IFDIR);  ///< directory.
static uint32_t const FILE_TYPE_S_IFREG  = _WIN_OR_POSIX(S_IFREG);  ///< regular file.

#if 0
static uint32_t const FILE_TYPE_S_IFIFO  = S_IFIFO;  ///< named pipe (fifo).
static uint32_t const FILE_TYPE_S_IFBLK  = S_IFBLK;  ///< block special.
static uint32_t const FILE_TYPE_S_IFLNK  = S_IFLNK;  ///< symbolic link.
static uint32_t const FILE_TYPE_S_IFSOCK = S_IFSOCK; ///< socket.
#endif

/**
 * @}
 */

/**
 * @defgroup __DOXYGEN_GROUP__FILE_OPEN_FLAGS__ List of file open flags.
 * @remarks
 *  include <fcntl.h>
 * @{
 */

static uint32_t const FILE_OPEN_FLAG_READ_ONLY  = _WIN_OR_POSIX(O_RDONLY); ///< open for reading only.
static uint32_t const FILE_OPEN_FLAG_WRITE_ONLY = _WIN_OR_POSIX(O_WRONLY); ///< open for writing only.
static uint32_t const FILE_OPEN_FLAG_READ_WRITE = _WIN_OR_POSIX(O_RDWR);   ///< open for reading and writing.
static uint32_t const FILE_OPEN_APPEND          = _WIN_OR_POSIX(O_APPEND); ///< set append mode.

static uint32_t const FILE_OPEN_CREATE          = _WIN_OR_POSIX(O_CREAT);  ///< create if nonexistant.
static uint32_t const FILE_OPEN_TRUNCATE        = _WIN_OR_POSIX(O_TRUNC);  ///< truncate to zero length.
static uint32_t const FILE_OPEN_EXISTS_ERROR    = _WIN_OR_POSIX(O_EXCL);   ///< error if already exists.

#if 0
// !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
static uint32_t const FILE_OPEN_FLAG_ACCESS_ALL = O_ACCMODE;   ///< mask for above modes.
static uint32_t const FILE_OPEN_NON_BLOCK       = O_NONBLOCK;  ///< no delay.
static uint32_t const FILE_OPEN_SHARED_LOCK     = O_SHLOCK;    ///< open with shared file lock.
static uint32_t const FILE_OPEN_EXCLUSIVE_LOCK  = O_EXLOCK;    ///< open with exclusive file lock.
static uint32_t const FILE_OPEN_ASYNC           = O_ASYNC;     ///< signal pgrp when data ready.
static uint32_t const FILE_OPEN_SYNC            = O_FSYNC;     ///< synch I/O file integrity. (source compatibility: do not use)
static uint32_t const FILE_OPEN_NOFOLLOW        = O_NOFOLLOW;  ///< don't follow symlinks.
#endif

static uint32_t const FILE_OPEN_DEFAULT = FILE_OPEN_CREATE | FILE_OPEN_FLAG_READ_WRITE;

/**
 * @}
 */

/**
 * file open flags.
 *
 * @author zer0
 * @date   2017-03-17
 */
struct FileOpenFlags
{
public:
    uint32_t flags;

public:
    TBAG_CONSTEXPR FileOpenFlags() TBAG_NOEXCEPT : flags(FILE_OPEN_DEFAULT)
    { /* EMPTY. */ }
    TBAG_CONSTEXPR FileOpenFlags(uint32_t flag) TBAG_NOEXCEPT : flags(flag)
    { /* EMPTY. */ }
    ~FileOpenFlags()
    { /* EMPTY. */ }

public:
    inline FileOpenFlags & clear() TBAG_NOEXCEPT
    {
        flags = 0;
        return *this;
    }

public:
    // @formatter:off
    inline FileOpenFlags & rdonly(bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_FLAG_READ_ONLY , flag); }
    inline FileOpenFlags & wronly(bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_FLAG_WRITE_ONLY, flag); }
    inline FileOpenFlags & rdwr  (bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_FLAG_READ_WRITE, flag); }
    inline FileOpenFlags & append(bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_APPEND         , flag); }
    inline FileOpenFlags & creat (bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_CREATE         , flag); }
    inline FileOpenFlags & trunc (bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_TRUNCATE       , flag); }
    inline FileOpenFlags & excl  (bool flag = true) TBAG_NOEXCEPT { return set(FILE_OPEN_EXISTS_ERROR   , flag); }
    // @formatter:on

private:
    inline FileOpenFlags & set(uint32_t value, bool flag) TBAG_NOEXCEPT
    {
        if (flag) {
            flags |= value;
        } else {
            flags &= ~value;
        }
        return *this;
    }
};

/**
 * @defgroup __DOXYGEN_GROUP__FILE_MODE__ List of file mode flags.
 * @remarks
 *  include <fcntl.h>
 * @{
 */

// Read, write, execute/search by owner.
#if defined(TBAG_PLATFORM_WINDOWS)
static uint32_t const FILE_MODE_OWNER_READ  = _S_IREAD;  ///< [XSI] R for owner.
static uint32_t const FILE_MODE_OWNER_WRITE = _S_IWRITE; ///< [XSI] W for owner.
#else
static uint32_t const FILE_MODE_OWNER_READ  = S_IRUSR;  ///< [XSI] R for owner.
static uint32_t const FILE_MODE_OWNER_WRITE = S_IWUSR;  ///< [XSI] W for owner.
#endif

#if 0
static uint32_t const FILE_MODE_OWNER_ALL      = S_IRWXU;  ///< [XSI] RWX mask for owner.
static uint32_t const FILE_MODE_OWNER_EXECUTE  = S_IXUSR;  ///< [XSI] X for owner.

// Read, write, execute/search by group.
static uint32_t const FILE_MODE_GROUP_ALL      = S_IRWXG;  ///< [XSI] RWX mask for group.
static uint32_t const FILE_MODE_GROUP_READ     = S_IRGRP;  ///< [XSI] R for group.
static uint32_t const FILE_MODE_GROUP_WRITE    = S_IWGRP;  ///< [XSI] W for group.
static uint32_t const FILE_MODE_GROUP_EXECUTE  = S_IXGRP;  ///< [XSI] X for group.

// Read, write, execute/search by others.
static uint32_t const FILE_MODE_OTHER_ALL      = S_IRWXO;  ///< [XSI] RWX mask for other.
static uint32_t const FILE_MODE_OTHER_READ     = S_IROTH;  ///< [XSI] R for other.
static uint32_t const FILE_MODE_OTHER_WRITE    = S_IWOTH;  ///< [XSI] W for other.
static uint32_t const FILE_MODE_OTHER_EXECUTE  = S_IXOTH;  ///< [XSI] X for other.

static uint32_t const FILE_MODE_S_ISUID        = S_ISUID;  ///< [XSI] set user id on execution.
static uint32_t const FILE_MODE_S_ISGID        = S_ISGID;  ///< [XSI] set group id on execution.
static uint32_t const FILE_MODE_S_ISVTX        = S_ISVTX;  ///< [XSI] directory restrcted delete.
#endif

/**
 * @}
 */

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__

