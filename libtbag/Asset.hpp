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
#include <libtbag/util/Noncopyable.hpp>

#include <set>
#include <string>

#include <uv.h>

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

constexpr char const * const GetHomeEnvName() noexcept(true)
{
#if defined(__OS_WINDOWS__)
    return "USERPROFILE";
#else
    return "HOME";
#endif
}

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 */
class Asset : public Noncopyable
{
public:
    using PathSet = std::set<std::string>;

private:
    PathSet dirs;

public:
    Asset() {
    }

    Asset(PathSet const & dirs) {
        this->dirs = dirs;
    }

    Asset(std::string const & path_env) {
        this->dirs = parsePathVariable(path_env);
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
    static PathSet parsePathVariable(std::string const & path_env) {
        PathSet result;
        std::string const DELIMITER = std::string() + GetPathSplitter();

        std::size_t start = 0;
        std::size_t end = path_env.find(DELIMITER);

        std::string current;

        while (end != std::string::npos) {
            current = path_env.substr(start, end - start);
            if (current.size() > 0) {
                result.insert(current);
            }
            start = end + DELIMITER.length();
            end = path_env.find(DELIMITER, start);
        }

        // Last token.
        current = path_env.substr(start, end);
        if (current.size() > 0) {
            result.insert(current);
        }

        return result;
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

