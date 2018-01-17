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
#define _TBAG_XX(name, symbol, type) type symbol;
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
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
    template <typename T> using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;

public:
    TBAG_CONSTEXPR static std::size_t size() TBAG_NOEXCEPT { return sizeof(UnionData); }

private:
    TypeTable _type;
    UnionData _data;

public:
    template <typename T>
    inline AnyPod(T data, TypeTable type = TypeInfo<T>::table()) : _type(type), _data()
    { ::memcpy(&_data, &data, sizeof(T)); }

    inline AnyPod() : _type(TypeTable::TT_UNKNOWN), _data()
    { ::memset(&_data, 0x00, sizeof(_data)); }

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
            _type = obj._type;
            _data = obj._data;
        }
    }

    inline void swap(AnyPod & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(_type, obj._type);
            std::swap(_data, obj._data);
        }
    }

    inline friend void swap(AnyPod & lh, AnyPod & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline TypeTable type() const TBAG_NOEXCEPT { return _type; }
    inline UnionData data() const TBAG_NOEXCEPT { return _data; }

public:
    inline bool operator ==(AnyPod const & obj) const TBAG_NOEXCEPT
    { return ::memcmp(&_data, &obj._data, sizeof(_data)) == 0; }
    inline bool operator !=(AnyPod const & obj) const TBAG_NOEXCEPT
    { return !this->operator==(obj); }

public:
    template <typename T>
    inline T cast() const TBAG_NOEXCEPT
    { return *((T*)(&_data)); }

#define _TBAG_XX(name, symbol, type) \
    inline void set(type v) TBAG_NOEXCEPT { _data.symbol = v; _type = TypeTable::TT_##name; } \
    inline AnyPod & operator =(type v) { set(v); return *this; } \
    /* -- END -- */
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ANYPOD_HPP__

