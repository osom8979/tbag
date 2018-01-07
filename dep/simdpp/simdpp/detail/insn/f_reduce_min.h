/*  Copyright (C) 2016  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_DETAIL_INSN_F_REDUCE_MIN_H
#define LIBSIMDPP_SIMDPP_DETAIL_INSN_F_REDUCE_MIN_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/core/f_min.h>
#include <simdpp/core/extract.h>
#include <simdpp/core/permute2.h>
#include <simdpp/detail/extract128.h>
#include <simdpp/detail/workarounds.h>

namespace simdpp {
namespace SIMDPP_ARCH_NAMESPACE {
namespace detail {
namespace insn {


static SIMDPP_INL
float i_reduce_min(const float32x4& a)
{
#if SIMDPP_USE_NULL || SIMDPP_USE_NEON_NO_FLT_SP
    float r = a.el(0);
    for (unsigned i = 1; i < a.length; i++) {
        r = r < a.el(i) ? r : a.el(i); // TODO nan
    }
    return r;
#elif SIMDPP_USE_SSE2
    float32x4 b = _mm_movehl_ps(a.native(), a.native());
    b = min(a, b);
    b = min(b, permute2<1,0>(b));
    return _mm_cvtss_f32(b.native());
#elif SIMDPP_USE_NEON64
    return vminnmvq_f32(a.native());
#elif SIMDPP_USE_NEON_FLT_SP
    float32x2_t a2 = vpmin_f32(vget_low_f32(a.native()), vget_high_f32(a.native()));
    a2 = vpmin_f32(a2, a2);
    return vget_lane_f32(a2, 0);
#elif SIMDPP_USE_ALTIVEC || SIMDPP_USE_MSA
    float32x4 b = min(a, move4_l<1>(a));
    b = min(b, move4_l<2>(b));
    return extract<0>(b);
#endif
}

#if SIMDPP_USE_AVX
static SIMDPP_INL
float i_reduce_min(const float32x8& a)
{
    float32x4 ah = detail::extract128<1>(a);
    float32x4 al = detail::extract128<0>(a);
    al = min(al, ah);
    return i_reduce_min(al);
}
#endif

#if SIMDPP_USE_AVX512F
static SIMDPP_INL
float i_reduce_min(const float32<16>& a)
{
    return i_reduce_min(min(extract256<0>(a), extract256<1>(a)));
}
#endif

template<unsigned N>
SIMDPP_INL float i_reduce_min(const float32<N>& a)
{
    float32v r = a.vec(0);
    for (unsigned i = 1; i < a.vec_length; ++i)
        r = min(r, a.vec(i));
    return i_reduce_min(r);
}

// -----------------------------------------------------------------------------

static SIMDPP_INL
double i_reduce_min(const float64x2& a)
{
#if SIMDPP_USE_SSE2
    float64x2 b = min(a, permute2<1,1>(a));
    return _mm_cvtsd_f64(b.native());
#elif SIMDPP_USE_NEON64
    return vminnmvq_f64(a.native());
#elif SIMDPP_USE_VSX_206 || SIMDPP_USE_MSA
    float64x2 b = min(a, permute2<1,1>(a));
    return extract<0>(b);
#elif SIMDPP_USE_NULL || SIMDPP_USE_NEON32 || SIMDPP_USE_ALTIVEC
    double r = a.el(0);
    for (unsigned i = 1; i < a.length; i++) {
        r = r < a.el(i) ? r : a.el(i); // TODO nan
    }
    return r;
#endif
}

#if SIMDPP_USE_AVX
static SIMDPP_INL
double i_reduce_min(const float64x4& a)
{
    float64x2 ah = detail::extract128<1>(a);
    float64x2 al = detail::extract128<0>(a);
    al = min(al, ah);
    return i_reduce_min(al);
}
#endif

#if SIMDPP_USE_AVX512F
static SIMDPP_INL
double i_reduce_min(const float64<8>& a)
{
    return i_reduce_min(min(extract256<0>(a), extract256<1>(a)));
}
#endif

template<unsigned N>
SIMDPP_INL double i_reduce_min(const float64<N>& a)
{
    float64v r = a.vec(0);
    for (unsigned i = 1; i < a.vec_length; ++i)
        r = min(r, a.vec(i));
    return i_reduce_min(r);
}

} // namespace insn
} // namespace detail
} // namespace SIMDPP_ARCH_NAMESPACE
} // namespace simdpp

#endif

