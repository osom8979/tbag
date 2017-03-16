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

namespace unix {
TBAG_API std::string getNative(std::string const & path);
TBAG_API std::string getGeneric(std::string const & path);
TBAG_API std::string getRootDir(std::string const & path);
TBAG_API std::string getParent(std::string const & path);

TBAG_API bool isAbsolute(std::string const & path);
TBAG_API bool isRelative(std::string const & path);

TBAG_API std::string appendParent(std::string const & path);
TBAG_API std::vector<std::string> splitNodes(std::string const & path);
} // namespace unix

namespace windows {
TBAG_API std::string getNative(std::string const & path);
TBAG_API std::string getGeneric(std::string const & path);
TBAG_API std::string getRootDir(std::string const & path);
TBAG_API std::string getParent(std::string const & path);

TBAG_API bool isAbsolute(std::string const & path);
TBAG_API bool isRelative(std::string const & path);

TBAG_API std::string appendParent(std::string const & path);
TBAG_API std::vector<std::string> splitNodes(std::string const & path);
} // namespace windows

// @formatter:off
inline std::string getNative(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getNative(path); }
inline std::string getGeneric(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getGeneric(path); }
inline std::string getRootDir(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getRootDir(path); }
inline std::string getParent(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::getParent(path); }

inline bool isAbsolute(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::isAbsolute(path); }
inline bool isRelative(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::isRelative(path); }

inline std::string appendParent(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::appendParent(path); }
inline std::vector<std::string> splitNodes(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::splitNodes(path); }
// @formatter:on

TBAG_API std::vector<std::string> splitNodesWithCanonical(std::string const & path);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSNODE_HPP__

