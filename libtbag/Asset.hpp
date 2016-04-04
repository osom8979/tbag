/**
 * @file   Asset.hpp
 * @brief  Asset class prototype.
 * @author zer0
 * @date   2016-04-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Strings.hpp>

#include <vector>
#include <map>
#include <string>

#include <uv.h>

#ifndef __ASSET_CONFIG__MAX_PATH_LENGTH__
#define __ASSET_CONFIG__MAX_PATH_LENGTH__ 256
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

constexpr char const GetPathSeparator() noexcept(true)
{
#if defined(__OS_WINDOWS__)
    return '\\';
#else
    return '/';
#endif
}

constexpr char const GetPathSplitter() noexcept(true)
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

char const * const HOME_DIRECTORY_NAME = "HOME";
char const * const ROOT_DIRECTORY_NAME = "ROOT";

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 */
class Asset : public Noncopyable
{
public:
    /** Default setting for the constructor. */
    class default_setting { };

public:
    using PathMap = std::map<std::string, std::string>;

private:
    PathMap dirs;

public:
    Asset() {
        // EMPTY.
    }

    explicit Asset(default_setting const & __unused__) {
    }

    Asset(PathMap const & dirs) {
        this->dirs = dirs;
    }

    Asset(Asset const & obj) {
        this->copy(obj);
    }

    Asset(Asset && obj) {
        this->swap(obj);
    }

    virtual ~Asset() {
        this->dirs.clear();
    }

public:
    Asset & operator = (Asset const & obj) {
        return this->copy(obj);
    }

    Asset & operator = (Asset && obj) {
        this->swap(obj);
        return *this;
    }

public:
    Asset & copy(Asset const & obj) {
        if (this != &obj) {
            this->dirs = obj.dirs;
        }
        return *this;
    }

    void swap(Asset & obj) {
        if (this != &obj) {
            this->dirs.swap(obj.dirs);
        }
    }

public:
    /** Obtain HOME directory. */
    static std::string getHomeDir() {
        std::size_t path_length = (__ASSET_CONFIG__MAX_PATH_LENGTH__);
        std::vector<char> buffer;
        buffer.resize(path_length);

        if (uv_os_homedir(&buffer[0], &path_length) != 0) {
            return "";
        }
        return std::string(buffer.begin(), buffer.begin() + path_length);
    }

    /** Obtain executable file directory. */
    static std::string getExeDir() {
        std::size_t path_length = (__ASSET_CONFIG__MAX_PATH_LENGTH__);
        std::vector<char> buffer;
        buffer.resize(path_length);

        if (uv_exepath(&buffer[0], &path_length) != 0) {
            return "";
        }

        // Separate directory & filename.
        std::string path(buffer.begin(), buffer.begin() + path_length);
        std::size_t last_separator_index = path.rfind(std::string() + GetPathSeparator());
        return path.substr(0, last_separator_index);
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

