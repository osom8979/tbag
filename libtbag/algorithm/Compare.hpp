/**
 * @file   Compare.hpp
 * @brief  Compare class prototype.
 * @author zer0
 * @date   2018-07-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_COMPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_COMPARE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <iterator>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

template <typename Iterator>
bool compareMap(Iterator first1, Iterator last1, Iterator first2, Iterator last2)
{
    if (std::distance(first1, last1) != std::distance(first2, last2)) {
        return false;
    }
    for (; first1 != last1; ++first1, (void) ++first2) {
        if (first1->first != first2->first || first1->second != first2->second) {
            return false;
        }
    }
    return true;
}

template <typename MapType>
bool compareMap(MapType & lh, MapType & rh)
{
    return compareMap(lh.begin(), lh.end(), rh.begin(), rh.end());
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_COMPARE_HPP__

