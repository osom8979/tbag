/**
 * @file   box_cpu.cpp
 * @brief  box_cpu class implementation.
 * @author zer0
 * @date   2020-01-07
 */

#include <libtbag/box/details/box_cpu.hpp>
#include <libtbag/memory/Memory.hpp>

#include <cassert>
#include <cstring>

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
    case BT_BOOL:    box_cpu_element_copy_impl((bool *)src, size, dest, dest_type); break;
    case BT_INT8:    box_cpu_element_copy_impl((si8  *)src, size, dest, dest_type); break;
    case BT_INT16:   box_cpu_element_copy_impl((si16 *)src, size, dest, dest_type); break;
    case BT_INT32:   box_cpu_element_copy_impl((si32 *)src, size, dest, dest_type); break;
    case BT_INT64:   box_cpu_element_copy_impl((si64 *)src, size, dest, dest_type); break;
    case BT_UINT8:   box_cpu_element_copy_impl((ui8  *)src, size, dest, dest_type); break;
    case BT_UINT16:  box_cpu_element_copy_impl((ui16 *)src, size, dest, dest_type); break;
    case BT_UINT32:  box_cpu_element_copy_impl((ui32 *)src, size, dest, dest_type); break;
    case BT_UINT64:  box_cpu_element_copy_impl((ui64 *)src, size, dest, dest_type); break;
    case BT_FLOAT32: box_cpu_element_copy_impl((fp32 *)src, size, dest, dest_type); break;
    case BT_FLOAT64: box_cpu_element_copy_impl((fp64 *)src, size, dest, dest_type); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

void box_cpu_set(void * TBAG_RESTRICT dest, btype dest_type,
                 void const * TBAG_RESTRICT src, btype src_type) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);

    // clang-format off
    switch (src_type) {
    case BT_BOOL:    box_cpu_set_impl((bool *)src, dest, dest_type); break;
    case BT_INT8:    box_cpu_set_impl((si8  *)src, dest, dest_type); break;
    case BT_INT16:   box_cpu_set_impl((si16 *)src, dest, dest_type); break;
    case BT_INT32:   box_cpu_set_impl((si32 *)src, dest, dest_type); break;
    case BT_INT64:   box_cpu_set_impl((si64 *)src, dest, dest_type); break;
    case BT_UINT8:   box_cpu_set_impl((ui8  *)src, dest, dest_type); break;
    case BT_UINT16:  box_cpu_set_impl((ui16 *)src, dest, dest_type); break;
    case BT_UINT32:  box_cpu_set_impl((ui32 *)src, dest, dest_type); break;
    case BT_UINT64:  box_cpu_set_impl((ui64 *)src, dest, dest_type); break;
    case BT_FLOAT32: box_cpu_set_impl((fp32 *)src, dest, dest_type); break;
    case BT_FLOAT64: box_cpu_set_impl((fp64 *)src, dest, dest_type); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

bool box_cpu_all(void const * data, btype type, ui32 size)
{
    // clang-format off
    switch (type) {
    case BT_BOOL:    return box_cpu_all_impl((bool const *)data, size);
    case BT_INT8:    return box_cpu_all_impl((si8  const *)data, size);
    case BT_INT16:   return box_cpu_all_impl((si16 const *)data, size);
    case BT_INT32:   return box_cpu_all_impl((si32 const *)data, size);
    case BT_INT64:   return box_cpu_all_impl((si64 const *)data, size);
    case BT_UINT8:   return box_cpu_all_impl((ui8  const *)data, size);
    case BT_UINT16:  return box_cpu_all_impl((ui16 const *)data, size);
    case BT_UINT32:  return box_cpu_all_impl((ui32 const *)data, size);
    case BT_UINT64:  return box_cpu_all_impl((ui64 const *)data, size);
    case BT_FLOAT32: return box_cpu_all_impl((fp32 const *)data, size);
    case BT_FLOAT64: return box_cpu_all_impl((fp64 const *)data, size);
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return false;
    }
    // clang-format on
}

bool box_cpu_any(void const * data, btype type, ui32 size)
{
    // clang-format off
    switch (type) {
    case BT_BOOL:    return box_cpu_any_impl((bool const *)data, size);
    case BT_INT8:    return box_cpu_any_impl((si8  const *)data, size);
    case BT_INT16:   return box_cpu_any_impl((si16 const *)data, size);
    case BT_INT32:   return box_cpu_any_impl((si32 const *)data, size);
    case BT_INT64:   return box_cpu_any_impl((si64 const *)data, size);
    case BT_UINT8:   return box_cpu_any_impl((ui8  const *)data, size);
    case BT_UINT16:  return box_cpu_any_impl((ui16 const *)data, size);
    case BT_UINT32:  return box_cpu_any_impl((ui32 const *)data, size);
    case BT_UINT64:  return box_cpu_any_impl((ui64 const *)data, size);
    case BT_FLOAT32: return box_cpu_any_impl((fp32 const *)data, size);
    case BT_FLOAT64: return box_cpu_any_impl((fp64 const *)data, size);
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return false;
    }
    // clang-format on
}

std::size_t box_cpu_count(void const * data, btype type, ui32 size)
{
    // clang-format off
    switch (type) {
    case BT_BOOL:    return box_cpu_count_impl((bool const *)data, size);
    case BT_INT8:    return box_cpu_count_impl((si8  const *)data, size);
    case BT_INT16:   return box_cpu_count_impl((si16 const *)data, size);
    case BT_INT32:   return box_cpu_count_impl((si32 const *)data, size);
    case BT_INT64:   return box_cpu_count_impl((si64 const *)data, size);
    case BT_UINT8:   return box_cpu_count_impl((ui8  const *)data, size);
    case BT_UINT16:  return box_cpu_count_impl((ui16 const *)data, size);
    case BT_UINT32:  return box_cpu_count_impl((ui32 const *)data, size);
    case BT_UINT64:  return box_cpu_count_impl((ui64 const *)data, size);
    case BT_FLOAT32: return box_cpu_count_impl((fp32 const *)data, size);
    case BT_FLOAT64: return box_cpu_count_impl((fp64 const *)data, size);
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return 0;
    }
    // clang-format on
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

