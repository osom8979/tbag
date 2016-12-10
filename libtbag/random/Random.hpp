/**
 * @file   Random.hpp
 * @brief  Random utilities.
 * @author zer0
 * @date   2016-04-07
 * @date   2016-12-10 (Move package: libtbag -> libtbag/random)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_RANDOM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_RANDOM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <random>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace random {

template <typename IntegerType>
static IntegerType gen(IntegerType min, IntegerType max)
{
    static_assert(std::is_integral<IntegerType>::value, "IntegerType is not an integer type.");
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<IntegerType> distribution(min, max);
    return distribution(engine);
}

} // namespace random

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_HPP__

