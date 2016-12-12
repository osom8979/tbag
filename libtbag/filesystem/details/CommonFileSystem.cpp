/**
 * @file   CommonFileSystem.cpp
 * @brief  Common filesystem utilities implementation.
 * @author zer0
 * @date   2016-12-12
 */

#include <libtbag/filesystem/details/CommonFileSystem.hpp>
#include <libtbag/filesystem/details/FileSystemTypes.hpp>
#include <libtbag/filesystem/details/WindowsFileSystem.hpp>
#include <libtbag/filesystem/details/UnixFileSystem.hpp>
#include <libtbag/string/StringUtils.hpp>

#if defined(__PLATFORM_WINDOWS__)
namespace __impl = ::libtbag::filesystem::details::windows;
#else
namespace __impl = ::libtbag::filesystem::details::unix;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
        string::createRandomString(&buffer[0], buffer.size());
        cursor = prefix + buffer + suffix;

        if (__impl::isDirectory(cursor) == false && __impl::createDirectory(cursor)) {
            return cursor;
        }
    }

    return std::string();
}

std::string createDefaultTempDir()
{
    std::string temp_dir = __impl::getTempDir();
    if (__impl::isDirectory(temp_dir) == false) {
        return std::string();
    }

    if (temp_dir[temp_dir.size() - 1] != PATH_SEPARATOR) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

bool removeAll(std::string const & path)
{
    if (__impl::isDirectory(path)) {
        bool result = true;
        for (auto & cursor : __impl::scanDir(path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(path + PATH_SEPARATOR + cursor);
            }
        }
        result &= __impl::removeDirectory(path);
        return result;
    }
    return __impl::removeFile(path);
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

    (*stream) << " * Temp directory: " << __impl::getTempDir() << std::endl;
    (*stream) << " * Home directory: " << __impl::getHomeDir() << std::endl;
    (*stream) << " * Work directory: " << __impl::getWorkDir() << std::endl;
    (*stream) << " * Work directory (realpath): " << __impl::getRealPath(".") << std::endl;
    (*stream) << " * Exe path: "       << __impl::getExePath() << std::endl;
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

