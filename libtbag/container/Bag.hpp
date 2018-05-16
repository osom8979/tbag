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

    reference at(size_type index) TBAG_NOEXCEPT_SP_OP(_container.operator[](index))
    {
        return _container.operator[](index);
    }

    const_reference at(std::size_t index) const TBAG_NOEXCEPT_SP_OP(_container.operator[](index))
    {
        return _container.operator[](index);
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
    using DimValue = unsigned;

public:
    TBAG_CONSTEXPR static DimValue const ELEMENTS_BUFFER_SIZE = L;
    TBAG_CONSTEXPR static DimValue const NOT_FOUND = -1;

public:
    TBAG_CONSTEXPR static DimValue const  maxDimSize() TBAG_NOEXCEPT { return ELEMENTS_BUFFER_SIZE; }
    TBAG_CONSTEXPR static DimValue const getNotFound() TBAG_NOEXCEPT { return NOT_FOUND; }

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
        std::fill(_dims, _dims + ELEMENTS_BUFFER_SIZE, 0);
        _size = 0;
    }

public:
    void resize(DimValue const * dims, DimValue size)
    {
        assert(0 < COMPARE_AND(size) <= maxDimSize());
        DimValue const MIN_SIZE = libtbag::algorithm::getMin(size, maxDimSize());
        for (DimValue i = 0; i < MIN_SIZE; ++i) {
            _dims[i] = dims[i];
        }
        _size = size;
    }

    template <typename Tp>
    void resize(std::initializer_list<Tp> list)
    {
        resize((DimValue const *)list.begin(), (DimValue)list.size());
    }

public:
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

public:
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
class Bag
{
public:
    using Self = Bag<T, A, C, L>;
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
    static_assert(!std::is_pointer  <value_type>::value, "value_type must not be a pointer type");
    static_assert(!std::is_reference<value_type>::value, "value_type must not be a reference type");

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
          iterator  begin(difference_type step = 1)       TBAG_NOEXCEPT { return       iterator(data(), step); }
    const_iterator  begin(difference_type step = 1) const TBAG_NOEXCEPT { return const_iterator(data(), step); }
          iterator    end(difference_type step = 1)       TBAG_NOEXCEPT { return       iterator(data() + size(), step); }
    const_iterator    end(difference_type step = 1) const TBAG_NOEXCEPT { return const_iterator(data() + size(), step); }
    // @formatter:on

public:
    // @formatter:off
          reverse_iterator rbegin(difference_type step = 1)       TBAG_NOEXCEPT { return       reverse_iterator(  end(step)); }
    const_reverse_iterator rbegin(difference_type step = 1) const TBAG_NOEXCEPT { return const_reverse_iterator(  end(step)); }
          reverse_iterator   rend(difference_type step = 1)       TBAG_NOEXCEPT { return       reverse_iterator(begin(step)); }
    const_reverse_iterator   rend(difference_type step = 1) const TBAG_NOEXCEPT { return const_reverse_iterator(begin(step)); }
    // @formatter:on

public:
    // @formatter:off
            const_iterator  cbegin(difference_type step = 1) const TBAG_NOEXCEPT { return  begin(step); }
            const_iterator    cend(difference_type step = 1) const TBAG_NOEXCEPT { return    end(step); }
    const_reverse_iterator crbegin(difference_type step = 1) const TBAG_NOEXCEPT { return rbegin(step); }
    const_reverse_iterator   crend(difference_type step = 1) const TBAG_NOEXCEPT { return   rend(step); }
    // @formatter:on

public:
    void resize(size_type const * dims, size_type size)
    {
        _dimenstions.resize(dims, size);
        _container.resize(_dimenstions.total());
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
        return Dimenstions::maxDimSize();
    }

    size_type size() const TBAG_NOEXCEPT_SP_OP(_dimenstions.total())
    {
        return _dimenstions.total();
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

    template <typename ... Args>
    reference atOffset(Args && ... args)
    {
        return _container.at(_dimenstions.offset({std::forward<Args>(args) ...}));
    }

    template <typename ... Args>
    const_reference atOffset(Args && ... args) const
    {
        return _container.at(_dimenstions.offset({std::forward<Args>(args) ...}));
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_HPP__

