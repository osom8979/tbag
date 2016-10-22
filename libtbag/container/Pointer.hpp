/**
 * @file   Pointer.hpp
 * @brief  Pointer class prototype.
 * @author zer0
 * @date   2016-10-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_POINTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_POINTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <functional>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Pointer class prototype.
 *
 * @author zer0
 * @date   2016-10-16
 */
template <typename T>
struct Pointer
{
    using Type = T;
    Type * ptr = nullptr;

    constexpr Pointer(Type * p) TBAG_NOEXCEPT : ptr(p)
    { /* EMPTY. */ }

    inline Pointer() TBAG_NOEXCEPT : ptr(nullptr)
    { /* EMPTY. */ }
    inline ~Pointer()
    { /* EMPTY. */ }

    inline Pointer(Pointer const & obj) TBAG_NOEXCEPT
    { if (this != &obj) { ptr = obj.ptr; } }
    inline Pointer(Pointer && obj) TBAG_NOEXCEPT
    { if (this != &obj) { std::swap(ptr, obj.ptr); } }

    inline Pointer & operator =(Type * p) TBAG_NOEXCEPT
    { ptr = p; return *this; }

    inline Pointer & operator =(Pointer const & obj) TBAG_NOEXCEPT
    { if (this != &obj) { ptr = obj.ptr; } return *this; }
    inline Pointer & operator =(Pointer && obj) TBAG_NOEXCEPT
    { if (this != &obj) { std::swap(ptr, obj.ptr); } return *this; }

    inline bool operator ==(Pointer const & obj) const TBAG_NOEXCEPT
    { return ptr == obj.ptr; }
    inline bool operator !=(Pointer const & obj) const TBAG_NOEXCEPT
    { return ptr != obj.ptr; }
    inline bool operator <(Pointer const & obj) const TBAG_NOEXCEPT
    { return ptr < obj.ptr; }
    inline bool operator >(Pointer const & obj) const TBAG_NOEXCEPT
    { return ptr > obj.ptr; }

    inline operator bool() const TBAG_NOEXCEPT
    { return ptr != nullptr; }

    inline Type * operator ->() TBAG_NOEXCEPT
    { return ptr; }
    inline Type const * operator ->() const TBAG_NOEXCEPT
    { return ptr; }

    template <typename CastType>
    inline CastType * cast() TBAG_NOEXCEPT
    { return static_cast<CastType*>(ptr); }

    inline Type * get() TBAG_NOEXCEPT
    { return ptr; }
    inline Type const * get() const TBAG_NOEXCEPT
    { return ptr; }

    /**
     * Specialization of class template for @c libtbag::container::Pointer.
     *
     * @author zer0
     * @date   2016-10-16
     */
    struct Hash
    {
        using argument_type  = Pointer;
        using hash_base_type = Type;
        using result_type    = std::size_t;

        inline result_type operator()(argument_type const & obj) const
        {
            return std::hash<hash_base_type*>()(obj.ptr);
        }
    };
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_POINTER_HPP__

