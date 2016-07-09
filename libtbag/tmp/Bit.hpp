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
template <typename BaseType> struct BitFlag;

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

/**
 * @see <https://en.wikipedia.org/wiki/Arithmetic_shift>
 * @see <https://en.wikipedia.org/wiki/Bitwise_operation#Arithmetic_shift>
 */
constexpr bool isArithmeticShift() noexcept
{
    using TestBitFlag = BitFlagImplement<char, false>;
    return ((TestBitFlag::max() >> 1) & TestBitFlag::max());
}

/**
 * @see <https://en.wikipedia.org/wiki/Bitwise_operation#Logical_shift>
 * @see <https://en.wikipedia.org/wiki/Logical_shift>
 */
constexpr bool isLogicalShift() noexcept
{
    return !isArithmeticShift();
}

template <typename BaseType>
struct BitFlag
{
    using Implement = BitFlagImplement<BaseType, std::is_unsigned<BaseType>::value>;

    static constexpr BaseType max() noexcept
    {
        return Implement::max();
    }

    static inline BaseType getRightLogicalShift(BaseType flag) noexcept
    {
        if (isArithmeticShift() && std::is_signed<BaseType>::value && (flag & max())) {
            return (flag >> 1) ^ max();
        }
        return flag >> 1;
    }

    static BaseType findHighBit(BaseType flag) noexcept
    {
        BaseType mask = max();
        while (mask != 0 && !(flag & mask)) {
            mask = getRightLogicalShift(mask);
        }
        return flag & mask;
    }
};

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_BIT_HPP__

