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
#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/UnixFileSystem.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#if defined(__PLATFORM_WINDOWS__)
namespace __impl = ::libtbag::filesystem::details::windows;
#else
namespace __impl = ::libtbag::filesystem::details::unix;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

// @formatter:off
namespace details {
namespace windows {
TBAG_API bool isProhibitedNameWithUtf8(std::string const utf8_path);
TBAG_API std::string removeLastSeparatorWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path);
TBAG_API std::string getNativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string getGenericWithUtf8(std::string const & utf8_path);
TBAG_API std::string getRootDirWithUtf8(std::string const & utf8_path);
TBAG_API bool isAbsoluteWithUtf8(std::string const & utf8_path);
TBAG_API bool isRelativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastNodeWithUtf8(std::string const & utf8_path);
TBAG_API std::string appendParentWithUtf8(std::string const & path);
TBAG_API std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path);
} // namespace windows

namespace unix {
TBAG_API bool isProhibitedNameWithUtf8(std::string const utf8_path);
TBAG_API std::string removeLastSeparatorWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path);
TBAG_API std::string getNativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string getGenericWithUtf8(std::string const & utf8_path);
TBAG_API std::string getRootDirWithUtf8(std::string const & utf8_path);
TBAG_API bool isAbsoluteWithUtf8(std::string const & utf8_path);
TBAG_API bool isRelativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastNodeWithUtf8(std::string const & utf8_path);
TBAG_API std::string appendParentWithUtf8(std::string const & path);
TBAG_API std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path);
} // namespace unix

} // namespace details
// @formatter:on

using namespace ::libtbag::filesystem::details;
using namespace __impl;

TBAG_API std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size = 6);
TBAG_API std::string createDefaultTempDir();

TBAG_API bool removeAll(std::string const & path);

TBAG_API void printInfos(std::ostream * stream = nullptr);

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

