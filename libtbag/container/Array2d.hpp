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
#include <libtbag/iterator/IteratorBypass.hpp>

#include <exception>
#include <memory>

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
template <typename T, typename A = std::allocator<T> >
class Array2d
{
public:
    using self_type = Array2d;
    using bag_type = Bag<T, A, 2>;

private:
    bag_type _bag;

public:
    TBAG_ITERATOR_TYPES_BYPASS(bag_type);
    TBAG_ITERATOR_SIZER_BYPASS(_bag);
    TBAG_ITERATOR_CLEAR_BYPASS(_bag);
    TBAG_ITERATOR_FORWARD_BYPASS(bag_type, _bag);
    TBAG_ITERATOR_REVERSE_BYPASS(bag_type, _bag);

public:
    Array2d() : _bag()
    {
        // EMPTY.
    }

    Array2d(size_type w, size_type h) : Array2d()
    {
        resize(w, h);
    }

    Array2d(Array2d const & obj) : Array2d()
    {
        (*this) = obj;
    }

    Array2d(Array2d && obj) : Array2d()
    {
        (*this) = std::move(obj);
    }

    ~Array2d()
    {
        // EMPTY.
    }

public:
    Array2d & operator =(Array2d const & obj)
    {
        copy(obj);
        return *this;
    }

    Array2d & operator =(Array2d && obj)
    {
        swap(obj);
        return *this;
    }

public:
    Array2d & copy(Array2d const & obj)
    {
        if (this != &obj) {
            _bag = obj._bag;
        }
        return *this;
    }

    void swap(Array2d & obj)
    {
        if (this != &obj) {
            _bag.swap(obj._bag);
        }
    }

    friend void swap(Array2d & lh, Array2d & rh) { lh.swap(rh); }

public:
    inline       pointer data()       TBAG_NOEXCEPT_SP_OP(_bag.data()) { return _bag.data(); }
    inline const_pointer data() const TBAG_NOEXCEPT_SP_OP(_bag.data()) { return _bag.data(); }

public:
    size_type  width() const TBAG_NOEXCEPT_SP_OP(_bag.size(1)) { return _bag.size(1); }
    size_type height() const TBAG_NOEXCEPT_SP_OP(_bag.size(0)) { return _bag.size(0); }

public:
    reference at(size_type width, size_type height) TBAG_NOEXCEPT_SP_OP(_bag.index(height, width))
    {
        return _bag.index(height, width);
    }

    const_reference at(size_type width, size_type height) const TBAG_NOEXCEPT_SP_OP(_bag.index(height, width))
    {
        return _bag.index(height, width);
    }

    reference at(size_type index) TBAG_NOEXCEPT_SP_OP(_bag.at(index))
    {
        return _bag.at(index);
    }

    const_reference at(size_type index) const TBAG_NOEXCEPT_SP_OP(_bag.at(index))
    {
        return _bag.at(index);
    }

    reference operator [](size_type index) TBAG_NOEXCEPT_SP_OP(_bag.operator[](index))
    {
        return _bag[index];
    }

    const_reference operator [](size_type index) const TBAG_NOEXCEPT_SP_OP(_bag.operator[](index))
    {
        return _bag[index];
    }

public:
    void resize(size_type width, size_type height)
    {
        _bag.resize(height, width);
    }

public:
    iterator height_begin(size_type x, size_type y = 0)
    {
        return begin_step(y * width() + x, width());
    }

    iterator height_end(size_type x, size_type y = 0)
    {
        return begin_step(y * width() + x, width());
    }

public:
    self_type sub(size_type x, size_type y, size_type w, size_type h) const
    {
        self_type result(w, h);
        auto result_itr = result.begin();
        auto y_itr = begin_step(y * width() + x, width());
        auto y_end = begin_step(y * width() * h + x, height());
        for (; y_itr != y_end; ++y_itr) {
            auto x_itr = y_itr.sub(1);
            auto x_end = y_itr + w;
            for (; x_itr != x_end; ++x_itr) {
                *result_itr = *x_itr;
            }
        }
        return result;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

