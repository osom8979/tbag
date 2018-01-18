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

#ifndef TBAG_TYPE_TABLE_POINT_MAP
#define TBAG_TYPE_TABLE_POINT_MAP(_TBAG_XX)           \
    _TBAG_XX(   VOIDP,   vp,                   void*) \
    _TBAG_XX(   BOOLP,   bp,                   bool*) \
    _TBAG_XX(   CHARP,   cp,                   char*) \
    _TBAG_XX(  SCHARP,  scp,            signed char*) \
    _TBAG_XX(  UCHARP,  ucp,          unsigned char*) \
    _TBAG_XX(  WCHARP,  wcp,                wchar_t*) \
    _TBAG_XX( CHAR16P, c16p,               char16_t*) \
    _TBAG_XX( CHAR32P, c32p,               char32_t*) \
    _TBAG_XX(  SHORTP,   sp,       signed short int*) \
    _TBAG_XX( USHORTP,  usp,     unsigned short int*) \
    _TBAG_XX(    INTP,   ip,             signed int*) \
    _TBAG_XX(   UINTP,  uip,           unsigned int*) \
    _TBAG_XX(   LONGP,   lp,        signed long int*) \
    _TBAG_XX(  ULONGP,  ulp,      unsigned long int*) \
    _TBAG_XX(  LLONGP,  llp,   signed long long int*) \
    _TBAG_XX( ULLONGP, ullp, unsigned long long int*) \
    _TBAG_XX(  FLOATP,   fp,                  float*) \
    _TBAG_XX( DOUBLEP,   dp,                 double*) \
    _TBAG_XX(LDOUBLEP,  ldp,            long double*) \
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
    TBAG_TYPE_TABLE_POINT_MAP(_TBAG_XX)
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
    using Type = typename std::remove_cv<T>::type;
    TBAG_CONSTEXPR static bool const is_arithmetic = std::is_arithmetic<Type>::value;
    using Number = typename std::conditional<is_arithmetic, Type, int>::type;
    TBAG_CONSTEXPR static bool const is_specialized = std::numeric_limits<Number>::is_specialized;
    TBAG_CONSTEXPR static bool const is_pointer     = std::is_pointer<Type>::value;
    TBAG_CONSTEXPR static bool const is_reference   = std::is_reference<Type>::value;
    TBAG_CONSTEXPR static bool     isArithmetic() TBAG_NOEXCEPT { return is_arithmetic;  }
    TBAG_CONSTEXPR static bool    isSpecialized() TBAG_NOEXCEPT { return is_specialized; }
    TBAG_CONSTEXPR static bool        isPointer() TBAG_NOEXCEPT { return is_pointer;     }
    TBAG_CONSTEXPR static bool      isReference() TBAG_NOEXCEPT { return is_reference;   }
    TBAG_CONSTEXPR static char const *     name() TBAG_NOEXCEPT { return "UNKNOWN";      }
    TBAG_CONSTEXPR static int              size() TBAG_NOEXCEPT { return sizeof(Type);   }
    TBAG_CONSTEXPR static int             index() TBAG_NOEXCEPT { return static_cast<int>(TypeTable::TT_UNKNOWN);      }
    TBAG_CONSTEXPR static TypeTable       table() TBAG_NOEXCEPT { return TypeTable::TT_UNKNOWN;                        }
    TBAG_CONSTEXPR static Number        maximum() TBAG_NOEXCEPT { return std::numeric_limits<Number>::max();           }
    TBAG_CONSTEXPR static Number        minimum() TBAG_NOEXCEPT { return std::numeric_limits<Number>::min();           }
    TBAG_CONSTEXPR static Number         lowest() TBAG_NOEXCEPT { return std::numeric_limits<Number>::lowest();        }
    TBAG_CONSTEXPR static Number        epsilon() TBAG_NOEXCEPT { return std::numeric_limits<Number>::epsilon();       }
    TBAG_CONSTEXPR static Number    round_error() TBAG_NOEXCEPT { return std::numeric_limits<Number>::round_error();   }
    TBAG_CONSTEXPR static Number       infinity() TBAG_NOEXCEPT { return std::numeric_limits<Number>::infinity();      }
    TBAG_CONSTEXPR static Number      quiet_NaN() TBAG_NOEXCEPT { return std::numeric_limits<Number>::quiet_NaN();     }
    TBAG_CONSTEXPR static Number  signaling_NaN() TBAG_NOEXCEPT { return std::numeric_limits<Number>::signaling_NaN(); }
    TBAG_CONSTEXPR static Number     denorm_min() TBAG_NOEXCEPT { return std::numeric_limits<Number>::denorm_min();    }
};

