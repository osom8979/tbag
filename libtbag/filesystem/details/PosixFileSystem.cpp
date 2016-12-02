/**
 * @file   PosixFileSystem.cpp
 * @brief  PosixFileSystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 */

#include <libtbag/filesystem/details/PosixFileSystem.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace posix      {

std::string getTempDir()
{
    return std::string();
}

std::string getWorkDir()
{
    return std::string();
}

} // namespace posix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

