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

#include <cassert>
#include <cstring>

#include <memory>
#include <algorithm>
#include <iterator>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * A Array2d::Iterator.
 *
 * @author zer0
 * @date   2015-11-18
 * @date   2015-11-19 (Rename: MatrixIterator -> Array2dIterator)
 * @date   2017-06-10 (Restore this class)
 */
template <typename ValueType
        , typename ReferenceType = ValueType&
        , typename PointerType = ValueType*>
class Array2dIterator
{
public:
    using Value     = ValueType;
    using Reference = ReferenceType;
    using Pointer   = PointerType;

public:
    using Self = Array2dIterator<Value, Reference, Pointer>;
    using Tag  = std::bidirectional_iterator_tag;
    using Base = std::iterator<Tag, Value, std::ptrdiff_t, Pointer, Reference>;

// iterator_traits:
public:
    using iterator_category = typename Base::iterator_category;
    using value_type        = typename Base::value_type;
    using difference_type   = typename Base::difference_type;
    using pointer           = typename Base::pointer;
    using reference         = typename Base::reference;

public:
    pointer cursor;
    difference_type step;

public:
    Array2dIterator() TBAG_NOEXCEPT : cursor(nullptr), step(1)
    { /* EMPTY. */ }

    Array2dIterator(pointer cursor, difference_type step = 1) TBAG_NOEXCEPT : cursor(cursor), step(step)
    { /* EMPTY. */ }

    Array2dIterator(Array2dIterator const & obj) TBAG_NOEXCEPT : cursor(obj.cursor), step(obj.step)
    { /* EMPTY. */ }

    virtual ~Array2dIterator() TBAG_NOEXCEPT
    { /* EMPTY. */ }

public:
    inline reference operator  *() const TBAG_NOEXCEPT { return *cursor; }
    inline pointer   operator ->() const TBAG_NOEXCEPT { return cursor;  }

public:
    Array2dIterator & operator =(Self const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            cursor = obj.cursor;
            step   = obj.step;
        }
        return *this;
    }

public:
    inline bool operator ==(Array2dIterator const & obj) const TBAG_NOEXCEPT
    { return this->cursor == obj.cursor; }
    inline bool operator !=(Array2dIterator const & obj) const TBAG_NOEXCEPT
    { return this->cursor != obj.cursor; }

public:
    inline Array2dIterator & operator ++() TBAG_NOEXCEPT
    {
        cursor += step;
        return *this;
    }

    inline Array2dIterator & operator --() TBAG_NOEXCEPT
    {
        cursor -= step;
        return *this;
    }

public:
    inline Array2dIterator operator ++(int) TBAG_NOEXCEPT
    {
        Array2dIterator temp(*this);
        cursor += step;
        return temp;
    }

    inline Array2dIterator operator --(int) TBAG_NOEXCEPT
    {
        Array2dIterator temp(*this);
        cursor -= step;
        return temp;
    }
};

/**
 * 2-Rank Array class.
 *
 * @author zer0
 * @date   2015-09-11
 * @date   2015-11-19 (Rename: Matrix -> Array2d)
 * @date   2017-06-10 (Restore this class)
 */
template <typename ValueType, typename AllocatorType = std::allocator<ValueType> >
class Array2d
{
public:
    using Value = ValueType;
    using Allocator = AllocatorType;

    using Self = Array2d<Value, Allocator>;

public:
    using value_type        = Value;
    using allocator_type    = Allocator;
    using reference         = typename std::add_lvalue_reference<value_type>::type;
    using const_reference   = typename std::add_const<reference>::type;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using pointer           = typename std::add_pointer<value_type>::type;
    using const_pointer     = typename std::add_const<pointer>::type;

    using       iterator = Array2dIterator<Value, Value&, Value*>;
    using const_iterator = Array2dIterator<Value, Value const &, Value const *>;

    TBAG_CONSTEXPR static char const * const OUT_OF_RANGE_EXCEPTION_MESSAGE = "Out of range.";

    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                  "Allocator::value_type must be same type as value_type");

private:
    Allocator _alloc;
    Value * _data;
    unsigned _width;
    unsigned _height;

public:
    Array2d() TBAG_NOEXCEPT_EXPR(std::is_nothrow_default_constructible<allocator_type>::value)
            : _data(nullptr), _width(0), _height(0)
    { /* EMPTY. */ }

    Array2d(unsigned w, unsigned h) : Array2d()
    { resize(w, h); }

    Array2d(Array2d const & obj) : Array2d()
    { (*this) = obj; }

    Array2d(Array2d && obj) : Array2d()
    { (*this) = std::move(obj); }

    virtual ~Array2d()
    { clear(); }

