/**
 * @file   box_cpu.hpp
 * @brief  box_cpu class prototype.
 * @author zer0
 * @date   2020-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/box/details/box_common.hpp>

#include <cmath>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

TBAG_API void * box_cpu_malloc(ui32 byte_size) TBAG_NOEXCEPT;
TBAG_API void box_cpu_free(void * ptr) TBAG_NOEXCEPT;
TBAG_API void box_cpu_memcpy(void * TBAG_RESTRICT dest,
                             void const * TBAG_RESTRICT src,
                             ui32 byte) TBAG_NOEXCEPT;
TBAG_API void box_cpu_element_copy(void * TBAG_RESTRICT dest, btype dest_type,
                                   void const * TBAG_RESTRICT src, btype src_type,
                                   ui32 size) TBAG_NOEXCEPT;
TBAG_API void box_cpu_set(void * TBAG_RESTRICT dest, btype dest_type,
                          void const * TBAG_RESTRICT src, btype src_type) TBAG_NOEXCEPT;

struct    no_complex_t { /* EMPTY. */ };
struct  left_complex_t { /* EMPTY. */ };
struct right_complex_t { /* EMPTY. */ };
struct   all_complex_t { /* EMPTY. */ };

template <typename LeftT, typename RightT>
struct complex_selector
{
    using __l = typename libtbag::remove_cpr<LeftT>::type;
    using __r = typename libtbag::remove_cpr<RightT>::type;

    TBAG_CONSTEXPR static bool const is_left_complex = libtbag::is_complex<__l>::value;
    TBAG_CONSTEXPR static bool const is_right_complex = libtbag::is_complex<__r>::value;

    // clang-format off
    using type = typename std::conditional< is_left_complex &&  is_right_complex, all_complex_t,
                 typename std::conditional< is_left_complex && !is_right_complex, left_complex_t,
                 typename std::conditional<!is_left_complex &&  is_right_complex, right_complex_t,
                 no_complex_t
                 >::type // right_complex_t
                 >::type // left_complex_t
                 >::type;// io_complex_t
    // clang-format on

    TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
};

template <typename T>
struct single_complex_selector
{
    using type = typename std::conditional<
            libtbag::is_complex<typename libtbag::remove_cpr<T>::type>::value,
            all_complex_t,
            no_complex_t
    >::type;
    TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
};

template <typename InputT, typename OutputT>
void box_cpu_element_copy_impl2(no_complex_t, InputT begin, InputT end, OutputT output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        *output = *begin;
    }
}

template <typename InputT, typename OutputT>
void box_cpu_element_copy_impl2(left_complex_t, InputT begin, InputT end, OutputT output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        *output = begin->real();
    }
}

template <typename InputT, typename OutputT>
void box_cpu_element_copy_impl2(right_complex_t, InputT begin, InputT end, OutputT output) TBAG_NOEXCEPT
{
    using __output_complex_t = typename libtbag::remove_cpr<OutputT>::type;
    using __output_value_t = typename __output_complex_t::value_type;
    for (; begin != end; ++begin, (void)++output) {
        output->real(*begin);
        output->imag(__output_value_t{});
    }
}

template <typename InputT, typename OutputT>
void box_cpu_element_copy_impl2(all_complex_t, InputT begin, InputT end, OutputT output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        output->real(begin->real());
        output->imag(begin->imag());
    }
}

template <typename InputT, typename OutputT>
void box_cpu_element_copy_impl(InputT begin, InputT end, OutputT output) TBAG_NOEXCEPT
{
    box_cpu_element_copy_impl2(complex_selector<InputT, OutputT>::value, begin, end, output);
}

