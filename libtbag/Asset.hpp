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

#ifndef PATH_SEPARATOR
# if defined(WIN32) || defined(_WIN32)
#  define PATH_SEPARATOR '\\'
# else
#  define PATH_SEPARATOR '/'
# endif
#endif // PATH_SEPARATOR

#ifndef PATH_SPLITTER
# if defined(WIN32) || defined(_WIN32)
#  define PATH_SPLITTER ';'
# else
#  define PATH_SPLITTER ':'
# endif
#endif // PATH_SPLITTER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

constexpr char GetPathSeparator() noexcept(true)
{
    return PATH_SEPARATOR;
}

constexpr char GetPathSplitter() noexcept(true)
{
    return PATH_SPLITTER;
}

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 */
class Asset : public Noncopyable
{
private:
    using PathSet = std::set<std::string>;
    PathSet dirs;

public:
    Asset() {
    }
    Asset(PathSet & dirs) {
    }
    Asset(Asset const & obj) {
    }
    Asset(Asset && obj) {
    }
    virtual ~Asset() {
    }

public:
    Asset & operator = (Asset const & obj) {
        return copy(obj);
    }
    Asset & operator = (Asset && obj) {
        swap(obj);
        return *this;
    }

public:
    Asset & copy(Asset const & obj) {
        return *this;
    }
    void swap(Asset & obj) {
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ASSET_HPP__

