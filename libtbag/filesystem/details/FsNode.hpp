/**
 * @file   FsNode.hpp
 * @brief  FsNode class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSNODE_HPP__

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

namespace uv {
TBAG_API std::string removeLastSeparatorWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastNodeWithUtf8(std::string const & utf8_path);

TBAG_API std::string getNativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string getGenericWithUtf8(std::string const & utf8_path);
TBAG_API std::string getRootDirWithUtf8(std::string const & utf8_path);

TBAG_API bool isAbsoluteWithUtf8(std::string const & utf8_path);
TBAG_API bool isRelativeWithUtf8(std::string const & utf8_path);

TBAG_API std::string appendParentWithUtf8(std::string const & utf8_path);
TBAG_API std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path);
} // namespace uv

namespace windows {
TBAG_API std::string removeLastSeparatorWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path);
TBAG_API std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path);
TBAG_API std::string removeLastNodeWithUtf8(std::string const & utf8_path);

TBAG_API std::string getNativeWithUtf8(std::string const & utf8_path);
TBAG_API std::string getGenericWithUtf8(std::string const & utf8_path);
TBAG_API std::string getRootDirWithUtf8(std::string const & utf8_path);

TBAG_API bool isAbsoluteWithUtf8(std::string const & utf8_path);
TBAG_API bool isRelativeWithUtf8(std::string const & utf8_path);

TBAG_API std::string appendParentWithUtf8(std::string const & utf8_path);
TBAG_API std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path);
} // namespace windows

inline std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeLastSeparatorWithUtf8(utf8_path); }
inline std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeDuplicateSeparatorsWithUtf8(utf8_path); }
inline std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeDuplicateSeparatorsWithGenericUtf8(utf8_path); }
inline std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeLastNodeWithUtf8(utf8_path); }

inline std::string getNativeWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getNativeWithUtf8(utf8_path); }
inline std::string getGenericWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getGenericWithUtf8(utf8_path); }
inline std::string getRootDirWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getRootDirWithUtf8(utf8_path); }

inline bool isAbsoluteWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::isAbsoluteWithUtf8(utf8_path); }
inline bool isRelativeWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::isRelativeWithUtf8(utf8_path); }

inline std::string appendParentWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::appendParentWithUtf8(utf8_path); }
inline std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path)
{ return TBAG_FS_PLATFORM_NAMESPACE::splitNodesWithUtf8(utf8_path); }

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSNODE_HPP__

