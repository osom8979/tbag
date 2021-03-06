/**
 * @file   Unit.hpp
 * @brief  Unit package.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tmp/Power.hpp>

#include <cstdint>

#define TBAG_CHAR_BIT    8  // Number of bits in a char
#define TBAG_MB_LEN_MAX  6  // Allow 31 bit UTF2

#define TBAG_SCHAR_MAX   (+127)  // min value for a signed char
#define TBAG_SCHAR_MIN   (-128)  // max value for a signed char

#define TBAG_SCHAR_MAX   (+127)  // min value for a signed char
#define TBAG_SCHAR_MIN   (-128)  // max value for a signed char

#define TBAG_UCHAR_MAX   (+255)  // max value for an unsigned char
#define TBAG_CHAR_MAX    (+127)  // max value for a char
#define TBAG_CHAR_MIN    (-128)  // min value for a char

#define TBAG_USHRT_MAX   (+65535)  // max value for an unsigned short */
#define TBAG_SHRT_MAX    (+32767)  // max value for a short */
#define TBAG_SHRT_MIN    (-32768)  // min value for a short */

#define TBAG_UINT_MAX    ( 0xffffffff  )  // max value for an unsigned int
#define TBAG_INT_MAX     (+2147483647  )  // max value for an int
#define TBAG_INT_MIN     (-2147483647-1)  // min value for an int

#if defined(__LP64__)
# define TBAG_ULONG_MAX  ( 0xffffffffffffffffUL )  // max unsigned long
# define TBAG_LONG_MAX   ( 0x7fffffffffffffffL  )  // max signed long
# define TBAG_LONG_MIN   (-0x7fffffffffffffffL-1)  // min signed long
#else
# define TBAG_ULONG_MAX  ( 0xffffffffUL )  // max unsigned long
# define TBAG_LONG_MAX   (+2147483647L  )  // max signed long
# define TBAG_LONG_MIN   (-2147483647L-1)  // min signed long
#endif

#define TBAG_ULLONG_MAX  ( 0xffffffffffffffffULL )  // max unsigned long long
#define TBAG_LLONG_MAX   ( 0x7fffffffffffffffLL  )  // max signed long long
#define TBAG_LLONG_MIN   (-0x7fffffffffffffffLL-1)  // min signed long long

#if !defined(_ANSI_SOURCE)
# if defined(__LP64__)
#  define TBAG_LONG_BIT    64
# else // !defined(__LP64__)
#  define TBAG_LONG_BIT    32
# endif // defined(__LP64__)
# define  TBAG_SSIZE_MAX  LONG_MAX // max value for a ssize_t
# define  TBAG_WORD_BIT    32
# if (!defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE)) || defined(_DARWIN_C_SOURCE)
#  define TBAG_SIZE_T_MAX  TBAG_ULONG_MAX   // max value for a size_t
#  define TBAG_UQUAD_MAX   TBAG_ULLONG_MAX
#  define TBAG_QUAD_MAX    TBAG_LLONG_MAX
#  define TBAG_QUAD_MIN    TBAG_LLONG_MIN
# endif // (!defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE)) || defined(_DARWIN_C_SOURCE)
#endif // !defined(_ANSI_SOURCE)

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

TBAG_CONSTEXPR char const       BYTE_LOWER_CHAR = 'b';
TBAG_CONSTEXPR char const  KILO_BYTE_LOWER_CHAR = 'k';
TBAG_CONSTEXPR char const  MEGA_BYTE_LOWER_CHAR = 'm';
TBAG_CONSTEXPR char const  GIGA_BYTE_LOWER_CHAR = 'g';
TBAG_CONSTEXPR char const  TERA_BYTE_LOWER_CHAR = 't';
TBAG_CONSTEXPR char const  PETA_BYTE_LOWER_CHAR = 'p';
TBAG_CONSTEXPR char const   EXA_BYTE_LOWER_CHAR = 'e';
TBAG_CONSTEXPR char const ZETTA_BYTE_LOWER_CHAR = 'z';
TBAG_CONSTEXPR char const YOTTA_BYTE_LOWER_CHAR = 'y';

TBAG_CONSTEXPR char const * const       BYTE_LONG_NAME = "Byte";
TBAG_CONSTEXPR char const * const  KILO_BYTE_LONG_NAME = "KiloByte";
TBAG_CONSTEXPR char const * const  MEGA_BYTE_LONG_NAME = "MegaByte";
TBAG_CONSTEXPR char const * const  GIGA_BYTE_LONG_NAME = "GigaByte";
TBAG_CONSTEXPR char const * const  TERA_BYTE_LONG_NAME = "TeraByte";
TBAG_CONSTEXPR char const * const  PETA_BYTE_LONG_NAME = "PetaByte";
TBAG_CONSTEXPR char const * const   EXA_BYTE_LONG_NAME = "ExaByte";
TBAG_CONSTEXPR char const * const ZETTA_BYTE_LONG_NAME = "ZettaByte";
TBAG_CONSTEXPR char const * const YOTTA_BYTE_LONG_NAME = "YottaByte";

