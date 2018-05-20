/**
 * @file   Array2d.hpp
 * @brief  Array2d class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/container/Bag.hpp>

#include <exception>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * 2-Rank Array class.
 *
 * @author zer0
 * @date   2015-09-11
 * @date   2015-11-19 (Rename: Matrix -> Array2d)
 * @date   2017-06-10 (Restore this class)
 * @date   2018-05-19 (Inherits the Bag class template)
 */
template <typename T, template <typename Tp> class A = std::allocator>
class Array2d : public Bag<T, A, std::vector<T, A<T> >, 2>
{
public:
    using Self = Array2d<T, A>;
    using Base = Bag<T, A, std::vector<T, A<T> >, 2>;

public:
    using value_type      = typename Base::value_type;
    using allocator_type  = typename Base::allocator_type;
    using reference       = typename Base::reference;
    using const_reference = typename Base::const_reference;
    using size_type       = typename Base::size_type;
    using difference_type = typename Base::difference_type;
    using pointer         = typename Base::pointer;
    using const_pointer   = typename Base::const_pointer;

    using       iterator = typename Base::iterator;
    using const_iterator = typename Base::const_iterator;

    using       reverse_iterator = typename Base::reverse_iterator;
    using const_reverse_iterator = typename Base::const_reverse_iterator;

public:
    Array2d()
    { /* EMPTY. */ }

    Array2d(size_type w, size_type h) : Array2d()
    { Base::resize(w, h); }

    Array2d(Array2d const & obj) : Array2d()
    { (*this) = obj; }

    Array2d(Array2d && obj) : Array2d()
    { (*this) = std::move(obj); }

    virtual ~Array2d()
    { /* EMPTY. */ }

public:
    Array2d & operator =(Array2d const & obj)
    {
        Self::copy(obj);
        return copy(obj);
    }

    Array2d & operator =(Array2d && obj)
    {
        Self::swap(obj);
        return *this;
    }

public:
    Array2d & copy(Array2d const & obj)
    {
        if (this != &obj) {
            Base::copy(obj);
        }
        return *this;
    }

    void swap(Array2d & obj)
    {
        Base::swap(obj);
    }

    friend void swap(Array2d & lh, Array2d & rh)
    {
        lh.swap(rh);
    }

public:
    inline size_type  width() const TBAG_NOEXCEPT { return Base::size(0); }
    inline size_type height() const TBAG_NOEXCEPT { return Base::size(1); }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

