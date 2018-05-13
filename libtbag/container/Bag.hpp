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
#include <libtbag/Err.hpp>
#include <libtbag/algorithm/MinMax.hpp>

#include <cstdint>
#include <cassert>

#include <memory>
#include <vector>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <algorithm>

#define DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG 4

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Base container interface of Bag class.
 *
 * @tparam T  Base type.
 * @tparam A  Base allocator.
 * @tparam C  Base container.
 */
template <typename T,
          template <typename Tp> class A,
          typename C>
class BagContainer;

/**
 * Vector container interface of Bag class.
 *
 * @tparam T  Base type.
 * @tparam A  Base allocator.
 */
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
    BagContainer() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<Container>::value) : _container()
    { /* EMPTY. */ }

    ~BagContainer()
    { /* EMPTY. */ }

public:
    void clear() TBAG_NOEXCEPT_SP_OP(_container.clear())
    {
        _container.clear();
    }

    void resize(std::size_t size) TBAG_NOEXCEPT_SP_OP(_container.resize(size))
    {
        _container.resize(size);
    }

    void swap(BagContainer & obj) TBAG_NOEXCEPT_SP_OP(_container.swap(obj._container))
    {
        _container.swap(obj._container);
    }

    void copy(BagContainer const & obj) TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_copy_assignable<Container>::value)
    {
        _container = obj._container;
    }

    Value & at(std::size_t index) TBAG_NOEXCEPT_SP_OP(_container.operator[](index))
    {
        return _container.operator[](index);
    }

    Value const & at(std::size_t index) const TBAG_NOEXCEPT_SP_OP(_container.operator[](index))
    {
        return _container.operator[](index);
    }

    Value * data() TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

    Value const * data() const TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

    std::size_t size() const TBAG_NOEXCEPT_SP_OP(_container.size())
    {
        return _container.size();
    }

    bool empty() const TBAG_NOEXCEPT_SP_OP(_container.empty())
    {
        return _container.empty();
    }
};

/**
 * Base dimension interface of Bag class.
 *
 * @tparam L Max dimensions.
 */
template <unsigned L = DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG>
class BagDimensions
{
public:
    using DimValue = unsigned;

public:
    TBAG_CONSTEXPR static DimValue const ELEMENTS_BUFFER_SIZE = L;
    TBAG_CONSTEXPR static DimValue const NOT_FOUND = -1;

public:
    TBAG_CONSTEXPR static DimValue const maxDimSize() TBAG_NOEXCEPT { return ELEMENTS_BUFFER_SIZE; }
    TBAG_CONSTEXPR static DimValue const getNotFound() TBAG_NOEXCEPT  { return NOT_FOUND; }

private:
    DimValue _dims[ELEMENTS_BUFFER_SIZE];
    DimValue _size;

public:
    BagDimensions() TBAG_NOEXCEPT : _dims{0,}, _size(0)
    { /* EMPTY */ }

    ~BagDimensions()
    { /* EMPTY */ }

public:
    void clear() TBAG_NOEXCEPT
    {
        for (DimValue i = 0; i < ELEMENTS_BUFFER_SIZE; ++i) {
            _dims[i] = 0;
        }
        _size = 0;
    }

    void resize(DimValue const * dims, DimValue size)
    {
        assert(0 < COMPARE_AND(size) <= maxDimSize());
        DimValue const MIN_SIZE = libtbag::algorithm::getMin(size, maxDimSize());
        for (DimValue i = 0; i < MIN_SIZE; ++i) {
            _dims[i] = dims[i];
        }
        _size = size;
    }

    template <typename T>
    void resize(std::initializer_list<T> list)
    {
        resize((DimValue const *)list.begin(), (DimValue)list.size());
    }

    template <typename ... Args>
    void resize(Args && ... list)
    {
        resize({std::forward<Args>(list) ...});
    }

    void swap(BagDimensions & obj) TBAG_NOEXCEPT
    {
        for (DimValue i = 0; i < maxDimSize(); ++i) {
            std::swap(_dims[i], obj._dims[i]);
        }
        std::swap(_size, obj._size);
    }

    void copy(BagDimensions const & obj) TBAG_NOEXCEPT
    {
        for (DimValue i = 0; i < maxDimSize(); ++i) {
            _dims[i] = obj._dims[i];
        }
        _size = obj._size;
    }

    DimValue & at(DimValue index)
    {
        assert(0 <= COMPARE_AND(index) < maxDimSize());
        return _dims[index];
    }

    DimValue const & at(DimValue index) const
    {
        assert(0 <= COMPARE_AND(index) < maxDimSize());
        return _dims[index];
    }

