/**
 * @file   Bit.hpp
 * @brief  Bit operators.
 * @author zer0
 * @date   2016-07-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TMP_BIT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TMP_BIT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <cstdint>
#include <numeric>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tmp {

template <typename BaseType, bool IsUnsigned> struct BitFlagImplement;

template <typename BaseType>
struct BitFlagImplement<BaseType, true>
{
    static_assert(std::is_unsigned<BaseType>::value, "BaseType must be a unsigned");

    static constexpr BaseType max() noexcept
    {
        return std::numeric_limits<BaseType>::max() ^ (std::numeric_limits<BaseType>::max() >> 1);
    }
};

template <typename BaseType>
struct BitFlagImplement<BaseType, false>
{
    static_assert(std::is_signed<BaseType>::value, "BaseType must be a signed");

    static constexpr BaseType max() noexcept
    {
        return (std::numeric_limits<BaseType>::max() + 1);
    }
};

template <typename BaseType>
struct BitFlag
{
    static constexpr BaseType max() noexcept
    {
        return BitFlagImplement<BaseType, std::is_unsigned<BaseType>::value>::max();
    }
};

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_BIT_HPP__

