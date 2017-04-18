/*  Copyright (C) 2013-2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_CORE_F_DIV_H
#define LIBSIMDPP_SIMDPP_CORE_F_DIV_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/detail/insn/f_div.h>
#include <simdpp/core/detail/scalar_arg_impl.h>

namespace simdpp {
namespace SIMDPP_ARCH_NAMESPACE {

/** Divides the values of two vectors.

    @code
    r0 = a0 / b0
    ...
    rN = aN / bN
    @endcode

    @icost{NEON, 6}
    @icost{ALTIVEC, 10}

    @par 256-bit version:
    @icost{SSE2-SSE4.1, 2}
    @icost{NEON, 12}
    @icost{ALTIVEC, 19}
*/
template<unsigned N, class E1, class E2> SIMDPP_INL
float32<N,expr_empty> div(const float32<N,E1>& a, const float32<N,E2>& b)
{
    return detail::insn::i_div(a.eval(), b.eval());
}

SIMDPP_SCALAR_ARG_IMPL_VEC(div, float32, float32)

/** Divides the values of two vectors

    @code
    r0 = a0 / b0
    ...
    rN = aN / bN
    @endcode

    @par 128-bit version:
    @novec{NEON, ALTIVEC}

    @par 256-bit version:
    @icost{SSE2-SSE4.1, 2}
    @novec{NEON, ALTIVEC}
*/
template<unsigned N, class E1, class E2> SIMDPP_INL
float64<N,expr_empty> div(const float64<N,E1>& a, const float64<N,E2>& b)
{
    return detail::insn::i_div(a.eval(), b.eval());
}

SIMDPP_SCALAR_ARG_IMPL_VEC(div, float64, float64)

} // namespace SIMDPP_ARCH_NAMESPACE
} // namespace simdpp

#endif

