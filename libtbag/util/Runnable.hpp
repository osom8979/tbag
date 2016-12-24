/**
 * @file   Runnable.hpp
 * @brief  Runnable class prototype.
 * @author zer0
 * @date   2016-12-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_RUNNABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_RUNNABLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Runnable class prototype.
 *
 * @author zer0
 * @date   2016-12-24
 */
struct Runnable
{
    virtual void run() = 0;
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_RUNNABLE_HPP__

