/**
 * @file   box_cpu.cpp
 * @brief  box cpu implementation.
 * @author zer0
 * @date   2019-03-04
 */

#include <libtbag/box/details/box_cpu.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/memory/Memory.hpp>

#include <cassert>
#include <cstring>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

void * box_cpu_malloc(ui32 byte_size) TBAG_NOEXCEPT
{
    static auto const DEFAULT_ALIGN_BYTE = static_cast<std::size_t>(tbDefaultAlignSize());
    assert(DEFAULT_ALIGN_BYTE >= 1);
    return tbAlignedMalloc(byte_size, DEFAULT_ALIGN_BYTE);
}

void box_cpu_free(void * ptr) TBAG_NOEXCEPT
{
    assert(ptr != nullptr);
    tbAlignedFree(ptr);
}

void box_cpu_memcpy(void * TBAG_RESTRICT dest, void const * TBAG_RESTRICT src, ui32 byte) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(dest != src);
    assert(byte >= 1);
    memcpy(dest, src, byte);
}

template <typename itype, typename otype>
inline static void _box_cpu_element_copy_impl(itype begin, itype end, otype output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        *output = static_cast<typename std::remove_pointer<otype>::type>(*begin);
    }
}

template <typename itype>
inline static void _box_cpu_element_copy_impl(itype src, ui32 size, void * dest, btype dest_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dest_type) {
    case BOX_TYPE_INT8:    _box_cpu_element_copy_impl(src, src + size, (si8  *)dest); break;
    case BOX_TYPE_INT16:   _box_cpu_element_copy_impl(src, src + size, (si16 *)dest); break;
    case BOX_TYPE_INT32:   _box_cpu_element_copy_impl(src, src + size, (si32 *)dest); break;
    case BOX_TYPE_INT64:   _box_cpu_element_copy_impl(src, src + size, (si64 *)dest); break;
    case BOX_TYPE_UINT8:   _box_cpu_element_copy_impl(src, src + size, (ui8  *)dest); break;
    case BOX_TYPE_UINT16:  _box_cpu_element_copy_impl(src, src + size, (ui16 *)dest); break;
    case BOX_TYPE_UINT32:  _box_cpu_element_copy_impl(src, src + size, (ui32 *)dest); break;
    case BOX_TYPE_UINT64:  _box_cpu_element_copy_impl(src, src + size, (ui64 *)dest); break;
    case BOX_TYPE_FLOAT32: _box_cpu_element_copy_impl(src, src + size, (fp32 *)dest); break;
    case BOX_TYPE_FLOAT64: _box_cpu_element_copy_impl(src, src + size, (fp64 *)dest); break;
    case BOX_TYPE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

void box_cpu_element_copy(void * TBAG_RESTRICT dest, btype dest_type,
                          void const * TBAG_RESTRICT src, btype src_type,
                          ui32 size) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(dest != src);
    assert(size >= 1);

    // clang-format off
    switch (src_type) {
    case BOX_TYPE_INT8:    _box_cpu_element_copy_impl((si8  *)src, size, dest, dest_type); break;
    case BOX_TYPE_INT16:   _box_cpu_element_copy_impl((si16 *)src, size, dest, dest_type); break;
    case BOX_TYPE_INT32:   _box_cpu_element_copy_impl((si32 *)src, size, dest, dest_type); break;
    case BOX_TYPE_INT64:   _box_cpu_element_copy_impl((si64 *)src, size, dest, dest_type); break;
    case BOX_TYPE_UINT8:   _box_cpu_element_copy_impl((ui8  *)src, size, dest, dest_type); break;
    case BOX_TYPE_UINT16:  _box_cpu_element_copy_impl((ui16 *)src, size, dest, dest_type); break;
    case BOX_TYPE_UINT32:  _box_cpu_element_copy_impl((ui32 *)src, size, dest, dest_type); break;
    case BOX_TYPE_UINT64:  _box_cpu_element_copy_impl((ui64 *)src, size, dest, dest_type); break;
    case BOX_TYPE_FLOAT32: _box_cpu_element_copy_impl((fp32 *)src, size, dest, dest_type); break;
    case BOX_TYPE_FLOAT64: _box_cpu_element_copy_impl((fp64 *)src, size, dest, dest_type); break;
    case BOX_TYPE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

template <typename itype, typename otype>
inline static void _box_cpu_set_impl(itype const * in, otype * out) TBAG_NOEXCEPT
{
    *out = static_cast<otype>(*in);
}

template <typename itype>
inline static void _box_cpu_set_impl(itype const * in, void * out, btype out_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (out_type) {
    case BOX_TYPE_INT8:    _box_cpu_set_impl(in, (si8  *)out); break;
    case BOX_TYPE_INT16:   _box_cpu_set_impl(in, (si16 *)out); break;
    case BOX_TYPE_INT32:   _box_cpu_set_impl(in, (si32 *)out); break;
    case BOX_TYPE_INT64:   _box_cpu_set_impl(in, (si64 *)out); break;
    case BOX_TYPE_UINT8:   _box_cpu_set_impl(in, (ui8  *)out); break;
    case BOX_TYPE_UINT16:  _box_cpu_set_impl(in, (ui16 *)out); break;
    case BOX_TYPE_UINT32:  _box_cpu_set_impl(in, (ui32 *)out); break;
    case BOX_TYPE_UINT64:  _box_cpu_set_impl(in, (ui64 *)out); break;
    case BOX_TYPE_FLOAT32: _box_cpu_set_impl(in, (fp32 *)out); break;
    case BOX_TYPE_FLOAT64: _box_cpu_set_impl(in, (fp64 *)out); break;
    case BOX_TYPE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

TBAG_API void box_cpu_set(void * TBAG_RESTRICT dest, btype dest_type,
                          void const * TBAG_RESTRICT src, btype src_type) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);

    // clang-format off
    switch (src_type) {
    case BOX_TYPE_INT8:    _box_cpu_set_impl((si8  *)src, dest, dest_type); break;
    case BOX_TYPE_INT16:   _box_cpu_set_impl((si16 *)src, dest, dest_type); break;
    case BOX_TYPE_INT32:   _box_cpu_set_impl((si32 *)src, dest, dest_type); break;
    case BOX_TYPE_INT64:   _box_cpu_set_impl((si64 *)src, dest, dest_type); break;
    case BOX_TYPE_UINT8:   _box_cpu_set_impl((ui8  *)src, dest, dest_type); break;
    case BOX_TYPE_UINT16:  _box_cpu_set_impl((ui16 *)src, dest, dest_type); break;
    case BOX_TYPE_UINT32:  _box_cpu_set_impl((ui32 *)src, dest, dest_type); break;
    case BOX_TYPE_UINT64:  _box_cpu_set_impl((ui64 *)src, dest, dest_type); break;
    case BOX_TYPE_FLOAT32: _box_cpu_set_impl((fp32 *)src, dest, dest_type); break;
    case BOX_TYPE_FLOAT64: _box_cpu_set_impl((fp64 *)src, dest, dest_type); break;
    case BOX_TYPE_NONE:
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

