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
#include <limits>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace type {

// It's by design, C++ standard says 'char', 'signed char' and 'unsigned char' are different types.
// see also: http://cpp14.centaur.ath.cx/basic.fundamental.html
#ifndef TBAG_TYPE_TABLE_MAP
#define TBAG_TYPE_TABLE_MAP(_TBAG_XX)              \
    _TBAG_XX(   BOOL,   b,                   bool) \
    _TBAG_XX(   CHAR,   c,                   char) \
    _TBAG_XX(  SCHAR,  sc,            signed char) \
    _TBAG_XX(  UCHAR,  uc,          unsigned char) \
    _TBAG_XX(  WCHAR,  wc,                wchar_t) \
    _TBAG_XX( CHAR16, c16,               char16_t) \
    _TBAG_XX( CHAR32, c32,               char32_t) \
    _TBAG_XX(  SHORT,   s,       signed short int) \
    _TBAG_XX( USHORT,  us,     unsigned short int) \
    _TBAG_XX(    INT,   i,             signed int) \
    _TBAG_XX(   UINT,  ui,           unsigned int) \
    _TBAG_XX(   LONG,   l,        signed long int) \
    _TBAG_XX(  ULONG,  ul,      unsigned long int) \
    _TBAG_XX(  LLONG,  ll,   signed long long int) \
    _TBAG_XX( ULLONG, ull, unsigned long long int) \
    _TBAG_XX(  FLOAT,   f,                  float) \
    _TBAG_XX( DOUBLE,   d,                 double) \
    _TBAG_XX(LDOUBLE,  ld,            long double) \
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
#define _TBAG_XX(name, symbol, type) TT_##name,
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
struct BaseTypeInfo : public std::false_type
{
    TBAG_CONSTEXPR static char const * name() TBAG_NOEXCEPT { return "UNKNOWN";             }
    TBAG_CONSTEXPR static TypeTable   table() TBAG_NOEXCEPT { return TypeTable::TT_UNKNOWN; }
};

