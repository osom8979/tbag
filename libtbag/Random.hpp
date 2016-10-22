/**
 * @file   Random.hpp
 * @brief  Random class prototype.
 * @author zer0
 * @date   2016-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <random>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Random class prototype.
 *
 * @author zer0
 * @date   2016-04-07
 */
class Random : public Noncopyable
{
public:
    constexpr Random() TBAG_NOEXCEPT = default;
    ~Random() TBAG_NOEXCEPT = default;

public:
    template <typename Integer>
    static Integer gen(Integer min, Integer max) {
        static_assert(std::is_integral<Integer>::value, "This is not an integer type.");
        std::random_device device;
        std::mt19937 engine(device());
        std::uniform_int_distribution<Integer> distribution(min, max);
        return distribution(engine);
    }
};

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RANDOM_HPP__

