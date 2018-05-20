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
#include <libtbag/type/TypeTable.hpp>

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
    using Container = std::vector<Value, Allocator>;

public:
    using value_type        = typename Container::value_type;
    using allocator_type    = typename Container::allocator_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;
    using size_type         = typename Container::size_type;
    using difference_type   = typename Container::difference_type;
    using pointer           = typename Container::pointer;
    using const_pointer     = typename Container::const_pointer;

public:
    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                  "allocator_type::value_type must be same type as value_type");

private:
    Container _container;

public:
    BagContainer() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_default_constructible<Container>::value)
            : _container()
    { /* EMPTY. */ }

    ~BagContainer()
    { /* EMPTY. */ }

public:
    void clear() TBAG_NOEXCEPT_SP_OP(_container.clear())
    {
        _container.clear();
    }

    void resize(size_type size) TBAG_NOEXCEPT_SP_OP(_container.resize(size))
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

    reference at(size_type index)
    {
        return _container[index];
    }

    const_reference at(size_type index) const
    {
        return _container[index];
    }

    pointer data() TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

    const_pointer data() const TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

    size_type size() const TBAG_NOEXCEPT_SP_OP(_container.size())
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
    using DimValue = unsigned int;
    using DimValueInfo = libtbag::type::TypeInfo<DimValue>;

public:
    TBAG_CONSTEXPR static DimValue const MAX_DIM_VALUE = DimValueInfo::maximum();
    TBAG_CONSTEXPR static DimValue const BUFFER_SIZE   = L;
    TBAG_CONSTEXPR static DimValue const NOT_FOUND     = -1;

public:
    TBAG_CONSTEXPR static DimValue const getMaxDimValue() TBAG_NOEXCEPT { return MAX_DIM_VALUE; }
    TBAG_CONSTEXPR static DimValue const  getBufferSize() TBAG_NOEXCEPT { return BUFFER_SIZE; }
    TBAG_CONSTEXPR static DimValue const    getNotFound() TBAG_NOEXCEPT { return NOT_FOUND; }

private:
    DimValue _dims[BUFFER_SIZE];
    DimValue _size;

public:
    BagDimensions() TBAG_NOEXCEPT : _dims{0,}, _size(0)
    { /* EMPTY */ }

    ~BagDimensions()
    { /* EMPTY */ }

public:
    void clear() TBAG_NOEXCEPT
    {
        std::fill(_dims, _dims + BUFFER_SIZE, 0);
        _size = 0;
    }

public:
    void resize(DimValue const * dims, DimValue size) TBAG_NOEXCEPT_ONLY_RELEASE
    {
        assert(0 < COMPARE_AND(size) <= getBufferSize());
        DimValue const SIZE = libtbag::algorithm::getMin(size, getBufferSize());
        DimValue i = 0;
        for (; i < SIZE; ++i) {
            _dims[i] = dims[i];
        }
        for (; i < getBufferSize(); ++i) {
            _dims[i] = 0;
        }
        _size = size;
    }

    template <typename Tp>
    void resize(std::initializer_list<Tp> list) TBAG_NOEXCEPT_ONLY_RELEASE
    {
        return resize((DimValue const *)list.begin(), (DimValue)list.size());
    }

public:
    void swap(BagDimensions & obj) TBAG_NOEXCEPT
    {
        std::swap_ranges(_dims, _dims + BUFFER_SIZE, obj._dims);
        std::swap(_size, obj._size);
    }

    void copy(BagDimensions const & obj) TBAG_NOEXCEPT
    {
        std::copy(obj._dims, obj._dims + BUFFER_SIZE, _dims);
        _size = obj._size;
    }

