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

/**
 * Obtain the back-end string of filesystem.
 */
inline char const * const getFsBackend() TBAG_NOEXCEPT
{
#if defined(__PLATFORM_WINDOWS__)
    return "Windows";
#else
    return "Uv";
#endif
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__

