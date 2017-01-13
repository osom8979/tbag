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
#include <libtbag/Type.hpp>

#include <functional>
#include <algorithm>
#include <type_traits>

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
    using Type           = T;
    using Reference      = typename std::add_lvalue_reference<Type>::type;
    using ConstReference = const Reference;

    Type * ptr = nullptr;

    TBAG_CONSTEXPR Pointer(Type * p) TBAG_NOEXCEPT : ptr(p)
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

    inline explicit operator bool() const TBAG_NOEXCEPT
    { return ptr != nullptr; }

    inline Reference operator *() TBAG_NOEXCEPT
    { return *ptr; }
    inline ConstReference operator *() const TBAG_NOEXCEPT
    { return *ptr; }

    inline Type * operator ->() TBAG_NOEXCEPT
    { return ptr; }
    inline Type const * operator ->() const TBAG_NOEXCEPT
    { return ptr; }

    inline Type * get() TBAG_NOEXCEPT
    { return ptr; }
    inline Type const * get() const TBAG_NOEXCEPT
    { return ptr; }

    template <typename CastType>
    inline Pointer<CastType> to() TBAG_NOEXCEPT
    { return Pointer<CastType>(cast<CastType>()); }

    // @formatter:off
    template <typename CastType>
    inline typename std::enable_if<
            std::is_convertible< // void types can not use std::is_convertible.
                    typename std::conditional< // Do not choose void type.
                            std::is_same<Type, void>::value,
                            CastType*, Type*
                    >::type,
                    CastType*
            >::value,
            CastType*
    >::type
    cast() const TBAG_NOEXCEPT
    { return static_cast<CastType*>(ptr); }
    // @formatter:on

    template <typename CastType>
    inline CastType * forceCast() const TBAG_NOEXCEPT
    { return (CastType*)(ptr); }

    // ------------------------
    // Specialization of class.
    // ------------------------

    struct Hash
    {
        using argument_type  = Pointer;
        using hash_base_type = Type;
        using result_type    = std::size_t;

        inline result_type operator()(argument_type const & obj) const
        { return std::hash<hash_base_type*>()(obj.ptr); }
    };

    struct EqualTo
    {
        using first_argument_type  = Pointer;
        using second_argument_type = Pointer;
        using result_type          = bool;

        inline result_type operator()(first_argument_type  const & v1
                                    , second_argument_type const & v2) const
        { return v1 == v2; }
    };

    struct Less
    {
        using first_argument_type  = Pointer;
        using second_argument_type = Pointer;
        using result_type          = bool;

        inline result_type operator()(first_argument_type  const & v1
                                    , second_argument_type const & v2) const
        { return v1 < v2; }
    };
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_POINTER_HPP__

