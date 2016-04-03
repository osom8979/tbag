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

#if defined(__OS_WINDOWS__)
char const PATH_SEPARATOR = '\\';
char const PATH_SPLITTER  = ';';
char const * const HOME_ENV_NAME = "USERPROFILE";
#else
char const PATH_SEPARATOR = '/';
char const PATH_SPLITTER  = ':';
char const * const HOME_ENV_NAME = "HOME";
#endif

constexpr char const GetPathSeparator() noexcept(true)
{
    return PATH_SEPARATOR;
}

constexpr char const GetPathSplitter() noexcept(true)
{
    return PATH_SPLITTER;
}

constexpr char const * const GetHomeEnvName() noexcept(true)
{
    return HOME_ENV_NAME;
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
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