TBAG_CONSTEXPR char const * const       BYTE_SHORT_NAME = "B";
TBAG_CONSTEXPR char const * const  KILO_BYTE_SHORT_NAME = "KB";
TBAG_CONSTEXPR char const * const  MEGA_BYTE_SHORT_NAME = "MB";
TBAG_CONSTEXPR char const * const  GIGA_BYTE_SHORT_NAME = "GB";
TBAG_CONSTEXPR char const * const  TERA_BYTE_SHORT_NAME = "TB";
TBAG_CONSTEXPR char const * const  PETA_BYTE_SHORT_NAME = "PB";
TBAG_CONSTEXPR char const * const   EXA_BYTE_SHORT_NAME = "EB";
TBAG_CONSTEXPR char const * const ZETTA_BYTE_SHORT_NAME = "ZB";
TBAG_CONSTEXPR char const * const YOTTA_BYTE_SHORT_NAME = "YB";

//typedef ratio<1LL, 1000000000000000000LL> atto;
//typedef ratio<1LL,    1000000000000000LL> femto;
//typedef ratio<1LL,       1000000000000LL> pico;
//typedef ratio<1LL,          1000000000LL> nano;
//typedef ratio<1LL,             1000000LL> micro;
//typedef ratio<1LL,                1000LL> milli;
//typedef ratio<1LL,                 100LL> centi;
//typedef ratio<1LL,                  10LL> deci;
//typedef ratio<                 10LL, 1LL> deca;
//typedef ratio<                100LL, 1LL> hecto;
//typedef ratio<               1000LL, 1LL> kilo;
//typedef ratio<            1000000LL, 1LL> mega;
//typedef ratio<         1000000000LL, 1LL> giga;
//typedef ratio<      1000000000000LL, 1LL> tera;
//typedef ratio<   1000000000000000LL, 1LL> peta;
//typedef ratio<1000000000000000000LL, 1LL> exa;

/**
 * @defgroup __DOXYGEN_GROUP__BYTE__ Byte.
 * @brief Unit of digital information.
 * @{
 */

TBAG_CONSTEXPR uint64_t const BYTE_UNIT = 1024;

TBAG_CONSTEXPR uint64_t const  KILO_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 1>::value;
TBAG_CONSTEXPR uint64_t const  MEGA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 2>::value;
TBAG_CONSTEXPR uint64_t const  GIGA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 3>::value;
TBAG_CONSTEXPR uint64_t const  TERA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 4>::value;
TBAG_CONSTEXPR uint64_t const  PETA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 5>::value;
TBAG_CONSTEXPR uint64_t const   EXA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 6>::value;
TBAG_CONSTEXPR uint64_t const ZETTA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 7>::value;
TBAG_CONSTEXPR uint64_t const YOTTA_BYTE_TO_BYTE = tmp::Power<BYTE_UNIT, 8>::value;

/**
 * @}
 */

TBAG_CONSTEXPR uint64_t const SECONDS_TO_MILLISECONDS = 1000;
TBAG_CONSTEXPR uint64_t const SECONDS_TO_MICROSECONDS = 1000 * SECONDS_TO_MILLISECONDS;
TBAG_CONSTEXPR uint64_t const SECONDS_TO_NANOSECONDS  = 1000 * SECONDS_TO_MICROSECONDS;

TBAG_CONSTEXPR int const SECONDS_TO_MILLISECONDS_INTEGER = (int)SECONDS_TO_MILLISECONDS;
TBAG_CONSTEXPR int const SECONDS_TO_MICROSECONDS_INTEGER = (int)SECONDS_TO_MICROSECONDS;
TBAG_CONSTEXPR int const SECONDS_TO_NANOSECONDS_INTEGER  = (int)SECONDS_TO_NANOSECONDS;

TBAG_CONSTEXPR uint64_t const MILLISECONDS_TO_MICROSECONDS = 1000;
TBAG_CONSTEXPR uint64_t const MILLISECONDS_TO_NANOSECONDS  = 1000 * MILLISECONDS_TO_MICROSECONDS;

TBAG_CONSTEXPR uint64_t const MILLISECONDS_TO_MICROSECONDS_INTEGER = (int)MILLISECONDS_TO_MICROSECONDS;
TBAG_CONSTEXPR uint64_t const MILLISECONDS_TO_NANOSECONDS_INTEGER  = (int)MILLISECONDS_TO_NANOSECONDS;

TBAG_CONSTEXPR uint64_t const DAY_TO_HOURS      = 24;
TBAG_CONSTEXPR uint64_t const DAY_TO_MINUTES    = 24*60;
TBAG_CONSTEXPR uint64_t const DAY_TO_SECONDS    = 24*60*60;
TBAG_CONSTEXPR uint64_t const HOUR_TO_MINUTES   = 60;
TBAG_CONSTEXPR uint64_t const HOUR_TO_SECONDS   = 60*60;
TBAG_CONSTEXPR uint64_t const MINUTE_TO_SECONDS = 60;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UNIT_HPP__

