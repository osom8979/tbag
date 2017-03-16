/**
 * @file   FsAttribute.hpp
 * @brief  FsAttribute class prototype.
 * @author zer0
 * @date   2017-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__

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

namespace windows {
TBAG_API bool exists       (std::string const & acp_path);
TBAG_API bool isDirectory  (std::string const & acp_path);
TBAG_API bool isRegularFile(std::string const & acp_path);
TBAG_API bool isExecutable (std::string const & acp_path);
TBAG_API bool isWritable   (std::string const & acp_path);
TBAG_API bool isReadable   (std::string const & acp_path);
} // namespace windows

TBAG_API bool checkAccessMode(std::string const & path, int mode);

TBAG_API bool getState(std::string const & path, FileState * state = nullptr);
TBAG_API bool getStateWithFile(ufile file, FileState * state = nullptr);

TBAG_API uint64_t getMode(std::string const & path);
TBAG_API uint64_t getPermission(std::string const & path);
TBAG_API uint64_t getFixedPermission(uint64_t mode);

TBAG_API bool checkFileType(std::string const & path, uint64_t type);

TBAG_API bool exists      (std::string const & path);
TBAG_API bool isExecutable(std::string const & path);
TBAG_API bool isWritable  (std::string const & path);
TBAG_API bool isReadable  (std::string const & path);

TBAG_API bool isDirectory  (std::string const & path);
TBAG_API bool isRegularFile(std::string const & path);

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_DETAILS_FSATTRIBUTE_HPP__