    DimValue size() const TBAG_NOEXCEPT
    {
        return _size;
    }

    bool empty() const TBAG_NOEXCEPT
    {
        return _size == 0;
    }

    static DimValue calcTotal(DimValue const * dims, DimValue size)
    {
        if (size == 0) {
            return 0;
        }
        DimValue result = dims[0];
        assert(dims[0] > 0);
        for (DimValue i = 1; i < size; ++i) {
            assert(dims[i] > 0);
            result *= dims[i];
        }
        return result;
    }

    DimValue total() const
    {
        return calcTotal(_dims, _size);
    }

    DimValue offset(DimValue const * offsets, DimValue size)
    {
        assert(0 < COMPARE_AND(size) <= _size);
        DimValue const MIN_SIZE = libtbag::algorithm::getMin(size, _size);
        DimValue result = offsets[0];
        DimValue dim_step = _dims[0];
        assert(_dims[0] > 0);
        for (DimValue i = 1; i < MIN_SIZE; ++i) {
            result += offsets[i] * dim_step;
            assert(_dims[i] > 0);
            dim_step *= _dims[i];
        }
        return result;
    }

    template <typename T>
    DimValue offset(std::initializer_list<T> list)
    {
        return offset((DimValue const *)list.begin(), (DimValue)list.size());
    }

    template <typename ... Args>
    DimValue offset(Args && ... list)
    {
        return offset({std::forward<Args>(list) ...});
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
          typename C = std::vector<T, A<T> >,
          unsigned L = DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG>
class Bag : protected BagContainer<T, A, C>,
            protected BagDimensions<L>
{
public:
    using Self = Bag<T, A, C, L>;
    using BaseContainer   = BagContainer<T, A, C>;
    using BaseDimenstions = BagDimensions<L>;

public:
    using Value     = typename BaseContainer::Value;
    using Allocator = typename BaseContainer::Allocator;
    using Container = typename BaseContainer::Container;
    using DimValue  = typename BaseDimenstions::DimValue;

public:
    static_assert(!std::is_pointer  <Value>::value, "Value must not be a pointer type");
    static_assert(!std::is_reference<Value>::value, "Value must not be a reference type");

public:
    Bag() : BaseContainer(), BaseDimenstions()
    { /* EMPTY. */ }

    template <typename Tp>
    Bag(std::initializer_list<Tp> list) : Bag()
    {
        Self::resize(list);
    }

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
            BaseDimenstions::copy(obj);
            BaseContainer::copy(obj);
        }
    }

    void swap(Bag & obj)
    {
        if (this != &obj) {
            BaseDimenstions::swap(obj);
            BaseContainer::swap(obj);
        }
    }

    friend void swap(Bag & lh, Bag & rh)
    {
        lh.swap(rh);
    }

public:
    void resize(DimValue const * dims, DimValue size)
    {
        BaseDimenstions::resize(dims, size);
        BaseContainer::resize(BaseDimenstions::total());
    }

    template <typename Tp>
    void resize(std::initializer_list<Tp> list)
    {
        Self::resize((DimValue const *)list.begin(), (DimValue)list.size());
    }

    template <typename ... Args>
    void resize(Args && ... list)
    {
        Self::resize({std::forward<Args>(list) ...});
    }

    void clear()
    {
        BaseDimenstions::clear();
        BaseContainer::clear();
    }

public:
    DimValue size() const TBAG_NOEXCEPT_SP_OP(BaseDimenstions::total())
    {
        return BaseDimenstions::total();
    }

    DimValue size(DimValue index) const TBAG_NOEXCEPT_SP_OP(BaseDimenstions::at(index))
    {
        return BaseDimenstions::at(index);
    }

    bool empty() const TBAG_NOEXCEPT_SP_OP(BaseDimenstions::empty())
    {
        return BaseDimenstions::empty();
    }

public:
    Value & at(DimValue index) TBAG_NOEXCEPT_SP_OP(BaseContainer::at(index))
    {
        return BaseContainer::at(index);
    }

    Value const & at(DimValue index) const TBAG_NOEXCEPT_SP_OP(BaseContainer::at(index))
    {
        return BaseContainer::at(index);
    }

    template <typename ... Args>
    Value & atOffset(Args && ... args)
    {
        return BaseContainer::at(BaseDimenstions::offset({std::forward<Args>(args) ...}));
    }

    template <typename ... Args>
    Value const & atOffset(Args && ... args) const
    {
        return BaseContainer::at(BaseDimenstions::offset({std::forward<Args>(args) ...}));
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