#define _TBAG_XX(n, s, t)                                                                   \
template <> struct BaseTypeInfo<t> : public std::true_type {                                \
    TBAG_CONSTEXPR static char const * name() TBAG_NOEXCEPT { return #n;                }   \
    TBAG_CONSTEXPR static TypeTable   table() TBAG_NOEXCEPT { return TypeTable::TT_##n; }   \
};
TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX

template <typename T>
struct TypeInfo : public BaseTypeInfo<typename std::remove_cv<T>::type>
{
    using Base = BaseTypeInfo<typename std::remove_cv<T>::type>;
    using Type = typename std::remove_cv<T>::type;

    TBAG_CONSTEXPR static bool const is_arithmetic = std::is_arithmetic<Type>::value;
    using Number = typename std::conditional<is_arithmetic, Type, int>::type;

    TBAG_CONSTEXPR static bool const is_pointer        = std::is_pointer<Type>::value;
    TBAG_CONSTEXPR static bool const is_reference      = std::is_reference<Type>::value;
    TBAG_CONSTEXPR static bool const is_floating_point = std::is_floating_point<Type>::value;
    TBAG_CONSTEXPR static bool const is_integral       = std::is_integral<Type>::value;
    TBAG_CONSTEXPR static bool const is_unsigned       = std::is_unsigned<Type>::value;
    TBAG_CONSTEXPR static bool const is_signed         = std::is_signed<Type>::value;

    TBAG_CONSTEXPR static int size() TBAG_NOEXCEPT { return sizeof(Type); }

    TBAG_CONSTEXPR static bool  isArithmetic() TBAG_NOEXCEPT { return is_arithmetic;     }
    TBAG_CONSTEXPR static bool isSpecialized() TBAG_NOEXCEPT { return Base::value;       }
    TBAG_CONSTEXPR static bool     isPointer() TBAG_NOEXCEPT { return is_pointer;        }
    TBAG_CONSTEXPR static bool   isReference() TBAG_NOEXCEPT { return is_reference;      }
    TBAG_CONSTEXPR static bool    isFloating() TBAG_NOEXCEPT { return is_floating_point; }
    TBAG_CONSTEXPR static bool    isIntegral() TBAG_NOEXCEPT { return is_integral;       }
    TBAG_CONSTEXPR static bool    isUnsigned() TBAG_NOEXCEPT { return is_unsigned;       }
    TBAG_CONSTEXPR static bool      isSigned() TBAG_NOEXCEPT { return is_signed;         }

    using Limits = std::numeric_limits<Number>;

    /**
     * returns the largest finite value of the given type
     *
     * @remarks
     *  Equivalent to CHAR_MAX, SCHAR_MAX, UCHAR_MAX, SHRT_MAX,
     *  USHRT_MAX, INT_MAX, UINT_MAX, LONG_MAX, ULONG_MAX, LLONG_MAX,
     *  ULLONG_MAX, UINT_LEAST16_MAX, UINT_LEAST32_MAX, FLT_MAX,
     *  DBL_MAX or LDBL_MAX, depending on type.
     */
    TBAG_CONSTEXPR static Number maximum() TBAG_NOEXCEPT { return Limits::max(); }

    /**
     * returns the smallest finite value of the given type
     *
     * @remarks
     *  - For floating types with denormalization (variable number of exponent bits): @n
     *    minimum positive normalized value.
     *  - Equivalent to CHAR_MIN, SCHAR_MIN, SHRT_MIN, INT_MIN, LONG_MIN, @n
     *    LLONG_MIN, FLT_MIN, DBL_MIN, LDBL_MIN or 0, depending on type.
     */
    TBAG_CONSTEXPR static Number minimum() TBAG_NOEXCEPT { return Limits::min(); }

    /**
     * returns the lowest finite value of the given type
     *
     * @remarks
     *  - For integral types: the same as min().
     *  - For floating-point types: implementation-dependent; generally, the negative of max().
     */
    TBAG_CONSTEXPR static Number lowest() TBAG_NOEXCEPT { return Limits::lowest(); }

    /** returns the difference between 1.0 and the next representable value of the given floating-point type */
    TBAG_CONSTEXPR static Number epsilon() TBAG_NOEXCEPT { return Limits::epsilon(); }
    /** returns the maximum rounding error of the given floating-point type */
    TBAG_CONSTEXPR static Number round_error() TBAG_NOEXCEPT { return Limits::round_error(); }
    /** returns the positive infinity value of the given floating-point type */
    TBAG_CONSTEXPR static Number infinity() TBAG_NOEXCEPT { return Limits::infinity(); }
    /** returns a quiet NaN value of the given floating-point type */
    TBAG_CONSTEXPR static Number quiet_NaN() TBAG_NOEXCEPT { return Limits::quiet_NaN(); }
    /** returns a signaling NaN value of the given floating-point type */
    TBAG_CONSTEXPR static Number signaling_NaN() TBAG_NOEXCEPT { return Limits::signaling_NaN(); }
    /** returns the smallest positive subnormal value of the given floating-point type */
    TBAG_CONSTEXPR static Number denorm_min() TBAG_NOEXCEPT { return Limits::denorm_min(); }

    TBAG_CONSTEXPR static int index() TBAG_NOEXCEPT { return static_cast<int>(Base::table()); }
};

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

template <typename T>
TBAG_CONSTEXPR inline TypeTable getTypeTable() TBAG_NOEXCEPT
{
    return TypeInfo<T>::table();
}

#ifndef _STATIC_ASSERT_CHECK_PRIMARY_TYPE
#define _STATIC_ASSERT_CHECK_PRIMARY_TYPE(type_name) \
    static_assert(BaseTypeInfo<type_name>::value, "The " #type_name " type is not a primary type.")
#endif

#if 0
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(std::size_t   );
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(std::ptrdiff_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(std::intptr_t );
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(std::uintptr_t);
#endif

_STATIC_ASSERT_CHECK_PRIMARY_TYPE(  int8_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE( uint8_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE( int16_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(uint16_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE( int32_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(uint32_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE( int64_t);
_STATIC_ASSERT_CHECK_PRIMARY_TYPE(uint64_t);

TBAG_CONSTEXPR TypeTable const   TT_INT8 = getTypeTable<  int8_t>();
TBAG_CONSTEXPR TypeTable const  TT_UINT8 = getTypeTable< uint8_t>();
TBAG_CONSTEXPR TypeTable const  TT_INT16 = getTypeTable< int16_t>();
TBAG_CONSTEXPR TypeTable const TT_UINT16 = getTypeTable<uint16_t>();
TBAG_CONSTEXPR TypeTable const  TT_INT32 = getTypeTable< int32_t>();
TBAG_CONSTEXPR TypeTable const TT_UINT32 = getTypeTable<uint32_t>();
TBAG_CONSTEXPR TypeTable const  TT_INT64 = getTypeTable< int64_t>();
TBAG_CONSTEXPR TypeTable const TT_UINT64 = getTypeTable<uint64_t>();

inline char const * getTypeName(TypeTable t) TBAG_NOEXCEPT
{
    switch (t) {
#define _TBAG_XX(name, symbol, type) case TypeTable::TT_##name: return #name;
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default: return "UNKNOWN";
    }
}

inline std::size_t getTypeSize(TypeTable t) TBAG_NOEXCEPT
{
    switch (t) {
#define _TBAG_XX(name, symbol, type) case TypeTable::TT_##name: return sizeof(type);
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default: return 0;
    }
}

} // namespace type

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPE_TYPETABLE_HPP__

