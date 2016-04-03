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

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 */
class Asset : public Noncopyable
{
private:
    std::set<std::string> dirs;

public:
    Asset() {
    }
    Asset(Asset const & obj) {
    }
    Asset(Asset && obj) {
    }
    virtual ~Asset() {
    }

public:
    Asset & operator = (Asset const & obj) {
        return *this;
    }
    Asset & operator = (Asset && obj) {
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

