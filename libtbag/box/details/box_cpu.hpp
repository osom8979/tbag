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

template <typename itype, typename otype>
void box_cpu_element_copy_impl(itype begin, itype end, otype output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        *output = static_cast<typename std::remove_pointer<otype>::type>(*begin);
    }
}

template <typename itype>
void box_cpu_element_copy_impl(itype src, ui32 size, void * dest, btype dest_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dest_type) {
    case BT_BOOL:    box_cpu_element_copy_impl(src, src + size, (bool *)dest); break;
    case BT_INT8:    box_cpu_element_copy_impl(src, src + size, (si8  *)dest); break;
    case BT_INT16:   box_cpu_element_copy_impl(src, src + size, (si16 *)dest); break;
    case BT_INT32:   box_cpu_element_copy_impl(src, src + size, (si32 *)dest); break;
    case BT_INT64:   box_cpu_element_copy_impl(src, src + size, (si64 *)dest); break;
    case BT_UINT8:   box_cpu_element_copy_impl(src, src + size, (ui8  *)dest); break;
    case BT_UINT16:  box_cpu_element_copy_impl(src, src + size, (ui16 *)dest); break;
    case BT_UINT32:  box_cpu_element_copy_impl(src, src + size, (ui32 *)dest); break;
    case BT_UINT64:  box_cpu_element_copy_impl(src, src + size, (ui64 *)dest); break;
    case BT_FLOAT32: box_cpu_element_copy_impl(src, src + size, (fp32 *)dest); break;
    case BT_FLOAT64: box_cpu_element_copy_impl(src, src + size, (fp64 *)dest); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <typename itype, typename otype>
void box_cpu_set_impl(itype const * in, otype * out) TBAG_NOEXCEPT
{
    *out = static_cast<otype>(*in);
}

template <typename itype>
void box_cpu_set_impl(itype const * in, void * out, btype out_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (out_type) {
    case BT_BOOL:    box_cpu_set_impl(in, (bool *)out); break;
    case BT_INT8:    box_cpu_set_impl(in, (si8  *)out); break;
    case BT_INT16:   box_cpu_set_impl(in, (si16 *)out); break;
    case BT_INT32:   box_cpu_set_impl(in, (si32 *)out); break;
    case BT_INT64:   box_cpu_set_impl(in, (si64 *)out); break;
    case BT_UINT8:   box_cpu_set_impl(in, (ui8  *)out); break;
    case BT_UINT16:  box_cpu_set_impl(in, (ui16 *)out); break;
    case BT_UINT32:  box_cpu_set_impl(in, (ui32 *)out); break;
    case BT_UINT64:  box_cpu_set_impl(in, (ui64 *)out); break;
    case BT_FLOAT32: box_cpu_set_impl(in, (fp32 *)out); break;
    case BT_FLOAT64: box_cpu_set_impl(in, (fp64 *)out); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

// clang-format off
template <typename L, typename R> struct equal_to      { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l == r; } };
template <typename L, typename R> struct not_equal     { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l != r; } };
template <typename L, typename R> struct less_than     { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l <  r; } };
template <typename L, typename R> struct less_equal    { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l <= r; } };
template <typename L, typename R> struct greater_than  { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l >  r; } };
template <typename L, typename R> struct greater_equal { inline bool operator ()(L l, R r) const TBAG_NOEXCEPT { return l >= r; } };
// clang-format on

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
    case BT_BOOL:    box_cpu_comp_impl<CompT>(lh, (bool const *)rh, out, size); break;
    case BT_INT8:    box_cpu_comp_impl<CompT>(lh, (si8  const *)rh, out, size); break;
    case BT_INT16:   box_cpu_comp_impl<CompT>(lh, (si16 const *)rh, out, size); break;
    case BT_INT32:   box_cpu_comp_impl<CompT>(lh, (si32 const *)rh, out, size); break;
    case BT_INT64:   box_cpu_comp_impl<CompT>(lh, (si64 const *)rh, out, size); break;
    case BT_UINT8:   box_cpu_comp_impl<CompT>(lh, (ui8  const *)rh, out, size); break;
    case BT_UINT16:  box_cpu_comp_impl<CompT>(lh, (ui16 const *)rh, out, size); break;
    case BT_UINT32:  box_cpu_comp_impl<CompT>(lh, (ui32 const *)rh, out, size); break;
    case BT_UINT64:  box_cpu_comp_impl<CompT>(lh, (ui64 const *)rh, out, size); break;
    case BT_FLOAT32: box_cpu_comp_impl<CompT>(lh, (fp32 const *)rh, out, size); break;
    case BT_FLOAT64: box_cpu_comp_impl<CompT>(lh, (fp64 const *)rh, out, size); break;
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
    case BT_BOOL:    box_cpu_comp_impl<CompT>((bool const *)lh, rh, rh_type, out, size); break;
    case BT_INT8:    box_cpu_comp_impl<CompT>((si8  const *)lh, rh, rh_type, out, size); break;
    case BT_INT16:   box_cpu_comp_impl<CompT>((si16 const *)lh, rh, rh_type, out, size); break;
    case BT_INT32:   box_cpu_comp_impl<CompT>((si32 const *)lh, rh, rh_type, out, size); break;
    case BT_INT64:   box_cpu_comp_impl<CompT>((si64 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT8:   box_cpu_comp_impl<CompT>((ui8  const *)lh, rh, rh_type, out, size); break;
    case BT_UINT16:  box_cpu_comp_impl<CompT>((ui16 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT32:  box_cpu_comp_impl<CompT>((ui32 const *)lh, rh, rh_type, out, size); break;
    case BT_UINT64:  box_cpu_comp_impl<CompT>((ui64 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT32: box_cpu_comp_impl<CompT>((fp32 const *)lh, rh, rh_type, out, size); break;
    case BT_FLOAT64: box_cpu_comp_impl<CompT>((fp64 const *)lh, rh, rh_type, out, size); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__

