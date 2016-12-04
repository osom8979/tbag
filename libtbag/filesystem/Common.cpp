/**
 * @file   Common.cpp
 * @brief  Common filesystem methods.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/filesystem/Common.hpp>
#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/UnixFileSystem.hpp>

#include <iostream>
#include <uv.h>

#if defined(__PLATFORM_WINDOWS__)
#include <Windows.h>
#endif

#if defined(__PLATFORM_WINDOWS__)
namespace __imp = ::libtbag::filesystem::details::windows;
#else
namespace __imp = ::libtbag::filesystem::details::unix;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace common     {

std::string getTempDir()
{
    return __imp::getTempDir();
}

std::string getWorkDir()
{
    return __imp::getWorkDir();
}

std::string getHomeDir()
{
    return __imp::getHomeDir();
}

std::string getExePath()
{
    return __imp::getExePath();
}

bool createDirectory(std::string const & path, int mode)
{
    return __imp::createDirectory(path, mode);
}

bool removeDirectory(std::string const & path)
{
    return __imp::removeDirectory(path);
}

bool rename(std::string const & from, std::string const & to)
{
    return __imp::rename(from, to);
}

bool remove(std::string const & path)
{
    return __imp::remove(path);
}

bool exists(std::string const & path)
{
    return __imp::exists(path);
}

bool isDirectory(std::string const & path)
{
    return __imp::isDirectory(path);
}

bool isRegularFile(std::string const & path)
{
    return __imp::isRegularFile(path);
}

bool isExecutable(std::string const & path)
{
    return __imp::isExecutable(path);
}

bool isWritable(std::string const & path)
{
    return __imp::isWritable(path);
}

bool isReadable(std::string const & path)
{
    return __imp::isReadable(path);
}

std::vector<std::string> scanDir(std::string const & path)
{
    return __imp::scanDir(path);
}

void printInfos(std::ostream * stream)
{
    if (stream == nullptr) {
        stream = &std::cout;
    }

    (*stream) << "Filesystem information:\n";
    if (isWindowsPlatform()) {
        (*stream) << " * Windows Platform Filesystem.\n";
    }
    if (isUnixLikePlatform()) {
        (*stream) << " * Unix-like Platform Filesystem.\n";
    }

    (*stream) << " * Temp directory: " << getTempDir() << std::endl;
    (*stream) << " * Home directory: " << getHomeDir() << std::endl;
    (*stream) << " * Work directory: " << getWorkDir() << std::endl;
    (*stream) << " * Exe path: "       << getExePath() << std::endl;
}

} // namespace common
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

