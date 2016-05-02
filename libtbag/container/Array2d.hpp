/**
 * @file   Array2d.hpp
 * @brief  Array2d class prototype.
 * @author zer0
 * @date   2016-04-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/exception/Exception.hpp>

#include <cassert>

#include <memory>
#include <algorithm>
#include <iterator>


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
 */
template <typename Value
        , typename Reference = Value&
        , typename Pointer   = Value*>
class Array2dIterator
{
public:
    using Self = Array2dIterator<Value, Reference, Pointer>;
    using Tag  = std::bidirectional_iterator_tag;
    using Base = std::iterator<Tag, Value, ptrdiff_t, Pointer, Reference>;

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
    static constexpr std::size_t getDataSize() noexcept {
        return sizeof(Value);
    }

public:
    Array2dIterator() noexcept : cursor(nullptr), step(1) {
        __EMPTY_BLOCK__
    }

    Array2dIterator(pointer cursor, difference_type step = 1) noexcept : cursor(cursor), step(step) {
        __EMPTY_BLOCK__
    }

    Array2dIterator(Self const & obj) noexcept : cursor(obj.cursor), step(obj.step) {
        __EMPTY_BLOCK__
    }

    ~Array2dIterator() noexcept {
        __EMPTY_BLOCK__
    }

public:
    reference operator *() const noexcept {
        return *cursor;
    }

    pointer operator ->() const noexcept {
        return cursor;
    }

public:
    Self & operator =(Self const & obj) const noexcept {
        if (this != &obj) {
            this->cursor = obj.cursor;
            this->step   = obj.step;
        }
        return *this;
    }

public:
    bool operator ==(Self const & obj) const noexcept {
        return (this->cursor == obj.cursor ? true : false);
    }

    bool operator !=(Self const & obj) const noexcept {
        return (this->cursor != obj.cursor ? true : false);
    }

public:
    Self & operator ++() noexcept {
        cursor += step;
        return *this;
    }

    Self & operator --() noexcept {
        cursor -= step;
        return *this;
    }

public:
    Self operator ++(int) noexcept {
        Self temp(*this);
        cursor += step;
        return temp;
    }

    Self operator --(int) noexcept {
        Self temp(*this);
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
 */
template <typename DataType, typename Allocator = std::allocator<DataType> >
class Array2d : public Allocator
{
public:
    using Self = Array2d<DataType, Allocator>;

    using      Iterator = Array2dIterator<DataType, DataType&, DataType*>;
    using ConstIterator = Array2dIterator<DataType, DataType const &, DataType const *>;

    using  SizeType = int;
    using IndexType = int;

private:
    SizeType _width;
    SizeType _height;
    DataType * _data;

public:
    Array2d() : _width(0), _height(0), _data(nullptr) {
        __EMPTY_BLOCK__
    }

    Array2d(SizeType w, SizeType h) : Array2d() {
        this->resize(w, h);
    }

    Array2d(Array2d const & obj) : Array2d() {
        this->copy(obj);
    }

    Array2d(Array2d && obj) : Array2d() {
        this->clear();
        this->swap(obj);
    }

    ~Array2d() {
        this->clear();
    }

public:
    Array2d & operator =(Array2d const & obj) noexcept {
        return this->copy(obj);
    }

    Array2d & operator =(Array2d && obj) noexcept {
        this->clear();
        this->swap(obj);
        return *this;
    }

public:
    Array2d & copy(Array2d const & obj) noexcept {
        if (this != &obj) {
            this->resize(obj._width, obj._height);
            std::copy(obj._data, obj._data + obj.size(), this->_data);
        }
        return *this;
    }

    void swap(Array2d & obj) noexcept {
        if (this != &obj) {
            std::swap(this->_width,  obj._width);
            std::swap(this->_height, obj._height);
            std::swap(this->_data,   obj._data);
        }
    }

public:
    inline SizeType width() const noexcept {
        return _width;
    }

    inline SizeType height() const noexcept {
        return _height;
    }

    inline DataType * data() noexcept {
        return _data;
    }

    inline DataType const * data() const noexcept {
        return _data;
    }

    inline std::size_t size() const noexcept {
        return _width * _height;
    }

public:
    void clear() {
        if (_data == nullptr) {
            assert(_width  == 0);
            assert(_height == 0);
            return;
        }

        Allocator::deallocate(_data, size());
        this->_data   = nullptr;
        this->_width  = 0;
        this->_height = 0;
    }

    void resize(SizeType w, SizeType h) throw (exception::IllegalArgumentException) {
        if (w <= 0 || h <= 0) {
            throw exception::IllegalArgumentException();
        }

        if (static_cast<std::size_t>(w * h) == size()) {
            this->_width  = w;
            this->_height = h;
        } else {
            this->clear(); // IMPORTANT!
            this->_width  = w;
            this->_height = h;
            this->_data   = Allocator::allocate(size());
        }

        std::fill(_data, _data + size(), DataType());
    }

public:
    DataType & at(IndexType x, IndexType y) throw (exception::OutOfRangeException) {
        if (x < 0 || _width <= x || y < 0 || _height <= y) {
            throw exception::OutOfRangeException();
        }
        return *(_data + (_width * y + x));
    }

    DataType & at(std::size_t index) throw (exception::OutOfRangeException) {
        if (size() <= index) {
            throw exception::OutOfRangeException();
        }
        return *(_data + index);
    }

public:
    DataType get(IndexType x, IndexType y) const throw (exception::OutOfRangeException) {
        if (x < 0 || _width <= x || y < 0 || _height <= y) {
            throw exception::OutOfRangeException();
        }
        return *(_data + (_width * y + x));
    }

    DataType get(std::size_t index) const throw (exception::OutOfRangeException) {
        if (size() <= index) {
            throw exception::OutOfRangeException();
        }
        return *(_data + index);
    }

public:
    inline DataType & operator [](std::size_t index) {
        return *(_data + index);
    }

public:
    Iterator begin() noexcept {
        return  Iterator(_data);
    }

    ConstIterator cbegin() const noexcept {
        return ConstIterator(_data);
    }

public:
    Iterator end() noexcept {
        return  Iterator(_data + size());
    }

    ConstIterator cend() const noexcept {
        return ConstIterator(_data + size());
    }

public:
    Iterator beginWithHeight() noexcept {
        return  Iterator(_data, _width);
    }

    ConstIterator cbeginWithHeight() const noexcept {
        return  ConstIterator(_data, _width);
    }

public:
    Iterator endWithHeight() noexcept {
        return  Iterator(_data + size(), _width);
    }

    ConstIterator cendWithHeight() const noexcept {
        return  ConstIterator(_data + size(), _width);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ARRAY2D_HPP__