#define _TBAG_XX(n, s, t)                                       \
template <> struct BaseTypeInfo<t> : public std::true_type {    \
    using Type = typename std::remove_cv<t>::type;              \
    TBAG_CONSTEXPR static bool const is_arithmetic = std::is_arithmetic<Type>::value;               \
    using Number = typename std::conditional<is_arithmetic, Type, int>::type;                       \
    TBAG_CONSTEXPR static bool const is_specialized = std::numeric_limits<Type>::is_specialized;    \
    TBAG_CONSTEXPR static bool const is_pointer     = std::is_pointer<Type>::value;                 \
    TBAG_CONSTEXPR static bool const is_reference   = std::is_reference<Type>::value;               \
    TBAG_CONSTEXPR static bool    isArithmetic() TBAG_NOEXCEPT { return is_arithmetic;  }           \
    TBAG_CONSTEXPR static bool   isSpecialized() TBAG_NOEXCEPT { return is_specialized; }           \
    TBAG_CONSTEXPR static bool       isPointer() TBAG_NOEXCEPT { return is_pointer;     }           \
    TBAG_CONSTEXPR static bool     isReference() TBAG_NOEXCEPT { return is_reference;   }           \
    TBAG_CONSTEXPR static char const *    name() TBAG_NOEXCEPT { return #n;             }           \
    TBAG_CONSTEXPR static int             size() TBAG_NOEXCEPT { return sizeof(Type);   }           \
    TBAG_CONSTEXPR static int            index() TBAG_NOEXCEPT { return static_cast<int>(TypeTable::TT_##n);        } \
    TBAG_CONSTEXPR static TypeTable      table() TBAG_NOEXCEPT { return TypeTable::TT_##n;                          } \
    TBAG_CONSTEXPR static Type         maximum() TBAG_NOEXCEPT { return std::numeric_limits<Type>::max();           } \
    TBAG_CONSTEXPR static Type         minimum() TBAG_NOEXCEPT { return std::numeric_limits<Type>::min();           } \
    TBAG_CONSTEXPR static Type          lowest() TBAG_NOEXCEPT { return std::numeric_limits<Type>::lowest();        } \
    TBAG_CONSTEXPR static Type         epsilon() TBAG_NOEXCEPT { return std::numeric_limits<Type>::epsilon();       } \
    TBAG_CONSTEXPR static Type     round_error() TBAG_NOEXCEPT { return std::numeric_limits<Type>::round_error();   } \
    TBAG_CONSTEXPR static Type        infinity() TBAG_NOEXCEPT { return std::numeric_limits<Type>::infinity();      } \
    TBAG_CONSTEXPR static Type       quiet_NaN() TBAG_NOEXCEPT { return std::numeric_limits<Type>::quiet_NaN();     } \
    TBAG_CONSTEXPR static Type   signaling_NaN() TBAG_NOEXCEPT { return std::numeric_limits<Type>::signaling_NaN(); } \
    TBAG_CONSTEXPR static Type      denorm_min() TBAG_NOEXCEPT { return std::numeric_limits<Type>::denorm_min();    } \
};
TBAG_TYPE_TABLE_MAP(_TBAG_XX)
TBAG_TYPE_TABLE_POINT_MAP(_TBAG_XX)
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

template <typename T>
struct TypeInfo : public BaseTypeInfo<typename std::remove_all_extents<typename std::remove_cv<T>::type>::type>
{ /* EMPTY. */ };

template <typename T>
inline TypeTable getTypeTable() TBAG_NOEXCEPT
{
    return TypeInfo<T>::table();
}

inline char const * getTypeName(TypeTable t) TBAG_NOEXCEPT
{
    switch (t) {
#define _TBAG_XX(name, symbol, type) case TypeTable::TT_##name: return #name;
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
    TBAG_TYPE_TABLE_POINT_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default: return "UNKNOWN";
    }
}

} // namespace type

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPE_TYPETABLE_HPP__

