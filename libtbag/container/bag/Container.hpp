/**
 * @file   Container.hpp
 * @brief  Container class prototype.
 * @author zer0
 * @date   2018-07-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_CONTAINER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_CONTAINER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/memory/Allocator.hpp>

#include <cassert>
#include <iterator>
#include <algorithm>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {
namespace bag       {

/**
 * Container class prototype.
 *
 * @tparam T  Base type.
 * @tparam A  Base allocator.
 *
 * @author zer0
 * @date   2018-04-25
 * @date   2018-07-28 (Move: libtbag/container -> libtbag/container/bag)
 */
template <typename T, typename A = libtbag::memory::Allocator<T> >
class Container
{
public:
    using value_type      = T;
    using allocator_type  = A;
    using self_type       = Container<value_type, allocator_type>;
    using reference       = value_type &;
    using const_reference = value_type const &;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = value_type *;
    using const_pointer   = value_type const *;

public:
    using       iterator = value_type *;
    using const_iterator = value_type const *;

    using       reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                  "allocator_type::value_type must be same type as value_type");

private:
    allocator_type  _allocator;
    pointer         _data;
    size_type       _capacity;
    size_type       _size;

public:
    Container() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<allocator_type>::value)
            : _allocator(), _data(nullptr), _capacity(0), _size(0)
    {
        // EMPTY.
    }

    Container(size_type capacity) TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<allocator_type>::value)
            : _allocator(), _data(nullptr), _capacity(0), _size(0)
    {
        reserve(capacity);
    }

    Container(allocator_type const & allocator) TBAG_NOEXCEPT_SPECIFIER(
            (std::is_nothrow_constructible<allocator_type, allocator_type const &>::value))
            : _allocator(allocator), _data(nullptr), _capacity(0), _size(0)
    {
        // EMPTY.
    }

    Container(Container const & obj) TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<allocator_type>::value)
            : Container()
    {
        copy(obj);
    }

    Container(Container && obj) TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<allocator_type>::value)
            : Container()
    {
        swap(obj);
    }

    ~Container()
    {
        clear();

        if (_data != nullptr) {
            assert(_capacity >= 0);
            _allocator.deallocate(_data, _capacity);
        }

        _data     = nullptr;
        _capacity = 0;
        _size     = 0;
    }

public:
    template <typename Allocator>
    Container & operator =(Container<value_type, Allocator> const & obj)
    {
        copy<Allocator>(obj);
        return *this;
    }

    Container & operator =(Container && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

    template <typename Allocator>
    void copy(Container<value_type, Allocator> const & obj)
    {
        if (this == &obj) {
            return;
        }
        resize(obj._size);
        std::copy(obj.begin(), obj.end(), begin());
    }

    void swap(Container & obj) TBAG_NOEXCEPT
    {
        if (this == &obj) {
            return;
        }
        // @formatter:off
        std::swap(_allocator, obj._allocator);
        std::swap(_data     , obj._data);
        std::swap(_capacity , obj._capacity);
        std::swap(_size     , obj._size);
        // @formatter:on
    }

    inline friend void swap(Container & lh, Container & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

private:
    static void swapElements(Container & lt, Container & gt)
    {
        assert(lt._size < gt._size);
        auto const TEMP_SIZE = lt._size;
        lt.resize(gt._size);
        std::swap_ranges(lt._data, lt._data + lt._size, gt._data);
        gt.resize(TEMP_SIZE);
    }

public:
    void swapElements(Container & obj)
    {
        if (this == &obj) {
            return;
        }

        if (_size < obj._size) {
            swapElements(*this, obj);
        } else if (_size > obj._size) {
            swapElements(obj, *this);
        } else {
            assert(_size == obj._size);
            std::swap_ranges(_data, _data + _size, obj._data);
        }
    }

public:
    iterator begin() TBAG_NOEXCEPT { return iterator(_data); }
    iterator   end() TBAG_NOEXCEPT { return iterator(_data + _size); }

    const_iterator begin() const TBAG_NOEXCEPT { return const_iterator(_data); }
    const_iterator   end() const TBAG_NOEXCEPT { return const_iterator(_data + _size); }

    reverse_iterator rbegin() TBAG_NOEXCEPT { return reverse_iterator(  end()); }
    reverse_iterator   rend() TBAG_NOEXCEPT { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT { return const_reverse_iterator(  end()); }
    const_reverse_iterator   rend() const TBAG_NOEXCEPT { return const_reverse_iterator(begin()); }

    const_iterator cbegin() const TBAG_NOEXCEPT { return begin(); }
    const_iterator   cend() const TBAG_NOEXCEPT { return   end(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT { return rbegin(); }
    const_reverse_iterator   crend() const TBAG_NOEXCEPT { return   rend(); }

public:
    reference front() { return _data[0]; }
    reference  back() { return _data[_size > 0 ? _size-1 : 0]; }

    const_reference front() const { return _data[0]; }
    const_reference  back() const { return _data[_size > 0 ? _size-1 : 0]; }

public:
    reference operator [](size_type index)
    {
        return at(index);
    }

    const_reference operator [](size_type index) const
    {
        return at(index);
    }

    reference at(size_type index)
    {
        assert(0 <= COMPARE_AND(index) < _size);
        return _data[index];
    }

    const_reference at(size_type index) const
    {
        assert(0 <= COMPARE_AND(index) < _size);
        return _data[index];
    }

public:
    allocator_type & allocator() TBAG_NOEXCEPT
    {
        return _allocator;
    }

    allocator_type const & allocator() const TBAG_NOEXCEPT
    {
        return _allocator;
    }

    pointer data() TBAG_NOEXCEPT
    {
        return _data;
    }

    const_pointer data() const TBAG_NOEXCEPT
    {
        return _data;
    }

public:
    TBAG_CONSTEXPR static size_type max_size() TBAG_NOEXCEPT
    {
        return allocator_type::max_size();
    }

    size_type size() const TBAG_NOEXCEPT
    {
        return _size;
    }

    bool empty() const TBAG_NOEXCEPT
    {
        return _size == 0;
    }

    size_type capacity() const TBAG_NOEXCEPT
    {
        return _capacity;
    }

public:
    void reserve(size_type size)
    {
        if (size <= _capacity) {
            return;
        }

        pointer data = _allocator.allocate(size);

        if (_data != nullptr) {
            assert(_capacity > 0);
            std::swap_ranges(_data, _data + _size, data);
            _allocator.deallocate(_data, _capacity);
        } else {
            assert(_capacity == 0);
        }

        _data = data;
        _capacity = size;
    }

    void resize(size_type size)
    {
        if (size > _capacity) {
            reserve(size);
        }

        if (size > _size) {
            // New element!
            auto const END = _data + size;
            for (auto itr = _data + _size; itr != END; ++itr) {
                _allocator.construct(itr);
            }
            _size = size;

        } else if (size < _size) {
            // Remove element!
            auto const END = _data + _size;
            for (auto itr = _data + size; itr != END; ++itr) {
                _allocator.destroy(itr);
            }
            _size = size;

        } else {
            // No reaction.
            assert(_size == size);
        }
    }

    void clear()
    {
        resize(0);
    }
};

} // namespace bag
} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_CONTAINER_HPP__