public:
    DimValue & at(DimValue index)
    {
        assert(0 <= COMPARE_AND(index) < getBufferSize());
        return _dims[index];
    }

    DimValue const & at(DimValue index) const
    {
        assert(0 <= COMPARE_AND(index) < getBufferSize());
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

public:
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

    template <typename Tp>
    static DimValue calcTotal(std::initializer_list<Tp> list)
    {
        return calcTotal((DimValue const *)list.begin(), (DimValue)list.size());
    }

    DimValue total(DimValue size) const
    {
        return calcTotal(_dims, size);
    }

    DimValue total() const
    {
        return total(_size);
    }

public:
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

    template <typename Tp>
    DimValue offset(std::initializer_list<Tp> list)
    {
        return offset((DimValue const *)list.begin(), (DimValue)list.size());
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
class Bag
{
public:
    using Self        = Bag<T, A, C, L>;
    using Container   = BagContainer<T, A, C>;
    using Dimenstions = BagDimensions<L>;

public:
    using value_type        = typename Container::value_type;
    using allocator_type    = typename Container::allocator_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;
    using size_type         = typename Dimenstions::DimValue;
    using difference_type   = typename Container::difference_type;
    using pointer           = typename Container::pointer;
    using const_pointer     = typename Container::const_pointer;

public:
    static_assert(!std::is_pointer<value_type>::value,
                  "value_type must not be a pointer type");
    static_assert(!std::is_reference<value_type>::value,
                  "value_type must not be a reference type");
    static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                  "allocator_type::value_type must be same type as value_type");

public:
    template <typename V,
              typename R = reference,
              typename P = pointer,
              typename D = difference_type>
    class BagIterator : public std::iterator<std::bidirectional_iterator_tag, V, D, P, R>
    {
    public:
        using Self = BagIterator<V, R, P>;
        using Base = std::iterator<std::bidirectional_iterator_tag, V, std::ptrdiff_t, P, R>;

    public:
        using value_type        = typename Base::value_type;
        using difference_type   = typename Base::difference_type;
        using pointer           = typename Base::pointer;
        using reference         = typename Base::reference;
        using iterator_category = typename Base::iterator_category;

    private:
        pointer          _cursor;
        difference_type  _step;

    public:
        BagIterator(pointer cursor = nullptr, difference_type step = 1) TBAG_NOEXCEPT
                : _cursor(cursor), _step(step)
        { /* EMPTY. */ }

        BagIterator(BagIterator const & obj) TBAG_NOEXCEPT
                : _cursor(obj._cursor), _step(obj._step)
        { /* EMPTY. */ }

        ~BagIterator()
        { /* EMPTY. */ }

    public:
        inline reference operator  *() const TBAG_NOEXCEPT { return *_cursor; }
        inline pointer   operator ->() const TBAG_NOEXCEPT { return  _cursor; }

    public:
        BagIterator & operator =(BagIterator const & obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                _cursor = obj._cursor;
                _step = obj._step;
            }
            return *this;
        }

    public:
        inline bool operator ==(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor == obj._cursor; }
        inline bool operator !=(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor != obj._cursor; }

    public:
        inline bool operator <(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor < obj._cursor; }
        inline bool operator >(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor > obj._cursor; }

    public:
        inline bool operator <=(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor <= obj._cursor; }
        inline bool operator >=(BagIterator const & obj) const TBAG_NOEXCEPT
        { return _cursor >= obj._cursor; }

    public:
        inline BagIterator & operator ++() TBAG_NOEXCEPT
        {
            _cursor += _step;
            return *this;
        }

        inline BagIterator & operator --() TBAG_NOEXCEPT
        {
            _cursor -= _step;
            return *this;
        }

    public:
        inline BagIterator operator ++(int) TBAG_NOEXCEPT
        {
            BagIterator itr(*this);
            _cursor += _step;
            return itr;
        }

        inline BagIterator operator --(int) TBAG_NOEXCEPT
        {
            BagIterator itr(*this);
            _cursor -= _step;
            return itr;
        }
    };

public:
    using       iterator = BagIterator<value_type,       reference,       pointer>;
    using const_iterator = BagIterator<value_type, const_reference, const_pointer>;

    using       reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    Dimenstions  _dimenstions;
    Container    _container;

public:
    Bag() TBAG_NOEXCEPT_SPECIFIER(
            std::is_nothrow_default_constructible<Dimenstions>::value &&
            std::is_nothrow_default_constructible<Container>::value)
            : _dimenstions(), _container()
    { /* EMPTY. */ }

    template <typename Tp>
    Bag(std::initializer_list<Tp> list) : Bag()
    {
        resize(list);
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
            _dimenstions.copy(obj._dimenstions);
            _container.copy(obj._container);
        }
    }

    void swap(Bag & obj)
    {
        if (this != &obj) {
            _dimenstions.swap(obj._dimenstions);
            _container.swap(obj._container);
        }
    }

    friend void swap(Bag & lh, Bag & rh)
    {
        lh.swap(rh);
    }

