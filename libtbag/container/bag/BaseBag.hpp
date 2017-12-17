/**
 * @file   BaseBag.hpp
 * @brief  BaseBag class prototype.
 * @author zer0
 * @date   2017-12-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_BASEBAG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_BASEBAG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <vector>
#include <initializer_list>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {
namespace bag       {

/**
 * BaseBag class prototype.
 *
 * @author zer0
 * @date   2017-12-08
 */
template <typename T, typename Allocator = std::allocator<T> >
class BaseBag
{
public:
    using Dimension = std::vector<unsigned int>;
    using Vector    = std::vector<T, Allocator>;

    using dim_type               = Dimension::value_type;
    using value_type             = typename Vector::value_type;
    using allocator_type         = typename Vector::allocator_type;
    using reference              = typename Vector::reference;
    using const_reference        = typename Vector::const_reference;
    using size_type              = typename Vector::size_type;
    using difference_type        = typename Vector::difference_type;
    using pointer                = typename Vector::pointer;
    using const_pointer          = typename Vector::const_pointer;
    using iterator               = typename Vector::iterator;
    using const_iterator         = typename Vector::const_iterator;
    using reverse_iterator       = typename Vector::reverse_iterator;
    using const_reverse_iterator = typename Vector::const_reverse_iterator;

    using DimensionList = std::initializer_list<dim_type>;

private:
    Dimension  _dimension;
    Vector     _vector;

public:
    BaseBag() : _dimension(), _vector()
    { /* EMPTY. */ }

    BaseBag(DimensionList dims) : BaseBag()
    {
        size_type total = 0;
        for (auto & dim : dims) {
            _dimension.emplace_back(dim);
            total += dim;
        }
        _vector.resize(total);
    }

    BaseBag(BaseBag const & obj) : BaseBag()
    { *this = obj; }

    BaseBag(BaseBag && obj) : BaseBag()
    { *this = std::move(obj); }

    virtual ~BaseBag()
    { /* EMPTY. */ }

public:
    reference operator =(BaseBag const & obj)
    {
        if (this != &obj) {
            _dimension = obj._dimension;
            _vector = obj._vector;
        }
        return *this;
    }

    reference operator =(BaseBag && obj)
    {
        swap(obj);
        return *this;
    }

public:
    void swap(BaseBag & obj)
    {
        if (this != &obj) {
            _dimension.swap(obj._dimension);
            _vector.swap(obj._vector);
        }
    }

    inline friend void swap(BaseBag & lh, BaseBag & rh) { lh.swap(rh); }
};

} // namespace bag
} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_BASEBAG_HPP__

