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
template <typename T, typename Allocator>
class BaseBag
{
public:
    using value_type = T;

public:
    using Dimension = std::vector<int>;
    using Vector    = std::vector<T>;

private:
    Dimension   _dimension;
    Vector      _vector;

public:
    BaseBag() : _dimension(), _vector()
    { /* EMPTY. */ }

    BaseBag(Dimension const & dim, Vector const & vec) : _dimension(dim), _vector(vec)
    { /* EMPTY. */ }

    BaseBag(BaseBag const & obj) : BaseBag()
    { /* EMPTY. */ }

    BaseBag(BaseBag && obj) : BaseBag()
    { /* EMPTY. */ }

    virtual ~BaseBag()
    { /* EMPTY. */ }

public:
    BaseBag & operator =(BaseBag const & obj)
    {
        if (this != &obj) {
            _dimension = obj._dimension;
            _vector = obj._vector;
        }
        return *this;
    }

    BaseBag & operator =(BaseBag && obj)
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

public:
};

} // namespace bag
} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_BAG_BASEBAG_HPP__

