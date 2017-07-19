/**
 * @file   Find.hpp
 * @brief  Find class prototype.
 * @author zer0
 * @date   2017-07-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_FIND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_FIND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

template <typename Iterator, typename T>
void findMinMax(Iterator itr, Iterator end, T * min, T * max)
{
    T min_cursor = *itr;
    T max_cursor = *itr;

    for (; itr != end; ++itr) {
        if (*itr < min_cursor) {
            min_cursor = *itr;
        }
        if (*itr > max_cursor) {
            max_cursor = *itr;
        }
    }

    // @formatter:off
    if (min != nullptr) { *min = min_cursor; }
    if (max != nullptr) { *max = max_cursor; }
    // @formatter:on
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_FIND_HPP__

