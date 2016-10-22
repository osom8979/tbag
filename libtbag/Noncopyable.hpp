/**
 * @file   Noncopyable.hpp
 * @brief  Noncopyable class prototype.
 * @author zer0
 * @date   2016-04-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_NONCOPYABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_NONCOPYABLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Noncopyable class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 */
struct Noncopyable
{
protected:
    constexpr Noncopyable() = default;
    ~Noncopyable() = default;

protected:
    Noncopyable(Noncopyable const &) = delete;
    void operator=(Noncopyable const &) = delete;
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_NONCOPYABLE_HPP__

