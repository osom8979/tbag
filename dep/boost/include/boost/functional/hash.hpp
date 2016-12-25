
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/functional/hash/hash.hpp>

#include <boost/cstdint.hpp>
#include <functional>

#if defined(_MSC_VER)
#   define BOOST_FUNCTIONAL_HASH_ROTL32(x, r) _rotl(x,r)
#else
#   define BOOST_FUNCTIONAL_HASH_ROTL32(x, r) (x << r) | (x >> (32 - r))
#endif

namespace boost {

    namespace hash_detail {

        template <typename SizeT>
        inline void hash_combine_impl(SizeT& seed, SizeT value)
        {
            seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        inline void hash_combine_impl(boost::uint32_t& h1,
                                      boost::uint32_t k1)
        {
            const uint32_t c1 = 0xcc9e2d51;
            const uint32_t c2 = 0x1b873593;

            k1 *= c1;
            k1 = BOOST_FUNCTIONAL_HASH_ROTL32(k1,15);
            k1 *= c2;

            h1 ^= k1;
            h1 = BOOST_FUNCTIONAL_HASH_ROTL32(h1,13);
            h1 = h1*5+0xe6546b64;
        }

    } // namespace hash_detail

    template <class T>
    inline void hash_combine(std::size_t& seed, T const& v)
    {
        std::hash<T> hasher;
        return boost::hash_detail::hash_combine_impl(seed, hasher(v));
    }

    template <class It>
    inline std::size_t hash_range(It first, It last)
    {
        std::size_t seed = 0;

        for(; first != last; ++first)
        {
            hash_combine(seed, *first);
        }

        return seed;
    }

} // namespace boost