public:
    pointer data() TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

    const_pointer data() const TBAG_NOEXCEPT_SP_OP(_container.data())
    {
        return _container.data();
    }

public:
    // @formatter:off
          iterator  begin()       TBAG_NOEXCEPT { return       iterator(data()); }
    const_iterator  begin() const TBAG_NOEXCEPT { return const_iterator(data()); }
          iterator    end()       TBAG_NOEXCEPT { return       iterator(data() + size()); }
    const_iterator    end() const TBAG_NOEXCEPT { return const_iterator(data() + size()); }
    // @formatter:on

public:
    // @formatter:off
          reverse_iterator rbegin()       TBAG_NOEXCEPT { return       reverse_iterator(  end()); }
    const_reverse_iterator rbegin() const TBAG_NOEXCEPT { return const_reverse_iterator(  end()); }
          reverse_iterator   rend()       TBAG_NOEXCEPT { return       reverse_iterator(begin()); }
    const_reverse_iterator   rend() const TBAG_NOEXCEPT { return const_reverse_iterator(begin()); }
    // @formatter:on

public:
    // @formatter:off
            const_iterator  cbegin() const TBAG_NOEXCEPT { return  begin(); }
            const_iterator    cend() const TBAG_NOEXCEPT { return    end(); }
    const_reverse_iterator crbegin() const TBAG_NOEXCEPT { return rbegin(); }
    const_reverse_iterator   crend() const TBAG_NOEXCEPT { return   rend(); }
    // @formatter:on

public:
    void resize(size_type const * dims, size_type size)
    {
        assert(size <= max_size());
        _dimenstions.resize(dims, size);
        _container.resize(_dimenstions.total());
        assert(_container.size() == _dimenstions.total());
    }

    template <typename Tp>
    void resize(std::initializer_list<Tp> list)
    {
        static_assert(std::is_integral<typename std::initializer_list<Tp>::value_type>::value,
                      "Only integers are supported.");
        resize((size_type const *)list.begin(), (size_type)list.size());
    }

    template <typename ... Args>
    void resize(Args && ... list)
    {
        resize({std::forward<Args>(list) ...});
    }

    void clear()
    {
        _dimenstions.clear();
        _container.clear();
    }

public:
    size_type max_size() const TBAG_NOEXCEPT
    {
        return Dimenstions::getMaxDimValue();
    }

    size_type size() const TBAG_NOEXCEPT_SP_OP(_container.size())
    {
        return static_cast<size_type>(_container.size());
    }

    size_type size(size_type index) const TBAG_NOEXCEPT_SP_OP(_dimenstions.at(index))
    {
        return _dimenstions.at(index);
    }

    bool empty() const TBAG_NOEXCEPT_SP_OP(_dimenstions.empty())
    {
        return _dimenstions.empty();
    }

public:
    reference at(size_type index) TBAG_NOEXCEPT_SP_OP(_container.at(index))
    {
        return _container.at(index);
    }

    const_reference at(size_type index) const TBAG_NOEXCEPT_SP_OP(_container.at(index))
    {
        return _container.at(index);
    }

    reference operator[](size_type index) TBAG_NOEXCEPT_SP_OP(_container.at(index))
    {
        return _container.at(index);
    }

    const_reference operator[](size_type index) const TBAG_NOEXCEPT_SP_OP(_container.at(index))
    {
        return _container.at(index);
    }

    template <typename ... Args>
    reference atDims(Args && ... args)
    {
        return _container.at(_dimenstions.offset({std::forward<Args>(args) ...}));
    }

    template <typename ... Args>
    const_reference atDims(Args && ... args) const
    {
        return _container.at(_dimenstions.offset({std::forward<Args>(args) ...}));
    }

public:
    void assign(const_pointer data, size_type size)
    {
        std::copy(data, libtbag::algorithm::getMin(size, size()), _container.data());
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

