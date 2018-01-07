/*  Copyright (C) 2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_CORE_MAKE_UINT_H
#define LIBSIMDPP_SIMDPP_CORE_MAKE_UINT_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/detail/insn/make_const.h>

namespace simdpp {
namespace SIMDPP_ARCH_NAMESPACE {

/** Creates a vector from unsigned integer values known at compile-time.
    The result of this function may be assigned or converted to a vector of any
    type: standard conversions are used to convert the arguments. All
    conversions and other overhead is performed at compile-time thus even if the
    minimal optimization level is selected, the function results in a simple
    load from memory.

    The function is not guaranteed to have adequate performance if the
    arguments are not known at compile-time.

    If the vector has fewer elements than the number of the parameters this
    function accepts then the extra values are discarded.

    Note that per C++ rules negative values are sign-extended to fill entire
    element before being converted to unsigned type thus e.g. it's safe to use
    -1 to fill element with ones.

    @par 1 parameter version
    @code
        | 0  1  2  3  ... n  |
    r = [ v0 v0 v0 v0 ... v0 ]
    @endcode

    @par 2 parameters version
    @code
        | 0  1  2  3  ... n  |
    r = [ v0 v1 v0 v1 ... v1 ]
    @endcode

    @par 4 parameters version
    @code
        | 0  1  2  3  ... n  |
    r = [ v0 v1 v2 v3 ... v3 ]
    @endcode

    @par 8 parameters version
    @code
        | 0  1  ..  7  8  ... n  |
    r = [ v0 v1 .. v7 v0  ... v7 ]
    @endcode
*/
// FIXME: return empty expr
SIMDPP_INL expr_vec_make_const<uint64_t,1> make_uint(uint64_t v0)
{
    expr_vec_make_const<uint64_t,1> a;
    a.a[0] = v0;
    return a;
}

static SIMDPP_INL
expr_vec_make_const<uint64_t,2> make_uint(uint64_t v0, uint64_t v1)
{
    expr_vec_make_const<uint64_t,2> a;
    a.a[0] = v0;  a.a[1] = v1;
    return a;
}

static SIMDPP_INL
expr_vec_make_const<uint64_t,4>
    make_uint(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3)
{
    expr_vec_make_const<uint64_t,4> a;
    a.a[0] = v0;  a.a[1] = v1;  a.a[2] = v2;  a.a[3] = v3;
    return a;
}

static SIMDPP_INL
expr_vec_make_const<uint64_t,8>
    make_uint(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3,
              uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7)
{
    expr_vec_make_const<uint64_t,8> a;
    a.a[0] = v0;  a.a[1] = v1;  a.a[2] = v2;  a.a[3] = v3;
    a.a[4] = v4;  a.a[5] = v5;  a.a[6] = v6;  a.a[7] = v7;
    return a;
}

static SIMDPP_INL
expr_vec_make_const<uint64_t,16>
    make_uint(uint64_t v0,  uint64_t v1,  uint64_t v2,  uint64_t v3,
              uint64_t v4,  uint64_t v5,  uint64_t v6,  uint64_t v7,
              uint64_t v8,  uint64_t v9,  uint64_t v10, uint64_t v11,
              uint64_t v12, uint64_t v13, uint64_t v14, uint64_t v15)
{
    expr_vec_make_const<uint64_t,16> a;
    a.a[0] = v0;    a.a[1] = v1;    a.a[2] = v2;    a.a[3] = v3;
    a.a[4] = v4;    a.a[5] = v5;    a.a[6] = v6;    a.a[7] = v7;
    a.a[8] = v8;    a.a[9] = v9;    a.a[10] = v10;  a.a[11] = v11;
    a.a[12] = v12;  a.a[13] = v13;  a.a[14] = v14;  a.a[15] = v15;
    return a;
}

template<class V> SIMDPP_INL
V make_uint(uint64_t v0)
{
    static_assert(is_vector<V>::value && !is_mask<V>::value,
                  "V must be a non-mask vector");
    expr_vec_make_const<uint64_t,1> a;
    a.a[0] = v0;
    return detail::insn::i_make_const_any<V>(a);
}

template<class V> SIMDPP_INL
V make_uint(uint64_t v0, uint64_t v1)
{
    static_assert(is_vector<V>::value && !is_mask<V>::value,
                  "V must be a non-mask vector");
    expr_vec_make_const<uint64_t,2> a;
    a.a[0] = v0;  a.a[1] = v1;
    return detail::insn::i_make_const_any<V>(a);
}

template<class V> SIMDPP_INL
V make_uint(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3)
{
    static_assert(is_vector<V>::value && !is_mask<V>::value,
                  "V must be a non-mask vector");
    expr_vec_make_const<uint64_t,4> a;
    a.a[0] = v0;  a.a[1] = v1;  a.a[2] = v2;  a.a[3] = v3;
    return detail::insn::i_make_const_any<V>(a);
}

template<class V> SIMDPP_INL
V make_uint(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3,
            uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7)
{
    static_assert(is_vector<V>::value && !is_mask<V>::value,
                  "V must be a non-mask vector");
    expr_vec_make_const<uint64_t,8> a;
    a.a[0] = v0;  a.a[1] = v1;  a.a[2] = v2;  a.a[3] = v3;
    a.a[4] = v4;  a.a[5] = v5;  a.a[6] = v6;  a.a[7] = v7;
    return detail::insn::i_make_const_any<V>(a);
}

template<class V> SIMDPP_INL
V make_uint(uint64_t v0,  uint64_t v1,  uint64_t v2,  uint64_t v3,
            uint64_t v4,  uint64_t v5,  uint64_t v6,  uint64_t v7,
            uint64_t v8,  uint64_t v9,  uint64_t v10, uint64_t v11,
            uint64_t v12, uint64_t v13, uint64_t v14, uint64_t v15)
{
    static_assert(is_vector<V>::value && !is_mask<V>::value,
                  "V must be a non-mask vector");
    expr_vec_make_const<uint64_t,16> a;
    a.a[0] = v0;    a.a[1] = v1;    a.a[2] = v2;    a.a[3] = v3;
    a.a[4] = v4;    a.a[5] = v5;    a.a[6] = v6;    a.a[7] = v7;
    a.a[8] = v8;    a.a[9] = v9;    a.a[10] = v10;  a.a[11] = v11;
    a.a[12] = v12;  a.a[13] = v13;  a.a[14] = v14;  a.a[15] = v15;
    return detail::insn::i_make_const_any<V>(a);
}

/// Creates a vector initialized to zero
SIMDPP_INL expr_vec_make_const<uint64_t,1> make_zero()
{
    return make_uint(0);
}

template<class V> SIMDPP_INL
V make_zero()
{
    return make_uint<V>(0);
}

/// Creates a vector initialized to ones
SIMDPP_INL expr_vec_make_ones make_ones()
{
    return expr_vec_make_ones();
}

template<class V> SIMDPP_INL
V make_ones()
{
    return (V) make_ones();
}

} // namespace SIMDPP_ARCH_NAMESPACE
} // namespace simdpp

#endif

