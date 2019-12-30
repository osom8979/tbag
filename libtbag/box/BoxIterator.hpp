/**
 * @file   BoxIterator.hpp
 * @brief  BoxIterator class prototype.
 * @author zer0
 * @date   2019-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATOR_HPP__

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

namespace box {

/**
 * BoxIterator class prototype.
 *
 * @author zer0
 * @date   2019-12-16
 * @date   2019-12-30 (Rename: Iterator -> BoxIterator)
 */
template <typename T>
struct BoxIterator : public std::iterator<std::forward_iterator_tag, T>
{
public:
    using base = std::iterator<std::forward_iterator_tag, T>;
    using value_type        = typename base::value_type;
    using difference_type   = typename base::difference_type;
    using pointer           = typename base::pointer;
    using reference         = typename base::reference;
    using iterator_category = typename base::iterator_category;

public:
    pointer data;
    int stride_byte;

public:
    BoxIterator(pointer data, int stride)
            : data(data), stride_byte(stride)
    { /* EMPTY. */ }

    BoxIterator(BoxIterator const & obj) TBAG_NOEXCEPT
            : data(obj.data), stride_byte(obj.stride_byte)
    { /* EMPTY. */ }

    BoxIterator(BoxIterator && obj) TBAG_NOEXCEPT
            : data(obj.data), stride_byte(obj.stride_byte)
    { /* EMPTY. */ }

    ~BoxIterator()
    { /* EMPTY. */ }

public:
    BoxIterator & operator =(BoxIterator const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            data = obj.data;
            stride_byte = obj.stride_byte;
        }
        return *this;
    }

    BoxIterator & operator =(BoxIterator && obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            data = obj.data;
            stride_byte = obj.stride_byte;
        }
        return *this;
    }

public:
    inline       pointer operator ->()       TBAG_NOEXCEPT { return data; }
    inline const pointer operator ->() const TBAG_NOEXCEPT { return data; }

    inline       reference operator *()       TBAG_NOEXCEPT { return *data; }
    inline const reference operator *() const TBAG_NOEXCEPT { return *data; }

public:
    inline bool operator ==(BoxIterator const & obj) const TBAG_NOEXCEPT
    {
        return data == obj.data;
    }

    inline bool operator !=(BoxIterator const & obj) const TBAG_NOEXCEPT
    {
        return data != obj.data;
    }

public:
    inline void step(int i) TBAG_NOEXCEPT
    {
        data = (pointer)(((std::intptr_t)data) + (stride_byte * i));
    }

    inline void next() TBAG_NOEXCEPT
    {
        step(1);
    }

    inline void prev() TBAG_NOEXCEPT
    {
        step(-1);
    }

public:
    BoxIterator & operator ++() TBAG_NOEXCEPT
    {
        next();
        return *this;
    }

    BoxIterator & operator --() TBAG_NOEXCEPT
    {
        prev();
        return *this;
    }

public:
    BoxIterator operator ++(int) TBAG_NOEXCEPT
    {
        BoxIterator temp = *this;
        next();
        return temp;
    }

    BoxIterator operator --(int) TBAG_NOEXCEPT
    {
        BoxIterator temp = *this;
        prev();
        return temp;
    }
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATOR_HPP__

