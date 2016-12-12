/**
 * @file   Common.hpp
 * @brief  Common filesystem method prototypes.
 * @author zer0
 * @date   2016-04-08
 *
 * @warning
 *  Supports UTF-8 (multibyte-string) only.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FileSystemTypes.hpp>
#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/UnixFileSystem.hpp>
#include <libtbag/filesystem/details/CommonFileSystem.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

TBAG_API std::string getTempDir();
TBAG_API std::string getWorkDir();
TBAG_API std::string getHomeDir();
TBAG_API std::string getExePath();
TBAG_API std::string getExeDir();
TBAG_API std::string getRealPath(std::string const & utf8_path);

TBAG_API bool createDirectory(std::string const & utf8_path);
TBAG_API bool removeDirectory(std::string const & utf8_path);
TBAG_API bool removeFile(std::string const & utf8_path);
TBAG_API bool removeAll(std::string const & path);
TBAG_API bool rename(std::string const & utf8_from, std::string const & utf8_to);
TBAG_API bool exists(std::string const & utf8_path);

TBAG_API bool isDirectory(std::string const & utf8_path);
TBAG_API bool isRegularFile(std::string const & utf8_path);
TBAG_API bool isExecutable(std::string const & utf8_path);
TBAG_API bool isWritable(std::string const & utf8_path);
TBAG_API bool isReadable(std::string const & utf8_path);

TBAG_API std::vector<std::string> scanDir(std::string const & utf8_path);

TBAG_API std::size_t getFileSize(std::string const & utf8_path);

// --------------------------
// Filesystem path operators.
// --------------------------

TBAG_API bool isProhibitedName(std::string const & utf8_path);

TBAG_API std::string removeLastSeparator(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparators(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGeneric(std::string const & utf8_path);

TBAG_API std::string getNative(std::string const & utf8_path);
TBAG_API std::string getGeneric(std::string const & utf8_path);
TBAG_API std::string getRootDir(std::string const & utf8_path);
TBAG_API std::string getParent(std::string const & utf8_path);

TBAG_API bool isAbsolute(std::string const & utf8_path);
TBAG_API bool isRelative(std::string const & utf8_path);

TBAG_API std::string removeLastNode(std::string const & utf8_path);
TBAG_API std::string appendParent(std::string const & utf8_path);

TBAG_API std::vector<std::string> splitNodes(std::string const & utf8_path);
TBAG_API std::vector<std::string> splitNodesWithCanonical(std::string const & utf8_path);

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

