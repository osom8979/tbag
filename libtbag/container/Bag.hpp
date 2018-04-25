/**
 * @file   Bag.hpp
 * @brief  Bag class prototype.
 * @author zer0
 * @date   2018-04-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>
#include <vector>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

template <typename T,
          template <typename Tp> class A,
          typename C>
class BagContainer;

template <typename T, template <typename Tp> class A>
class BagContainer<T, A, std::vector<T, A<T> > >
{
public:
    using Value     = T;
    using Allocator = A<Value>;
    using Container = std::vector<T, Allocator>;

private:
    Container _container;

public:
    BagContainer() : _container()
    { /* EMPTY. */ }

    ~BagContainer()
    { /* EMPTY. */ }

public:
    void clear()
    {
        _container.clear();
    }

    void resize(std::size_t size)
    {
        _container.resize(size);
    }

    void swap(BagContainer & obj)
    {
        _container.swap(obj);
    }

    void copy(BagContainer const & obj)
    {
        _container = obj._container;
    }

    Value & at(std::size_t index)
    {
        return _container[index];
    }

    Value const & at(std::size_t index) const
    {
        return _container[index];
    }

    std::size_t size() const
    {
        return _container.size();
    }
};

/**
 * Bag class prototype.
 *
 * @author zer0
 * @date   2018-04-25
 */
template <typename T,
          template <typename Tp> class A = std::allocator,
          typename C = std::vector<T, A<T> > >
class Bag : protected BagContainer<T, A, C>
{
public:
    using Base = BagContainer<T, A, C>;

public:
    using Value        = typename Base::Value;
    using Allocator    = typename Base::Allocator;
    using Container    = typename Base::Container;

    using DimValue     = int;
    using DimAllocator = A<DimValue>;
    using Dimensions   = std::vector<int, DimAllocator>;

public:
    static_assert(!std::is_pointer<Value>::value, "Value must not be a pointer type");
    static_assert(!std::is_reference<Value>::value, "Value must not be a reference type");

private:
    Dimensions _dims;
    int _padding;

public:
    Bag() : Base(), _dims(), _padding()
    { /* EMPTY. */ }

    Bag(Bag const & obj) : Bag()
    {
        *this = obj;
    }

    Bag(Bag && obj) : Bag()
    {
        *this = std::move(obj);
    }

    ~Bag()
    { /* EMPTY. */ }

public:
    Bag & operator =(Bag const & obj)
    {
        copy(obj);
        return *this;
    }

    Bag & operator =(Bag && obj)
    {
        swap(obj);
        return *this;
    }

public:
    void copy(Bag const & obj)
    {
        if (this != &obj) {
            _dims = obj._dims;
            _padding = obj._padding;
            Base::copy(obj);
        }
    }

    void swap(Bag & obj)
    {
        if (this != &obj) {
            _dims.swap(obj._dims);
            std::swap(_padding, obj._padding);
            Base::swap(obj);
        }
    }

    friend void swap(Bag & lh, Bag & rh)
    {
        lh.swap(rh);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

