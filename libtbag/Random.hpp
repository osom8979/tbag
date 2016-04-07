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
#include <libtbag/Noncopyable.hpp>

#include <random>

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
    Random();
    ~Random();

public:
    template <typename Integer>
    static Integer gen(Integer min, Integer max) {
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

