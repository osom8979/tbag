/**
 * @file   Dimension.hpp
 * @brief  Dimension class prototype.
 * @author zer0
 * @date   2018-07-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_DIMENSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_DIMENSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <initializer_list>
#include <algorithm>
#include <utility>

#ifndef DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG
#define DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG 4
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {
namespace bag       {

/**
 * Base dimension interface of Bag class.
 *
 * @tparam L Max dimensions.
 *
 * @author zer0
 * @date   2018-04-25
 * @date   2018-07-28 (Move: libtbag/container -> libtbag/container/bag)
 *
 * @remarks
 *  High dimension <- * -> Low dimension @n
 *  Low index <- * -> High index @n
 *  0, 1, 2, ..., 5, 6, 7, ... @n
 *  Example: @n
 *   - Image matrix: depth(0), height(1), width(2), channels(3)
 */
template <std::size_t L = DEFAULT_ELEMENTS_BUFFER_SIZE_OF_BAG>
class Dimension
{
public:
    using DimValue     = std::size_t;
    using DimValueInfo = libtbag::type::TypeInfo<DimValue>;

public:
    TBAG_CONSTEXPR static DimValue const NOT_FOUND   = DimValueInfo::maximum();
    TBAG_CONSTEXPR static DimValue const MAX_VALUE   = DimValueInfo::maximum() - 1;
    TBAG_CONSTEXPR static DimValue const BUFFER_SIZE = L;

private:
    DimValue  _dims[BUFFER_SIZE];
    DimValue  _size;

public:
    Dimension() TBAG_NOEXCEPT : _dims{0,}, _size(0)
    {
        // EMPTY.
    }

    template <typename Tp>
    Dimension(std::initializer_list<Tp> list) TBAG_NOEXCEPT : Dimension()
    {
        resize(list.begin(), list.end());
    }

    Dimension(Dimension const & obj) TBAG_NOEXCEPT : Dimension()
    {
        copy(obj);
    }

    Dimension(Dimension && obj) TBAG_NOEXCEPT : Dimension()
    {
        swap(obj);
    }

    ~Dimension() TBAG_NOEXCEPT
    {
        // EMPTY.
    }

public:
    Dimension & operator =(Dimension const & obj) TBAG_NOEXCEPT
    {
        copy(obj);
        return *this;
    }

    Dimension & operator =(Dimension && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

    void copy(Dimension const & obj) TBAG_NOEXCEPT
    {
        if (this == &obj) {
            return;
        }
        std::copy(obj._dims, obj._dims + buffer_size(), _dims);
        _size = obj._size;
    }

    void swap(Dimension & obj) TBAG_NOEXCEPT
    {
        std::swap_ranges(_dims, _dims + buffer_size(), obj._dims);
        std::swap(_size, obj._size);
    }

    inline friend void swap(Dimension & lh, Dimension & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    bool operator ==(Dimension const & obj) const TBAG_NOEXCEPT
    {
        if (this == &obj) {
            return true;
        }
        if (_size != obj._size) {
            return false;
        }
        return std::equal(_dims, _dims + buffer_size(), obj._dims);
    }

    bool operator !=(Dimension const & obj) const TBAG_NOEXCEPT
    {
        return !(this->operator==(obj));
    }

public:
    void clear() TBAG_NOEXCEPT
    {
        _size = 0;
    }

public:
    template <typename Tp>
    Dimension & operator =(std::initializer_list<Tp> list) TBAG_NOEXCEPT
    {
        resize(list.begin(), list.end());
        return *this;
    }

    template <typename Iterator>
    void resize(Iterator begin, Iterator end) TBAG_NOEXCEPT
    {
        auto const DEST_END = _dims + buffer_size();
        auto dest = _dims;
        for (; begin != end && dest != DEST_END; ++begin, ++dest) {
            *dest = *begin;
        }
        _size = std::distance(_dims, dest);
    }

    void resize(DimValue const * dims, DimValue size) TBAG_NOEXCEPT
    {
        resize(dims, dims + size);
    }

    template <typename Tp>
    void resize(std::initializer_list<Tp> list) TBAG_NOEXCEPT
    {
        return resize(list.begin(), list.end());
    }

public:
    DimValue & operator[](DimValue index) TBAG_NOEXCEPT_ONLY_RELEASE
    {
        return at(index);
    }

    DimValue const & operator[](DimValue index) const TBAG_NOEXCEPT_ONLY_RELEASE
    {
        return at(index);
    }

    DimValue & at(DimValue index) TBAG_NOEXCEPT_ONLY_RELEASE
    {
        assert(0 <= COMPARE_AND(index) < buffer_size());
        return _dims[index];
    }

    DimValue const & at(DimValue index) const TBAG_NOEXCEPT_ONLY_RELEASE
    {
        assert(0 <= COMPARE_AND(index) < buffer_size());
        return _dims[index];
    }

public:
    TBAG_CONSTEXPR static DimValue buffer_size() TBAG_NOEXCEPT
    {
        return BUFFER_SIZE;
    }

    TBAG_CONSTEXPR static DimValue max_size() TBAG_NOEXCEPT
    {
        return MAX_VALUE;
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
    template <typename Iterator>
    static DimValue calcTotal(Iterator begin, Iterator end) TBAG_NOEXCEPT
    {
        if (begin == end) {
            return 0;
        }
        DimValue result = *begin++;
        for (; begin != end; ++begin) {
            result *= *begin;
        }
        return result;
    }

    static DimValue calcTotal(DimValue const * dims, DimValue size, DimValue begin = 0) TBAG_NOEXCEPT
    {
        return calcTotal(dims + begin, dims + size);
    }

    template <typename Tp>
    static DimValue calcTotal(std::initializer_list<Tp> list) TBAG_NOEXCEPT
    {
        return calcTotal(list.begin(), list.size());
    }

    DimValue total(DimValue size, DimValue begin = 0) const TBAG_NOEXCEPT
    {
        return calcTotal(_dims, size, begin);
    }

    DimValue total() const TBAG_NOEXCEPT
    {
        return total(_size);
    }

    DimValue subTotal(DimValue i = 0) const TBAG_NOEXCEPT
    {
        return calcTotal(_dims, _size, i + 1);
    }

public:
    template <typename Iterator, typename DimIterator>
    static DimValue calcOffset(Iterator begin, Iterator end, DimIterator dim_begin, DimIterator dim_end, DimValue & result) TBAG_NOEXCEPT
    {
        if (dim_begin == dim_end) {
            return 1;
        } else {
            if (begin == end) {
                return *dim_begin * calcOffset(begin + 0, end, dim_begin + 1, dim_end, result);
            } else {
                auto dim = calcOffset(begin + 1, end, dim_begin + 1, dim_end, result);
                result += (*begin * dim);
                return *dim_begin * dim;
            }
        }
    }

    template <typename Iterator>
    DimValue offset(Iterator begin, Iterator end) const TBAG_NOEXCEPT
    {
        DimValue result = 0;
        calcOffset(begin, end, _dims, _dims + _size, result);
        return result;
    }

    DimValue offset(DimValue const * dims, DimValue size) const TBAG_NOEXCEPT
    {
        return offset(dims, dims + size);
    }

    template <typename Tp>
    DimValue offset(std::initializer_list<Tp> list) const TBAG_NOEXCEPT
    {
        return offset(list.begin(), list.end());
    }
};

} // namespace bag
} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_DIMENSION_HPP__

