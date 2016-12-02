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

std::string getHomeDir()
{
    return std::string();
}

std::string getExePath()
{
    return std::string();
}

std::string getExeDir()
{
    return std::string();
}

bool createDirectory(std::string const & path)
{
    return false;
}

bool removeDirectory(std::string const & path)
{
    return false;
}

bool rename(std::string const & from, std::string const & to)
{
    return false;
}

bool remove(std::string const & path)
{
    return false;
}

bool exists(std::string const & path)
{
    return false;
}

bool isDirectory(std::string const & path)
{
    return false;
}

bool isRegularFile(std::string const & path)
{
    return false;
}

std::vector<std::string> scanDir(std::string const & path)
{
    return std::vector<std::string>();
}

} // namespace posix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

