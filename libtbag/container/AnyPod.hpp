/**
 * @file   AnyPod.hpp
 * @brief  AnyPod class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ANYPOD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ANYPOD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <cstdint>
#include <cstring>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

union UnionData
{
    bool                   b;
    char                   c;
    signed char           sc;
    unsigned char         uc;
    wchar_t               wc;
    char16_t             c16;
    char32_t             c32;
    short                  s;
    unsigned short        us;
    int                    i;
    unsigned int          ui;
    long                   l;
    unsigned long         ul;
    long long             ll;
    unsigned long long   ull;
    float                  f;
    double                 d;
    long double           ld;
    void*                  p;
};

/**
 * AnyPod class prototype.
 *
 * @author zer0
 * @date   2018-01-16
 */
class AnyPod
{
public:
    template <typename T>
    using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;

public:
    TBAG_CONSTEXPR std::size_t size() const TBAG_NOEXCEPT { return sizeof(UnionData); }

public:
    TypeTable type;
    UnionData data;

public:
    template <typename T>
    inline AnyPod(T d, TypeTable t = TypeInfo<T>::table()) : type(t), data()
    { ::memcpy(&data, &d, sizeof(T)); }

    inline AnyPod() : type(TypeTable::TT_UNKNOWN), data()
    { ::memset(&data, 0x00, sizeof(data)); }

    inline AnyPod(AnyPod const & obj) : AnyPod() { (*this) = obj; }
    inline AnyPod(AnyPod && obj) : AnyPod() { (*this) = std::move(obj); }

    inline ~AnyPod() { /* EMPTY. */ }

public:
    inline AnyPod & operator =(AnyPod const & obj) { assign(obj); return *this; }
    inline AnyPod & operator =(AnyPod && obj) { swap(obj); return *this; }

public:
    inline void assign(AnyPod const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            type = obj.type;
            data = obj.data;
        }
    }

    inline void swap(AnyPod & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(type, obj.type);
            std::swap(data, obj.data);
        }
    }

    inline friend void swap(AnyPod & lh, AnyPod & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool operator ==(AnyPod const & obj) const TBAG_NOEXCEPT { return ::memcmp(&data, &obj.data, sizeof(data)) == 0; }
    inline bool operator !=(AnyPod const & obj) const TBAG_NOEXCEPT { return !this->operator==(obj); }
    inline bool operator <=(AnyPod const & obj) const TBAG_NOEXCEPT { return ::memcmp(&data, &obj.data, sizeof(data)) <= 0; }
    inline bool operator >=(AnyPod const & obj) const TBAG_NOEXCEPT { return ::memcmp(&data, &obj.data, sizeof(data)) >= 0; }
    inline bool operator < (AnyPod const & obj) const TBAG_NOEXCEPT { return !this->operator<=(obj); }
    inline bool operator > (AnyPod const & obj) const TBAG_NOEXCEPT { return !this->operator>=(obj); }

public:
    template <typename T>
    inline void set(T d, TypeTable t = TypeInfo<T>::table()) TBAG_NOEXCEPT
    {
        ::memcpy(&data, &d, sizeof(T));
        type = t;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ANYPOD_HPP__

