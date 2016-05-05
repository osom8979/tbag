/**
 * @file   Common.hpp
 * @brief  Common class prototype.
 * @author zer0
 * @date   2016-04-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <string>
#include <vector>
#include <set>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

std::size_t const MAX_PATH_LENGTH_OF_WINDOWS = 32768;
std::size_t const MAX_PATH_LENGTH_OF_WINDOWS_API = 256;

/**
 * Max path buffer size.
 */
std::size_t const MAX_PATH_BUFFER_SIZE = MAX_PATH_LENGTH_OF_WINDOWS_API;

char const PATH_SEPARATOR_OF_WINDOWS = '\\';
char const PATH_SEPARATOR_OF_POSIX   = '/';

constexpr char const getPathSeparator() noexcept
{
#if defined(__OS_WINDOWS__)
    return PATH_SEPARATOR_OF_WINDOWS;
#else
    return PATH_SEPARATOR_OF_POSIX;
#endif
}

constexpr char const getGenericPathSeparator() noexcept
{
    return PATH_SEPARATOR_OF_POSIX;
}

char const PATH_SPLITTER_OF_WINDOWS = ';';
char const PATH_SPLITTER_OF_POSIX   = ':';

constexpr char const getPathSplitter() noexcept
{
#if defined(__OS_WINDOWS__)
    return PATH_SPLITTER_OF_WINDOWS;
#else
    return PATH_SPLITTER_OF_POSIX;
#endif
}

/**
 * Common filesystem class prototype.
 *
 * @author zer0
 * @date   2016-04-08
 *
 * @warning
 *  Supports multibyte-string only.
 */
class Common : public Noncopyable
{
private:
    constexpr Common() noexcept = delete;
    ~Common() noexcept = delete;

// Representation Directory.
public:
    using DirFunction = int (*)(char * buffer, std::size_t * size);

    /**
     * For the libuv miscellaneous function.
     */
    static std::string getRepresentationDirectory(DirFunction func) {
        std::size_t path_length = MAX_PATH_BUFFER_SIZE;
        std::vector<char> buffer;
        buffer.resize(path_length);

        if (func(&buffer[0], &path_length) != 0) {
            return std::string();
        }
        return std::string(buffer.begin(), buffer.begin() + path_length);
    }

public:
    /**
     * Obtain Working directory.
     */
    static std::string getWorkDir() {
        return getRepresentationDirectory(&uv_cwd);
    }

    /**
     * Obtain HOME directory.
     */
    static std::string getHomeDir() {
        return getRepresentationDirectory(&uv_os_homedir);
    }

    /**
     * Obtain executable file path.
     */
    static std::string getExePath() {
        return getRepresentationDirectory(&uv_exepath);
    }

    /**
     * Obtain executable file directory.
     */
    static std::string getExeDir() {
        std::string path = getExePath();
        if (path.empty()) {
            return std::string();
        }

        // Separate directory & filename.
        std::string separator = std::string() + filesystem::getPathSeparator();
        std::size_t last_separator_index = path.rfind(separator);
        return path.substr(0, last_separator_index);
    }

public:
    /**
     * List of access mode.
     *
     * @remarks
     *  POSIX: include <sys/unistd.h>
     */
    enum AccessModeTable
    {
        ACCESS_MODE_EXISTS  = (0   ), ///< F_OK: test for existence of file.
        ACCESS_MODE_EXECUTE = (1<<0), ///< X_OK: test for execute or search permission.
        ACCESS_MODE_WRITE   = (1<<1), ///< W_OK: test for write permission
        ACCESS_MODE_READ    = (1<<2), ///< R_OK: test for read permission.
    };

    static bool isAccessFile(std::string const & path, int mode = ACCESS_MODE_EXISTS) {
        uv_fs_t request;
        int error_code = uv_fs_access(nullptr, &request, path.c_str(), mode, nullptr);
        uv_fs_req_cleanup(&request);
        return (error_code == 0 ? true : false);
    }

    static bool isExistsFile(std::string const & path) {
        return isAccessFile(path, ACCESS_MODE_EXISTS);
    }

public:
    static uint64_t getPermission(std::string const & path) {
        uint64_t result = 0;
        uv_fs_t  request;

        int error_code = uv_fs_stat(nullptr, &request, path.c_str(), nullptr);
        if (error_code == 0 && request.result == 0) {
            result = request.statbuf.st_mode;
        }
        uv_fs_req_cleanup(&request);

        return result;
    }

public:
    static std::set<std::string> scanDir(std::string const & path) {
        std::set<std::string> result;

        uv_fs_t request;
        uv_dirent_t dictate;

        int element_count = uv_fs_scandir(nullptr, &request, path.c_str(), 0, nullptr);
        if (element_count > 0) {
            while (UV_EOF != uv_fs_scandir_next(&request, &dictate)) {
                result.insert(std::string(dictate.name));
            }
        }
        uv_fs_req_cleanup(&request);

        assert(result.size() == static_cast<std::size_t>(element_count));

        return result;
    }

// Directory.
public:
    static bool createDir(std::string const & path, int mode = 0755) {
        uv_fs_t request;
        int error_code = uv_fs_mkdir(nullptr, &request, path.c_str(), mode, nullptr);
        uv_fs_req_cleanup(&request);

        return (error_code == 0 ? true : false);
    }

    static bool removeDir(std::string const & path) {
        uv_fs_t request;
        int error_code = uv_fs_rmdir(nullptr, &request, path.c_str(), nullptr);
        uv_fs_req_cleanup(&request);

        return (error_code == 0 ? true : false);
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

