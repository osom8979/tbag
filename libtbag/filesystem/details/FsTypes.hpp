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

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSYPES_HPP__

