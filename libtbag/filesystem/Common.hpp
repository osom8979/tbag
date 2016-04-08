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

#ifndef __MAX_PATH_BUFFER_SIZE__
#define __MAX_PATH_BUFFER_SIZE__ 256
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

constexpr char const GetPathSeparator() noexcept
{
#if defined(__OS_WINDOWS__)
    return '\\';
#else
    return '/';
#endif
}

constexpr char const GetGenericPathSeparator() noexcept
{
    return '/';
}

constexpr char const GetPathSplitter() noexcept
{
#if defined(__OS_WINDOWS__)
    return ';';
#else
    return ':';
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

public:
    /** Obtain HOME directory. */
    static std::string getHomeDir() {
        std::size_t path_length = __MAX_PATH_BUFFER_SIZE__;
        std::vector<char> buffer;
        buffer.resize(path_length);

        if (uv_os_homedir(&buffer[0], &path_length) != 0) {
            return "";
        }
        return std::string(buffer.begin(), buffer.begin() + path_length);
    }

    /** Obtain executable file directory. */
    static std::string getExeDir() {
        std::size_t path_length = __MAX_PATH_BUFFER_SIZE__;
        std::vector<char> buffer;
        buffer.resize(path_length);

        if (uv_exepath(&buffer[0], &path_length) != 0) {
            return "";
        }

        // Separate directory & filename.
        std::string path(buffer.begin(), buffer.begin() + path_length);
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