public:
    Array2d & operator =(Array2d const & obj)
    {
        return copy(obj);
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
            clear();
            _alloc = obj._alloc;

            resize(obj._width, obj._height);
            std::copy(obj._data, obj._data + obj.size(), _data);
        }
        return *this;
    }

    void swap(Array2d & obj)
    {
        if (this != &obj) {
            std::swap( _alloc, obj._alloc );
            std::swap( _width, obj._width );
            std::swap(_height, obj._height);
            std::swap(  _data, obj._data  );
        }
    }

public:
    inline unsigned  width() const TBAG_NOEXCEPT { return  _width; }
    inline unsigned height() const TBAG_NOEXCEPT { return _height; }

    inline Value       * data()       TBAG_NOEXCEPT { return _data; }
    inline Value const * data() const TBAG_NOEXCEPT { return _data; }

    inline std::size_t size() const TBAG_NOEXCEPT { return _width * _height; }
    inline std::size_t byte() const TBAG_NOEXCEPT { return size() * sizeof(Value); }

private:
    void create()
    {
        std::size_t const SIZE = size();
        if (SIZE > 0) {
            _data = _alloc.allocate(SIZE);
            for (std::size_t i = 0; i < SIZE; ++i) {
                _alloc.construct(_data + i);
            }
        }
    }

    void destory()
    {
        std::size_t const SIZE = size();
        if (SIZE > 0) {
            for (std::size_t i = 0; i < SIZE; ++i) {
                _alloc.destroy(_data + i);
            }
            _alloc.deallocate(_data, SIZE);
        }
    }

public:
    void clear()
    {
        if (_data == nullptr) {
            assert(_width  == 0);
            assert(_height == 0);
            return;
        }
        destory();
        _data   = nullptr;
        _width  = 0;
        _height = 0;
    }

    void resize(unsigned w, unsigned h)
    {
        if (static_cast<std::size_t>(w * h) == size()) {
            _width  = w;
            _height = h;
        } else {
            clear(); // IMPORTANT!
            _width  = w;
            _height = h;
            create();
        }
        std::fill(_data, _data + size(), Value());
    }

public:
    Value & at(unsigned x, unsigned y) throw (std::out_of_range)
    {
        if (x < 0 || _width <= x || y < 0 || _height <= y) {
            throw std::out_of_range(OUT_OF_RANGE_EXCEPTION_MESSAGE);
        }
        return *(_data + (_width * y + x));
    }

    Value & at(std::size_t index) throw (std::out_of_range)
    {
        if (size() <= index) {
            throw std::out_of_range(OUT_OF_RANGE_EXCEPTION_MESSAGE);
        }
        return *(_data + index);
    }

public:
    Value get(unsigned x, unsigned y) const throw (std::out_of_range)
    {
        if (x < 0 || _width <= x || y < 0 || _height <= y) {
            throw std::out_of_range(OUT_OF_RANGE_EXCEPTION_MESSAGE);
        }
        return *(_data + (_width * y + x));
    }

    Value get(std::size_t index) const throw (std::out_of_range)
    {
        if (size() <= index) {
            throw std::out_of_range(OUT_OF_RANGE_EXCEPTION_MESSAGE);
        }
        return *(_data + index);
    }

public:
    void assign(void const * data, std::size_t data_size)
    {
        ::memcpy(_data, data, (data_size < byte() ? data_size : byte()));
    }

public:
    inline Value & operator [](std::size_t index)
    { return *(_data + index); }

public:
    inline iterator begin() TBAG_NOEXCEPT { return iterator(_data);          }
    inline iterator   end() TBAG_NOEXCEPT { return iterator(_data + size()); }

    inline const_iterator cbegin() const TBAG_NOEXCEPT { return const_iterator(_data);          }
    inline const_iterator   cend() const TBAG_NOEXCEPT { return const_iterator(_data + size()); }

public:
    inline iterator beginWithHeight() TBAG_NOEXCEPT { return iterator(_data, _width);          }
    inline iterator   endWithHeight() TBAG_NOEXCEPT { return iterator(_data + size(), _width); }

    inline const_iterator cbeginWithHeight() const TBAG_NOEXCEPT { return const_iterator(_data, _width);          }
    inline const_iterator   cendWithHeight() const TBAG_NOEXCEPT { return const_iterator(_data + size(), _width); }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

