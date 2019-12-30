/**
 * @file   BoxIteratorGenerator.hpp
 * @brief  BoxIteratorGenerator class prototype.
 * @author zer0
 * @date   2019-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/box/BoxIterator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

/**
 * BoxIteratorGenerator class prototype.
 *
 * @author zer0
 * @date   2019-12-29
 * @date   2019-12-30 (Rename: IteratorGenerator -> BoxIteratorGenerator)
 */
template <typename T>
struct BoxIteratorGenerator
{
    using iterator = BoxIterator<T>;
    using const_iterator = BoxIterator<typename std::add_const<T>::type>;

    T * data_begin;
    T * data_end;
    int data_step;

    BoxIteratorGenerator(T * b, T * e, int s)
            : data_begin(b), data_end(e), data_step(s)
    {
        // EMPTY.
    }

    ~BoxIteratorGenerator()
    {
        // EMPTY.
    }

    iterator begin()
    {
        return iterator(data_begin, data_step);
    }

    iterator end()
    {
        return iterator(data_end, data_step);
    }

    const_iterator begin() const
    {
        return const_iterator(data_begin, data_step);
    }

    const_iterator end() const
    {
        return const_iterator(data_end, data_step);
    }

    const_iterator cbegin() const
    {
        return const_iterator(data_begin, data_step);
    }

    const_iterator cend() const
    {
        return const_iterator(data_end, data_step);
    }

    template <typename IteratorT, typename Predicated>
    Err forEach(IteratorT begin, IteratorT end, Predicated predicated)
    {
        for (; begin != end; ++begin) {
            predicated(*begin);
        }
        return libtbag::E_SUCCESS;
    }

    template <typename Predicated>
    Err forEach(Predicated predicated)
    {
        return forEach(begin(), end(), predicated);
    }
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__

