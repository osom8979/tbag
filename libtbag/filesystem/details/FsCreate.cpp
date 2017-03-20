/**
 * @file   FsCreate.cpp
 * @brief  FsCreate class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsCreate.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

bool createDirectoryEx(std::string const & path, int mode)
{
    uv_fs_t request = {0,};
    int const ERROR_CODE = uv_fs_mkdir(nullptr, &request, path.c_str(), mode, nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool createDirectory(std::string const & path)
{
    return createDirectoryEx(path, 0755);
}

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; retry < CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
        string::createRandomString(&buffer[0], buffer.size());
        cursor = prefix + buffer + suffix;

        if (isDirectory(cursor) == false && createDirectory(cursor)) {
            return cursor;
        }
    }

    return std::string();
}

std::string createDefaultTempDir()
{
    std::string temp_dir = getTempDir();
    if (isDirectory(temp_dir) == false) {
        return std::string();
    }

    if (temp_dir[temp_dir.size() - 1] != PATH_SEPARATOR_OF_POSIX) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR_OF_POSIX;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

