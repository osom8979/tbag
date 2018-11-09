/**
 * @file   Power.hpp
 * @brief  Power class prototype.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TMP_POWER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TMP_POWER_HPP__

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

/**
 * Returning base raised to power.
 */
template <uint64_t _Base, uint64_t _Power>
struct Power
{
    TBAG_CONSTEXPR static uint64_t const value = _Base * Power<_Base, _Power -1>::value;
};

/**
 * Returning base.
 */
template <uint64_t _Base>
struct Power<_Base, 1>
{
    TBAG_CONSTEXPR static uint64_t const value = _Base;
};

/**
 * Returning 1.
 */
template <uint64_t _Base>
struct Power<_Base, 0>
{
    static_assert(_Base != 0, "0 Power 0 is NaN.");
    TBAG_CONSTEXPR static uint64_t const value = 1;
};

/**
 * Positive integer power of 2.
 */
template <uint64_t _Value>
struct is_power_of_2
{
    TBAG_CONSTEXPR static bool const value = ((_Value & (_Value - 1)) == 0 && _Value/*(Ignore 0 number)*/);
};

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_POWER_HPP__

