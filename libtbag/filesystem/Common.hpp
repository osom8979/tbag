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
#include <libtbag/Noncopyable.hpp>

#include <vector>
#include <string>

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

constexpr char const GetPathSeparator() noexcept
{
#if defined(__OS_WINDOWS__)
    return PATH_SEPARATOR_OF_WINDOWS;
#else
    return PATH_SEPARATOR_OF_POSIX;
#endif
}

constexpr char const GetGenericPathSeparator() noexcept
{
    return PATH_SEPARATOR_OF_POSIX;
}

char const PATH_SPLITTER_OF_WINDOWS = ';';
char const PATH_SPLITTER_OF_POSIX   = ':';

constexpr char const GetPathSplitter() noexcept
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
    Common() = delete;
    ~Common() = delete;

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
            return "";
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
        if (path.size() == 0U) {
            return "";
        }

        // Separate directory & filename.
        std::string separator = std::string() + filesystem::GetPathSeparator();
        std::size_t last_separator_index = path.rfind(separator);
        return path.substr(0, last_separator_index);
    }

public:
    static bool isAccessFile(std::string const & path) {
        uv_fs_t request;
        int error_code = uv_fs_access(nullptr, &request, path.c_str(), F_OK, nullptr);
        uv_fs_req_cleanup(&request);

        if (error_code == 0 && request.result == 0) {
            return true;
        }
        return false;
    }
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

