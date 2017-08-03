/**
 * @file   Buffer.hpp
 * @brief  Buffer class prototype.
 * @author zer0
 * @date   2017-08-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BUFFER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BUFFER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <exception>
#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <memory>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Buffer class prototype.
 *
 * @author zer0
 * @date   2017-08-03
 */
template <typename Type, typename AllocatorType = std::allocator<Type> >
class Buffer
{
public:
    using value_type             = Type;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using allocator_type         = AllocatorType;

private:
    allocator_type _allocator;

private:
    value_type * _data;
    size_type _size;

public:
    Buffer() : _data(nullptr), _size(0)
    {
        // EMPTY.
    }

    explicit Buffer(size_type count)
    {
        // EMPTY.
    }

    Buffer(std::initializer_list<value_type> list) : Buffer()
    {
        for (auto & cursor : list) {
        }
    }

    Buffer(Buffer const & obj) : Buffer()
    {
        (*this) = obj;
    }

    Buffer(Buffer && obj) : Buffer()
    {
        (*this) = std::move(obj);
    }

    virtual ~Buffer()
    {
        // EMPTY.
    }

public:
    Buffer & operator =(Buffer const & obj)
    {
        return *this;
    }

    Buffer & operator =(Buffer && obj)
    {
        return *this;
    }

public:
    void swap(Buffer & obj)
    {
        if (this != &obj) {
            std::swap(_data, obj._data);
            std::swap(_size, obj._size);
        }
    }

// Iterators methods.
public:
    inline       iterator begin()       TBAG_NOEXCEPT { return       iterator(_data); }
    inline const_iterator begin() const TBAG_NOEXCEPT { return const_iterator(_data); }
    inline       iterator   end()       TBAG_NOEXCEPT { return       iterator(_data + _size); }
    inline const_iterator   end() const TBAG_NOEXCEPT { return const_iterator(_data + _size); }

    inline       reverse_iterator rbegin()       TBAG_NOEXCEPT { return       reverse_iterator(end()); }
    inline const_reverse_iterator rbegin() const TBAG_NOEXCEPT { return const_reverse_iterator(end()); }
    inline       reverse_iterator   rend()       TBAG_NOEXCEPT { return       reverse_iterator(begin()); }
    inline const_reverse_iterator   rend() const TBAG_NOEXCEPT { return const_reverse_iterator(begin()); }

    inline         const_iterator  cbegin() const TBAG_NOEXCEPT { return  begin(); }
    inline         const_iterator    cend() const TBAG_NOEXCEPT { return    end(); }
    inline const_reverse_iterator crbegin() const TBAG_NOEXCEPT { return rbegin(); }
    inline const_reverse_iterator   crend() const TBAG_NOEXCEPT { return   rend(); }

// Capacity methods.
public:
         bool empty() const TBAG_NOEXCEPT { return _size == 0; }
    size_type  size() const TBAG_NOEXCEPT { return _size; }

// Element access methods.
public:
    inline       reference operator[](size_type i)       { return *(_data + i); }
    inline const_reference operator[](size_type i) const { return *(_data + i); }

    reference at(size_type i)
    {
        if (i >= _size) {
            throw std::out_of_range("Buffer::at() Out of range exception.");
        }
        return *(_data + i);
    }

    const_reference at(size_type i) const
    {
        if (i >= _size) {
            throw std::out_of_range("Buffer::at() Out of range exception.");
        }
        return *(_data + i);
    }

    inline       reference front()       { return _data[0]; }
    inline const_reference front() const { return _data[0]; }
    inline       reference  back()       { return _data[_size-1]; }
    inline const_reference  back() const { return _data[_size-1]; }

    inline       pointer data()       TBAG_NOEXCEPT { return _data; }
    inline const_pointer data() const TBAG_NOEXCEPT { return _data; }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BUFFER_HPP__

