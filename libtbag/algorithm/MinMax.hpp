/**
 * @file   MinMax.hpp
 * @brief  MinMax class prototype.
 * @author zer0
 * @date   2018-05-12
 *
 * @remarks
 *  Don't use min/max symbol name.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_MINMAX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_MINMAX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <functional>
#include <iterator>
#include <initializer_list>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

/**
 * @defgroup __DOXYGEN_GROUP__MIN_ALGORITHM__ List of min algorithm.
 * @{
 */

template <typename ForwardIterator, typename Compare>
inline ForwardIterator getMinElement(ForwardIterator first, ForwardIterator last, Compare comp)
{
    if (first != last) {
        ForwardIterator i = first;
        while (++i != last) {
            if (comp(*i, *first)) {
                first = i;
            }
        }
    }
    return first;
}

template <typename ForwardIterator>
inline ForwardIterator getMinElement(ForwardIterator first, ForwardIterator last)
{
    return getMinElement(first, last, std::less<typename std::iterator_traits<ForwardIterator>::value_type>());
}

template <typename T, typename Compare>
inline T const & getMin(T const & a, T const & b, Compare comp)
{
    return comp(b, a) ? b : a;
}

template <typename T>
inline T const & getMin(T const & a, T const & b)
{
    return getMin(a, b, std::less<T>());
}

template <typename T, typename Compare>
inline T getMin(std::initializer_list<T> t, Compare comp)
{
    return *getMinElement(t.begin(), t.end(), comp);
}

template <typename T>
inline T getMin(std::initializer_list<T> t)
{
    return *getMinElement(t.begin(), t.end(), std::less<T>());
}

/**
 * @}
 * @defgroup __DOXYGEN_GROUP__MAX_ALGORITHM__ List of max algorithm.
 * @{
 */

template <typename ForwardIterator, typename Compare>
inline ForwardIterator getMaxElement(ForwardIterator first, ForwardIterator last, Compare comp)
{
    if (first != last) {
        ForwardIterator i = first;
        while (++i != last) {
            if (comp(*first, *i)) {
                first = i;
            }
        }
    }
    return first;
}

template <typename ForwardIterator>
inline ForwardIterator getMaxElement(ForwardIterator first, ForwardIterator last)
{
    return getMaxElement(first, last, std::less<typename std::iterator_traits<ForwardIterator>::value_type>());
}

template <typename T, typename Compare>
inline T const & getMax(T const & a, T const & b, Compare comp)
{
    return comp(a, b) ? b : a;
}

template <typename T>
inline T const & getMax(T const & a, T const & b)
{
    return getMax(a, b, std::less<T>());
}

template <typename T, typename Compare>
inline T getMax(std::initializer_list<T> t, Compare comp)
{
    return *getMaxElement(t.begin(), t.end(), comp);
}

template <typename T>
inline T getMax(std::initializer_list<T> t)
{
    return *getMaxElement(t.begin(), t.end(), std::less<T>());
}

/**
 * @}
 */

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_MINMAX_HPP__

