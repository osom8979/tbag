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
 * home directory environment variable name.
 *
 * @warning
 *  Don't use for the purpose of obtaining a Home directory.
 *
 * @translate{ko, 홈 디렉토리를 획득하기 위한 용도로 사용해선 안된다.}
 */
constexpr char const * const GetHomeEnvName() noexcept(true)
{
#if defined(__OS_WINDOWS__)
    return "USERPROFILE";
#else
    return "HOME";
#endif
}

/**
 * Common filesystem class prototype.
 *
 * @author zer0
 * @date   2016-04-08
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
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_COMMON_HPP__

