/**
 * @file   Optimization.hpp
 * @brief  Optimization class prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OPTIMIZATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OPTIMIZATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

inline void prevent_optimization_impl(void const * ptr)
{
    std::cout << ptr;
}

/**
 * @remarks
 *  Some compilers are really clever figuring out ways to access to data that @n
 *  would allow them to optimize things. Storing and reading a pointer from   @n
 *  volatile location seems to work around this.
 */
template <typename T>
T * prevent_optimization(T * ptr)
{
    volatile bool never = false;
    if (never) {
        prevent_optimization_impl(ptr);
    }
    T * volatile * volatile opaque;
    opaque = &ptr;
    return *opaque;
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_OPTIMIZATION_HPP__

