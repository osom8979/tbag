/**
 * @file   UnixFs.cpp
 * @brief  Unix Filesystem helper methods implementation.
 * @author zer0
 * @date   2016-12-02
 * @date   2016-12-13 (Rename: UnixFileSystem -> UnixFs)
 *
 * @remarks
 *  Use the libuv.
 */

#include <libtbag/filesystem/details/UnixFs.hpp>
#include <libtbag/filesystem/details/FsTemplate.hpp-inl>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsCheck.hpp>
#include <libtbag/filesystem/details/FsAttribute.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#include <cstdio>

#include <fcntl.h>
#include <sys/stat.h>

#include <unicode/unistr.h>
#include <uv.h>

STATIC_ASSERT_CHECK_IS_SAME(int, uv_file);

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {
namespace unix       {

// ---------------------
// libuv helper methods.
// ---------------------

std::string createTempDir(std::string const & prefix, std::string const & suffix, std::size_t unique_size)
{
    std::string buffer;
    buffer.resize(unique_size);

    std::string cursor;
    for (int retry = 0; CREATE_TEMPDIR_RETRY_COUNT; ++retry) {
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
    std::string temp_dir = details::getTempDir();
    if (isDirectory(temp_dir) == false) {
        return std::string();
    }

    if (temp_dir[temp_dir.size() - 1] != PATH_SEPARATOR_OF_POSIX) {
        // The operations of UTF-8 and ASCII are the same.
        temp_dir += PATH_SEPARATOR_OF_POSIX;
    }
    return createTempDir(temp_dir, TEMP_DIRECTORY_SUFFIX);
}

std::string getRealPath(std::string const & path)
{
    std::string result;
    uv_fs_t request = {0,};

    if (uv_fs_realpath(nullptr, &request, path.c_str(), nullptr) == 0) {
        result = std::string(static_cast<char*>(request.ptr));
    } else {
        result = path;
    }
    uv_fs_req_cleanup(&request);
    return result;
}

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

bool removeDirectory(std::string const & path)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rmdir(nullptr, &request, path.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

bool removeFile(std::string const & path)
{
    return ::remove(path.c_str()) == 0;
}

bool removeAll(std::string const & path)
{
    if (isDirectory(path)) {
        bool result = true;
        for (auto & cursor : scanDir(path)) {
            if (cursor != CURRENT_DIRECTORY_SHORTCUT && cursor != PARENT_DIRECTORY_SHORTCUT) {
                result &= removeAll(path + PATH_SEPARATOR_OF_POSIX + cursor);
            }
        }
        result &= removeDirectory(path);
        return result;
    }
    return removeFile(path);
}

bool rename(std::string const & from, std::string const & to)
{
    uv_fs_t request;
    int const ERROR_CODE = uv_fs_rename(nullptr, &request, from.c_str(), to.c_str(), nullptr);
    uv_fs_req_cleanup(&request);
    return ERROR_CODE == 0;
}

std::vector<std::string> scanDir(std::string const & path)
{
    std::vector<std::string> result;

    uv_fs_t request;
    uv_dirent_t dictate;

    int const ELEMENT_COUNT = uv_fs_scandir(nullptr, &request, path.c_str(), 0, nullptr);
    if (ELEMENT_COUNT > 0) {
        while (UV_EOF != uv_fs_scandir_next(&request, &dictate)) {
            result.push_back(std::string(dictate.name));
        }
    }
    uv_fs_req_cleanup(&request);

    return result;
}

// --------------------------
// Filesystem path operators.
// --------------------------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return details::isProhibitedNameWithUtf8(utf8_path, uv::isProhibitedChar<UChar>);
}

std::string removeLastSeparatorWithUtf8(std::string const & utf8_path)
{
    return details::removeLastSeparatorWithUtf8(utf8_path, uv::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_POSIX, uv::isPathSeparatorChar<UChar>);
}

std::string removeDuplicateSeparatorsWithGenericUtf8(std::string const & utf8_path)
{
    return details::removeDuplicateSeparators(utf8_path, PATH_SEPARATOR_OF_GENERIC, uv::isPathSeparatorChar<UChar>);
}

std::string getNativeWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithUtf8(utf8_path));
}

std::string getGenericWithUtf8(std::string const & utf8_path)
{
    return removeLastSeparatorWithUtf8(removeDuplicateSeparatorsWithGenericUtf8(utf8_path));
}

std::string getRootDirWithUtf8(std::string const & utf8_path)
{
    if (utf8_path.size() < 1 || utf8_path[0] != PATH_SEPARATOR_OF_POSIX) {
        return std::string();
    }
    return std::string() + PATH_SEPARATOR_OF_POSIX;
}

bool isAbsoluteWithUtf8(std::string const & utf8_path)
{
    return !getRootDirWithUtf8(utf8_path).empty();
}

bool isRelativeWithUtf8(std::string const & utf8_path)
{
    return !isAbsoluteWithUtf8(utf8_path);
}

std::string removeLastNodeWithUtf8(std::string const & utf8_path)
{
    return details::removeLastNodeWithUtf8(utf8_path, uv::isPathSeparatorChar<UChar>);
}

std::string appendParentWithUtf8(std::string const & utf8_path)
{
    return utf8_path + PATH_SEPARATOR_OF_POSIX + PARENT_DIRECTORY_SHORTCUT;
}

std::vector<std::string> splitNodesWithUtf8(std::string const & utf8_path)
{
    std::string const UTF8_DELIMITER = std::string() + PATH_SEPARATOR_OF_POSIX;
    std::string const ROOT = getRootDirWithUtf8(utf8_path);

    std::vector<std::string> result;
    result = string::splitUtf8Tokens(removeDuplicateSeparatorsWithUtf8(utf8_path), UTF8_DELIMITER);

    if (!ROOT.empty()) {
        // Force insert the POSIX root directory.
        result.insert(result.begin(), ROOT);
    }
    return result;
}

} // namespace unix
} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

