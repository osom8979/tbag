/**
 * @file   NumberOfDigits.hpp
 * @brief  NumberOfDigits class prototype.
 * @author zer0
 * @date   2018-05-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TMP_NUMBEROFDIGITS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TMP_NUMBEROFDIGITS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tmp {

template <std::size_t _Radix, std::size_t _Value>
struct NumberOfDigits
{
    TBAG_CONSTEXPR static std::size_t const value = 1 + NumberOfDigits<_Radix, _Value / _Radix>::value;
};

template <std::size_t _Radix>
struct NumberOfDigits<_Radix, 0>
{
    TBAG_CONSTEXPR static std::size_t const value = 0;
};

template <std::size_t _Value>
struct DecimalNumberOfDigits
{
    TBAG_CONSTEXPR static std::size_t const value = NumberOfDigits<10, _Value>::value;
};

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_NUMBEROFDIGITS_HPP__

