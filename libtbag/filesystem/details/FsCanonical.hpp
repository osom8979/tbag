/**
 * @file   FsCanonical.hpp
 * @brief  FsCanonical class prototype.
 * @author zer0
 * @date   2017-03-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCANONICAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCANONICAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

namespace unix {
TBAG_API std::string removeLastSeparator(std::string const & path);
TBAG_API std::string removeDuplicateSeparators(std::string const & path);
TBAG_API std::string removeDuplicateSeparatorsWithGeneric(std::string const & path);
TBAG_API std::string removeLastNode(std::string const & path);
} // namespace unix

namespace windows {
TBAG_API std::string removeLastSeparator(std::string const & path);
TBAG_API std::string removeDuplicateSeparators(std::string const & path);
TBAG_API std::string removeDuplicateSeparatorsWithGeneric(std::string const & path);
TBAG_API std::string removeLastNode(std::string const & path);
} // namespace windows

// @formatter:off
inline std::string removeLastSeparator(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeLastSeparator(path); }
inline std::string removeDuplicateSeparators(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeDuplicateSeparators(path); }
inline std::string removeDuplicateSeparatorsWithGeneric(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeDuplicateSeparatorsWithGeneric(path); }
inline std::string removeLastNode(std::string const & path)
{ return TBAG_FS_PLATFORM_NAMESPACE::removeLastNode(path); }
// @formatter:on

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSCANONICAL_HPP__