template <typename InputT>
void box_cpu_element_copy_impl(InputT src, ui32 size, void * dest, btype dest_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dest_type) {
    case BT_BOOL:       box_cpu_element_copy_impl(src, src + size, (bool *)dest); break;
    case BT_INT8:       box_cpu_element_copy_impl(src, src + size, (si8  *)dest); break;
    case BT_INT16:      box_cpu_element_copy_impl(src, src + size, (si16 *)dest); break;
    case BT_INT32:      box_cpu_element_copy_impl(src, src + size, (si32 *)dest); break;
    case BT_INT64:      box_cpu_element_copy_impl(src, src + size, (si64 *)dest); break;
    case BT_UINT8:      box_cpu_element_copy_impl(src, src + size, (ui8  *)dest); break;
    case BT_UINT16:     box_cpu_element_copy_impl(src, src + size, (ui16 *)dest); break;
    case BT_UINT32:     box_cpu_element_copy_impl(src, src + size, (ui32 *)dest); break;
    case BT_UINT64:     box_cpu_element_copy_impl(src, src + size, (ui64 *)dest); break;
    case BT_FLOAT32:    box_cpu_element_copy_impl(src, src + size, (fp32 *)dest); break;
    case BT_FLOAT64:    box_cpu_element_copy_impl(src, src + size, (fp64 *)dest); break;
    case BT_COMPLEX64:  box_cpu_element_copy_impl(src, src + size, (c64  *)dest); break;
    case BT_COMPLEX128: box_cpu_element_copy_impl(src, src + size, (c128 *)dest); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <typename InputT, typename OutputT>
void box_cpu_set_impl2(no_complex_t, InputT const * in, OutputT * out) TBAG_NOEXCEPT
{
    *out = *in;
}

template <typename InputT, typename OutputT>
void box_cpu_set_impl2(left_complex_t, InputT const * in, OutputT * out) TBAG_NOEXCEPT
{
    *out = in->real();
}

template <typename InputT, typename OutputT>
void box_cpu_set_impl2(right_complex_t, InputT const * in, OutputT * out) TBAG_NOEXCEPT
{
    using __output_complex_t = typename libtbag::remove_cpr<OutputT>::type;
    using __output_value_t = typename __output_complex_t::value_type;
    out->real(*in);
    out->imag(__output_value_t{});
}

template <typename InputT, typename OutputT>
void box_cpu_set_impl2(all_complex_t, InputT const * in, OutputT * out) TBAG_NOEXCEPT
{
    out->real(in->real());
    out->imag(in->imag());
}

template <typename InputT, typename OutputT>
void box_cpu_set_impl(InputT const * in, OutputT * out) TBAG_NOEXCEPT
{
    box_cpu_set_impl2(complex_selector<InputT, OutputT>::value, in, out);
}

template <typename InputT>
void box_cpu_set_impl(InputT const * in, void * out, btype out_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (out_type) {
    case BT_BOOL:       box_cpu_set_impl(in, (bool *)out); break;
    case BT_INT8:       box_cpu_set_impl(in, (si8  *)out); break;
    case BT_INT16:      box_cpu_set_impl(in, (si16 *)out); break;
    case BT_INT32:      box_cpu_set_impl(in, (si32 *)out); break;
    case BT_INT64:      box_cpu_set_impl(in, (si64 *)out); break;
    case BT_UINT8:      box_cpu_set_impl(in, (ui8  *)out); break;
    case BT_UINT16:     box_cpu_set_impl(in, (ui16 *)out); break;
    case BT_UINT32:     box_cpu_set_impl(in, (ui32 *)out); break;
    case BT_UINT64:     box_cpu_set_impl(in, (ui64 *)out); break;
    case BT_FLOAT32:    box_cpu_set_impl(in, (fp32 *)out); break;
    case BT_FLOAT64:    box_cpu_set_impl(in, (fp64 *)out); break;
    case BT_COMPLEX64:  box_cpu_set_impl(in, (c64  *)out); break;
    case BT_COMPLEX128: box_cpu_set_impl(in, (c128 *)out); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

// Numpy reference: numpy/core/src/umath/loops.c.src +2461
// *****************************************************************************
// **                           COMPLEX LOOPS                                 **
// *****************************************************************************
#define TBAG_COMPLEX_GE(xr,xi,yr,yi) ((xr > yr && !isnan(xi) && !isnan(yi)) || (xr == yr && xi >= yi))
#define TBAG_COMPLEX_LE(xr,xi,yr,yi) ((xr < yr && !isnan(xi) && !isnan(yi)) || (xr == yr && xi <= yi))
#define TBAG_COMPLEX_GT(xr,xi,yr,yi) ((xr > yr && !isnan(xi) && !isnan(yi)) || (xr == yr && xi > yi))
#define TBAG_COMPLEX_LT(xr,xi,yr,yi) ((xr < yr && !isnan(xi) && !isnan(yi)) || (xr == yr && xi < yi))
#define TBAG_COMPLEX_EQ(xr,xi,yr,yi) (xr == yr && xi == yi)
#define TBAG_COMPLEX_NE(xr,xi,yr,yi) (xr != yr || xi != yi)

template <typename L, typename R>
struct equal_to
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l == r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_EQ(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_EQ((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_EQ(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <typename L, typename R>
struct not_equal
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l != r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_NE(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_NE((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_NE(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <typename L, typename R>
struct less_than
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l < r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LT(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LT((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LT(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <typename L, typename R>
struct less_equal
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l <= r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LE(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LE((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_LE(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <typename L, typename R>
struct greater_than
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l > r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GT(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GT((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GT(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <typename L, typename R>
struct greater_equal
{
    inline bool __compare(no_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return l >= r;
    }

    inline bool __compare(left_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GE(l.real(), l.imag(), (typename L::value_type)r, (typename L::value_type){});
    }

    inline bool __compare(right_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GE((typename R::value_type)l, (typename R::value_type){}, r.real(), r.imag());
    }

    inline bool __compare(all_complex_t, L l, R r) const TBAG_NOEXCEPT
    {
        return TBAG_COMPLEX_GE(l.real(), l.imag(), r.real(), r.imag());
    }

    inline bool operator ()(L l, R r) const TBAG_NOEXCEPT
    {
        return __compare(complex_selector<L, R>::value, l, r);
    }
};

template <template <typename LeftT, typename RightT> class CompT, typename ltype, typename rtype>
void box_cpu_comp_impl(ltype const * lh, rtype const * rh,
                       bool * out, std::size_t size) TBAG_NOEXCEPT
{
    CompT<ltype, rtype> const compare;
    for (auto i = 0u; i < size; ++i) {
        out[i] = compare(lh[i], rh[i]);
    }
}

template <template <typename LeftT, typename RightT> class CompT, typename ltype>
void box_cpu_comp_impl(ltype const * lh, void const * rh, btype rh_type,
                       bool * out, std::size_t size) TBAG_NOEXCEPT
{
    // clang-format off
    switch (rh_type) {
    case BT_BOOL:       box_cpu_comp_impl<CompT>(lh, (bool const *)rh, out, size); break;
    case BT_INT8:       box_cpu_comp_impl<CompT>(lh, (si8  const *)rh, out, size); break;
    case BT_INT16:      box_cpu_comp_impl<CompT>(lh, (si16 const *)rh, out, size); break;
    case BT_INT32:      box_cpu_comp_impl<CompT>(lh, (si32 const *)rh, out, size); break;
    case BT_INT64:      box_cpu_comp_impl<CompT>(lh, (si64 const *)rh, out, size); break;
    case BT_UINT8:      box_cpu_comp_impl<CompT>(lh, (ui8  const *)rh, out, size); break;
    case BT_UINT16:     box_cpu_comp_impl<CompT>(lh, (ui16 const *)rh, out, size); break;
    case BT_UINT32:     box_cpu_comp_impl<CompT>(lh, (ui32 const *)rh, out, size); break;
    case BT_UINT64:     box_cpu_comp_impl<CompT>(lh, (ui64 const *)rh, out, size); break;
    case BT_FLOAT32:    box_cpu_comp_impl<CompT>(lh, (fp32 const *)rh, out, size); break;
    case BT_FLOAT64:    box_cpu_comp_impl<CompT>(lh, (fp64 const *)rh, out, size); break;
    case BT_COMPLEX64:  box_cpu_comp_impl<CompT>(lh, (c64  const *)rh, out, size); break;
    case BT_COMPLEX128: box_cpu_comp_impl<CompT>(lh, (c128 const *)rh, out, size); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <template <typename LeftT, typename RightT> class CompT>
void box_cpu_comp(void const * lh, btype lh_type,
                  void const * rh, btype rh_type,
                  bool * out, std::size_t size) TBAG_NOEXCEPT
{
    assert(rh != nullptr);
    assert(lh != nullptr);

    // clang-format off
    switch (lh_type) {
    case BT_BOOL:       box_cpu_comp_impl<CompT>((bool const *)lh, rh, rh_type, out, size); break;
    case BT_INT8:       box_cpu_comp_impl<CompT>((si8  const *)lh, rh, rh_type, out, size); break;
    case BT_INT16:      box_cpu_comp_impl<CompT>((si16 const *)lh, rh, rh_type, out, size); break;
    case BT_INT32:      box_cpu_comp_impl<CompT>((si32 const *)lh, rh, rh_type, out, size); break;
    case BT_INT64:      box_cpu_comp_impl<CompT>((si64 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT8:      box_cpu_comp_impl<CompT>((ui8  const *)lh, rh, rh_type, out, size); break;
    case BT_UINT16:     box_cpu_comp_impl<CompT>((ui16 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT32:     box_cpu_comp_impl<CompT>((ui32 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT64:     box_cpu_comp_impl<CompT>((ui64 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT32:    box_cpu_comp_impl<CompT>((fp32 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT64:    box_cpu_comp_impl<CompT>((fp64 const *)lh, rh, rh_type, out, size); break;
    case BT_COMPLEX64:  box_cpu_comp_impl<CompT>((c64  const *)lh, rh, rh_type, out, size); break;
    case BT_COMPLEX128: box_cpu_comp_impl<CompT>((c128 const *)lh, rh, rh_type, out, size); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <template <typename LeftT, typename RightT> class CompT, typename ltype, typename rtype>
void box_cpu_comp_value_impl(ltype const * lh, rtype const * rh,
                             bool * out, std::size_t size) TBAG_NOEXCEPT
{
    CompT<ltype, rtype> const compare;
    for (auto i = 0u; i < size; ++i) {
        out[i] = compare(lh[i], *rh);
    }
}

template <template <typename LeftT, typename RightT> class CompT, typename ltype>
void box_cpu_comp_value_impl(ltype const * lh, void const * rh, btype rh_type,
                             bool * out, std::size_t size) TBAG_NOEXCEPT
{
    // clang-format off
    switch (rh_type) {
    case BT_BOOL:       box_cpu_comp_value_impl<CompT>(lh, (bool const *)rh, out, size); break;
    case BT_INT8:       box_cpu_comp_value_impl<CompT>(lh, (si8  const *)rh, out, size); break;
    case BT_INT16:      box_cpu_comp_value_impl<CompT>(lh, (si16 const *)rh, out, size); break;
    case BT_INT32:      box_cpu_comp_value_impl<CompT>(lh, (si32 const *)rh, out, size); break;
    case BT_INT64:      box_cpu_comp_value_impl<CompT>(lh, (si64 const *)rh, out, size); break;
    case BT_UINT8:      box_cpu_comp_value_impl<CompT>(lh, (ui8  const *)rh, out, size); break;
    case BT_UINT16:     box_cpu_comp_value_impl<CompT>(lh, (ui16 const *)rh, out, size); break;
    case BT_UINT32:     box_cpu_comp_value_impl<CompT>(lh, (ui32 const *)rh, out, size); break;
    case BT_UINT64:     box_cpu_comp_value_impl<CompT>(lh, (ui64 const *)rh, out, size); break;
    case BT_FLOAT32:    box_cpu_comp_value_impl<CompT>(lh, (fp32 const *)rh, out, size); break;
    case BT_FLOAT64:    box_cpu_comp_value_impl<CompT>(lh, (fp64 const *)rh, out, size); break;
    case BT_COMPLEX64:  box_cpu_comp_value_impl<CompT>(lh, (c64  const *)rh, out, size); break;
    case BT_COMPLEX128: box_cpu_comp_value_impl<CompT>(lh, (c128 const *)rh, out, size); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <template <typename LeftT, typename RightT> class CompT>
void box_cpu_value_comp(void const * lh, btype lh_type,
                        void const * rh, btype rh_type,
                        bool * out, std::size_t size) TBAG_NOEXCEPT
{
    assert(rh != nullptr);
    assert(lh != nullptr);

    // clang-format off
    switch (lh_type) {
    case BT_BOOL:       box_cpu_comp_value_impl<CompT>((bool const *)lh, rh, rh_type, out, size); break;
    case BT_INT8:       box_cpu_comp_value_impl<CompT>((si8  const *)lh, rh, rh_type, out, size); break;
    case BT_INT16:      box_cpu_comp_value_impl<CompT>((si16 const *)lh, rh, rh_type, out, size); break;
    case BT_INT32:      box_cpu_comp_value_impl<CompT>((si32 const *)lh, rh, rh_type, out, size); break;
    case BT_INT64:      box_cpu_comp_value_impl<CompT>((si64 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT8:      box_cpu_comp_value_impl<CompT>((ui8  const *)lh, rh, rh_type, out, size); break;
    case BT_UINT16:     box_cpu_comp_value_impl<CompT>((ui16 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT32:     box_cpu_comp_value_impl<CompT>((ui32 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT64:     box_cpu_comp_value_impl<CompT>((ui64 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT32:    box_cpu_comp_value_impl<CompT>((fp32 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT64:    box_cpu_comp_value_impl<CompT>((fp64 const *)lh, rh, rh_type, out, size); break;
    case BT_COMPLEX64:  box_cpu_comp_value_impl<CompT>((c64  const *)lh, rh, rh_type, out, size); break;
    case BT_COMPLEX128: box_cpu_comp_value_impl<CompT>((c128 const *)lh, rh, rh_type, out, size); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <typename T>
bool box_cpu_all_impl2(no_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin) {
        if (!(*begin)) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool box_cpu_all_impl2(all_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin) {
        if (!(begin->real())) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool box_cpu_all_impl(T const * begin, T const * end) TBAG_NOEXCEPT
{
    return box_cpu_all_impl2(single_complex_selector<T>::value, begin, end);
}

template <typename T>
bool box_cpu_all_impl(T const * begin, ui32 size) TBAG_NOEXCEPT
{
    return box_cpu_all_impl(begin, begin + size);
}

TBAG_API bool box_cpu_all(void const * data, btype type, ui32 size);

template <typename T>
bool box_cpu_any_impl2(no_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin) {
        if ((*begin)) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool box_cpu_any_impl2(all_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin) {
        if ((begin->real())) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool box_cpu_any_impl(T const * begin, T const * end) TBAG_NOEXCEPT
{
    return box_cpu_any_impl2(single_complex_selector<T>::value, begin ,end);
}

template <typename T>
bool box_cpu_any_impl(T const * begin, ui32 size) TBAG_NOEXCEPT
{
    return box_cpu_any_impl(begin, begin + size);
}

TBAG_API bool box_cpu_any(void const * data, btype type, ui32 size);

template <typename T>
std::size_t box_cpu_count_impl2(no_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    std::size_t count = 0;
    for (; begin != end; ++begin) {
        if ((*begin)) {
            ++count;
        }
    }
    return count;
}

template <typename T>
std::size_t box_cpu_count_impl2(all_complex_t, T const * begin, T const * end) TBAG_NOEXCEPT
{
    std::size_t count = 0;
    for (; begin != end; ++begin) {
        if (begin->real()) {
            ++count;
        }
    }
    return count;
}

template <typename T>
std::size_t box_cpu_count_impl(T const * begin, T const * end) TBAG_NOEXCEPT
{
    return box_cpu_count_impl2(single_complex_selector<T>::value, begin, end);
}

template <typename T>
std::size_t box_cpu_count_impl(T const * begin, ui32 size) TBAG_NOEXCEPT
{
    return box_cpu_count_impl(begin, begin + size);
}

TBAG_API std::size_t box_cpu_count(void const * data, btype type, ui32 size);

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__

