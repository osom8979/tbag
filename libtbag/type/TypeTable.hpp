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
#define TBAG_TYPE_TABLE_MAP(_TBAG_XX) \
    _TBAG_XX(   BOOL,                   bool) \
    _TBAG_XX(   CHAR,                   char) \
    _TBAG_XX(  SCHAR,            signed char) \
    _TBAG_XX(  UCHAR,          unsigned char) \
    _TBAG_XX(  WCHAR,                wchar_t) \
    _TBAG_XX( CHAR16,               char16_t) \
    _TBAG_XX( CHAR32,               char32_t) \
    _TBAG_XX(  SHORT,       signed short int) \
    _TBAG_XX( USHORT,     unsigned short int) \
    _TBAG_XX(    INT,             signed int) \
    _TBAG_XX(   UINT,           unsigned int) \
    _TBAG_XX(   LONG,        signed long int) \
    _TBAG_XX(  ULONG,      unsigned long int) \
    _TBAG_XX(  LLONG,   signed long long int) \
    _TBAG_XX( ULLONG, unsigned long long int) \
    _TBAG_XX(  FLOAT,                  float) \
    _TBAG_XX( DOUBLE,                 double) \
    _TBAG_XX(LDOUBLE,            long double) \
    _TBAG_XX(  POINT,                  void*) \
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
    using Type = T;

    TBAG_CONSTEXPR static bool const is_arithmetic = std::is_arithmetic<Type>::value;

    using Number = typename std::conditional<is_arithmetic, Type, int>::type;

    TBAG_CONSTEXPR static bool const is_specialized = std::numeric_limits<Number>::is_specialized;

    TBAG_CONSTEXPR static bool     isArithmetic() TBAG_NOEXCEPT { return is_arithmetic;  }
    TBAG_CONSTEXPR static bool    isSpecialized() TBAG_NOEXCEPT { return is_specialized; }
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

#define _TBAG_XX(n, t)                                      \
template <> struct TypeInfo<t> : public std::true_type {    \
    using Type = t;                                         \
    TBAG_CONSTEXPR static bool const is_arithmetic = std::is_arithmetic<Type>::value;               \
    using Number = typename std::conditional<is_arithmetic, Type, int>::type;                       \
    TBAG_CONSTEXPR static bool const is_specialized = std::numeric_limits<Number>::is_specialized;  \
    TBAG_CONSTEXPR static bool    isArithmetic() TBAG_NOEXCEPT { return is_arithmetic;  }           \
    TBAG_CONSTEXPR static bool   isSpecialized() TBAG_NOEXCEPT { return is_specialized; }           \
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

