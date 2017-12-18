/**
 * @file   TypeTable.hpp
 * @brief  TypeTable class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPE_TYPETABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPE_TYPETABLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <numeric>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace type {

#ifndef TBAG_TYPE_TABLE_MAP
#define TBAG_TYPE_TABLE_MAP(_TBAG_XX) \
    _TBAG_XX(   BOOL,               bool) \
    _TBAG_XX(   CHAR,               char) \
    _TBAG_XX(  UCHAR,      unsigned char) \
    _TBAG_XX(  WCHAR,            wchar_t) \
    _TBAG_XX( CHAR16,           char16_t) \
    _TBAG_XX( CHAR32,           char32_t) \
    _TBAG_XX(  SHORT,              short) \
    _TBAG_XX( USHORT,     unsigned short) \
    _TBAG_XX(    INT,                int) \
    _TBAG_XX(   UINT,       unsigned int) \
    _TBAG_XX(   LONG,               long) \
    _TBAG_XX(  ULONG,      unsigned long) \
    _TBAG_XX(  LLONG,          long long) \
    _TBAG_XX( ULLONG, unsigned long long) \
    _TBAG_XX(  FLOAT,              float) \
    _TBAG_XX( DOUBLE,             double) \
    _TBAG_XX(LDOUBLE,        long double) \
    _TBAG_XX(  POINT,              void*) \
    /* -- END -- */
#endif

/**
 * TypeTable class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
enum class TypeTable : int
{
    TT_UNKNOWN = 0,
#define _TBAG_XX(name, type) TT_##name,
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
};

#if defined(TBAG_COMP_MSVC)
# if defined(min)
TBAG_PUSH_MACRO(min);
# undef min
# define __RESTORE_MIN__
# endif // defined(min)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif // defined(TBAG_COMP_MSVC)

template <typename T> struct TypeInfo;

template <typename T>
struct TypeInfo : public std::false_type
{
    using type = T;

    TBAG_CONSTEXPR static char const * const name() TBAG_NOEXCEPT { return "UNKNOWN"; }
    TBAG_CONSTEXPR static int size() TBAG_NOEXCEPT { return 0; }

    TBAG_CONSTEXPR static int index() TBAG_NOEXCEPT { return static_cast<int>(TypeTable::TT_UNKNOWN); }
    TBAG_CONSTEXPR static TypeTable table() TBAG_NOEXCEPT { return TypeTable::TT_UNKNOWN; }

    TBAG_CONSTEXPR static int maximum() TBAG_NOEXCEPT { return 0; }
    TBAG_CONSTEXPR static int minimum() TBAG_NOEXCEPT { return 0; }
};

#define _TBAG_XX(n, t)                                                                              \
template <> struct TypeInfo<t> : public std::true_type {                                            \
    using type = t;                                                                                 \
    TBAG_CONSTEXPR static char const * const name() TBAG_NOEXCEPT { return #n; }                    \
    TBAG_CONSTEXPR static int size() TBAG_NOEXCEPT { return sizeof(t); }                            \
    TBAG_CONSTEXPR static int index() TBAG_NOEXCEPT { return static_cast<int>(TypeTable::TT_##n); } \
    TBAG_CONSTEXPR static TypeTable table() TBAG_NOEXCEPT { return TypeTable::TT_##n; }             \
    TBAG_CONSTEXPR static t maximum() TBAG_NOEXCEPT { return std::numeric_limits<t>::max(); }       \
    TBAG_CONSTEXPR static t minimum() TBAG_NOEXCEPT { return std::numeric_limits<t>::min(); }       \
};
TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MIN__)
TBAG_POP_MACRO(min);
# undef __RESTORE_MIN__
# endif // defined(__RESTORE_MIN__)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif // defined(TBAG_COMP_MSVC)

} // namespace type

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPE_TYPETABLE_HPP__
