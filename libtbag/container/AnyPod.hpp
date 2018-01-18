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
#include <exception>
#include <string>

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
class TBAG_API AnyPod
{
public:
    template <typename T>
    using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;

public:
    TBAG_CONSTEXPR static std::size_t size() TBAG_NOEXCEPT { return sizeof(UnionData); }

private:
    TypeTable _type;
    UnionData _data;

public:
    AnyPod();
    AnyPod(AnyPod const & obj);
    AnyPod(AnyPod && obj);
    ~AnyPod();

public:
    template <typename T>
    AnyPod(T data) : AnyPod()
    { set(data); }

public:
    AnyPod & operator =(AnyPod const & obj);
    AnyPod & operator =(AnyPod && obj);

public:
    void assign(AnyPod const & obj) TBAG_NOEXCEPT;
    void swap(AnyPod & obj) TBAG_NOEXCEPT;

    inline friend void swap(AnyPod & lh, AnyPod & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool       exists() const TBAG_NOEXCEPT { return _type != TypeTable::TT_UNKNOWN; }
    inline TypeTable    type() const TBAG_NOEXCEPT { return _type; }
    inline char const * name() const TBAG_NOEXCEPT { return type::getTypeName(_type); }

    inline UnionData       & data()       TBAG_NOEXCEPT { return _data; }
    inline UnionData const & data() const TBAG_NOEXCEPT { return _data; }

#define _TBAG_XX(name, symbol, type) \
    inline void set(type v) TBAG_NOEXCEPT { _data.symbol = v; _type = TypeTable::TT_##name; }   \
    inline AnyPod & operator =(type v) { set(v); return *this; }                                \
    /* -- END -- */
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX

public:
    template <typename T>
    T cast() const TBAG_NOEXCEPT
    {
#define _TBAG_XX(name, symbol, type)        \
        if (_type == TypeTable::TT_##name) {\
            return (T)_data.symbol;         \
        } /* -- END -- */
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
#undef _TBAG_POINT_XX
        return *((T*)(&_data));
    }

public:
    int compare(AnyPod const & obj) const TBAG_NOEXCEPT;

public:
    bool operator ==(AnyPod const & obj) const TBAG_NOEXCEPT;
    bool operator !=(AnyPod const & obj) const TBAG_NOEXCEPT;
    bool operator  >(AnyPod const & obj) const TBAG_NOEXCEPT;
    bool operator  <(AnyPod const & obj) const TBAG_NOEXCEPT;
    bool operator >=(AnyPod const & obj) const TBAG_NOEXCEPT;
    bool operator <=(AnyPod const & obj) const TBAG_NOEXCEPT;

    template <typename T> bool operator ==(T v) const TBAG_NOEXCEPT { return this->operator ==(AnyPod(v)); }
    template <typename T> bool operator !=(T v) const TBAG_NOEXCEPT { return this->operator !=(AnyPod(v)); }
    template <typename T> bool operator  >(T v) const TBAG_NOEXCEPT { return this->operator  >(AnyPod(v)); }
    template <typename T> bool operator  <(T v) const TBAG_NOEXCEPT { return this->operator  <(AnyPod(v)); }
    template <typename T> bool operator >=(T v) const TBAG_NOEXCEPT { return this->operator >=(AnyPod(v)); }
    template <typename T> bool operator <=(T v) const TBAG_NOEXCEPT { return this->operator <=(AnyPod(v)); }

public:
    std::string toString() const;
};

template <typename T> bool operator ==(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v) == obj; }
template <typename T> bool operator !=(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v) != obj; }
template <typename T> bool operator  >(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v)  > obj; }
template <typename T> bool operator  <(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v)  < obj; }
template <typename T> bool operator >=(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v) >= obj; }
template <typename T> bool operator <=(T v, AnyPod const & obj) TBAG_NOEXCEPT { return AnyPod(v) <= obj; }

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_ANYPOD_HPP__

