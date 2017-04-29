/**
 * @file   HashCombine.hpp
 * @brief  HashCombine class prototype.
 * @author zer0
 * @date   2017-04-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_HASHCOMBINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_HASHCOMBINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <functional>

// Rotates bits.
#if defined(TBAG_PLATFORM_WINDOWS)
# include <stdlib.h>
# define TBAG_HASH_ROTL32(x, r) _rotl(x,r)
#else
# define TBAG_HASH_ROTL32(x, r) (x << r) | (x >> (32 - r))
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace functional {

// ---------------
namespace __impl {
// ---------------

template <typename SizeType>
inline void hashCombineImpl(SizeType & seed, SizeType value)
{
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

inline void hashCombineImpl(uint32_t & h1, uint32_t k1)
{
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    k1 *= c1;
    k1  = TBAG_HASH_ROTL32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1  = TBAG_HASH_ROTL32(h1, 13);
    h1  = h1 * 5 + 0xe6546b64;
}

// ------------------
} // namespace __impl
// ------------------

template <class T>
inline void hashCombine(std::size_t & seed, T const & v)
{
    std::hash<T> hasher;
    return __impl::hashCombineImpl(seed, hasher(v));
}

template <class Itr>
inline std::size_t hashRange(Itr first, Itr last)
{
    std::size_t seed = 0;
    for(; first != last; ++first) {
        hashCombine(seed, *first);
    }
    return seed;
}

} // namespace functional

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_HASHCOMBINE_HPP__

