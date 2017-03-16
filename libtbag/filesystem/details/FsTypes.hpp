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

#include <cstdlib>

#if defined(__PLATFORM_WINDOWS__)
# define TBAG_FS_PLATFORM_NAMESPACE ::libtbag::filesystem::details::windows
#else
# define TBAG_FS_PLATFORM_NAMESPACE ::libtbag::filesystem::details::unix
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

std::size_t const MAX_PATH_LENGTH_OF_WINDOWS     = 32767 + 1;
std::size_t const MAX_PATH_LENGTH_OF_WINDOWS_API =   260 + 1; // MAX_PATH define in windows.
std::size_t const MAX_PATH_LENGTH_OF_POSIX_API   =  1024 + 1; // PATH_MAX define in posix.

#if defined(__PLATFORM_WINDOWS__)
std::size_t const MAX_PATH_LENGTH = MAX_PATH_LENGTH_OF_WINDOWS_API;
#else
std::size_t const MAX_PATH_LENGTH = MAX_PATH_LENGTH_OF_POSIX_API;
#endif

/** Max path buffer size. */
std::size_t const MAX_PATH_BUFFER_SIZE = MAX_PATH_LENGTH_OF_WINDOWS_API;

char const PATH_SEPARATOR_OF_WINDOWS = '\\';
char const PATH_SEPARATOR_OF_POSIX   = '/';
char const PATH_SEPARATOR_OF_GENERIC = PATH_SEPARATOR_OF_POSIX;

#if defined(__PLATFORM_WINDOWS__)
char const PATH_SEPARATOR = PATH_SEPARATOR_OF_WINDOWS;
#else
char const PATH_SEPARATOR = PATH_SEPARATOR_OF_POSIX;
#endif

char const PATH_SPLITTER_OF_WINDOWS = ';';
char const PATH_SPLITTER_OF_POSIX   = ':';

#if defined(__PLATFORM_WINDOWS__)
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
#if defined(__PLATFORM_ANDROID__)
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

/** Portable @c timespec structure. */
struct TimeSpec
{
    long sec;
    long nsec;
};

/** Portable @c stat structure. */
struct FileState
{
    uint64_t dev;
    uint64_t mode;
    uint64_t nlink;
    uint64_t uid;
    uint64_t gid;
    uint64_t rdev;
    uint64_t ino;
    uint64_t size;
    uint64_t blksize;
    uint64_t blocks;
    uint64_t flags;
    uint64_t gen;

    TimeSpec atim;
    TimeSpec mtim;
    TimeSpec ctim;
    TimeSpec birthtim;
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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__

