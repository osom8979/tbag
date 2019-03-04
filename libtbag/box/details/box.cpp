/**
 * @file   box.cpp
 * @brief  C-based box method implementation.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
 */

#include <libtbag/box/details/box.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/memory/Memory.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    return (ui32*)tbMalloc(rank);
}

void box_dim_free(ui32 * dims) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    tbFree(dims);
}

void box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(rank >= 1);
    while (rank) {
        *dest = *src;
        ++dest;
        ++src;
        --rank;
    }
}

ui32 * box_dim_malloc_copy(ui32 const * src, ui32 rank) TBAG_NOEXCEPT
{
    assert(src != nullptr);
    assert(rank >= 1);
    ui32 * result = box_dim_malloc(rank);
    assert(result != nullptr);
    box_dim_copy(result, src, rank);
    return result;
}

bool box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT
{
    assert(dims1 != nullptr);
    assert(dims2 != nullptr);
    if (rank1 != rank2) {
        return false;
    }
    assert(rank1 == rank2);
    if (dims1 == dims2) {
        return true;
    }
    assert(dims1 != dims2);
    for (ui32 i = 0; i < rank1; ++i) {
        if (dims1[i] != dims2[i]) {
            return false;
        }
    }
    return true;
}

static void * _box_malloc_cpu(btype type, ui32 total_byte) TBAG_NOEXCEPT
{
    static auto const DEFAULT_ALIGN_BYTE = static_cast<std::size_t>(tbDefaultAlignSize());
    assert(DEFAULT_ALIGN_BYTE >= 1);
    assert(box_support_type(type));
    return tbAlignedMalloc(total_byte, DEFAULT_ALIGN_BYTE);
}

static void _box_free_cpu(btype type, void * ptr) TBAG_NOEXCEPT
{
    assert(ptr != nullptr);
    assert(box_support_type(type));
    tbAlignedFree(ptr);
}

static void * _box_malloc_backend(btype type, bdev device, ui32 total_byte) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (device) {
    case BOX_DEVICE_CPU:  return _box_malloc_cpu(type, total_byte);
    case BOX_DEVICE_CUDA: TBAG_FALLTHROUGH
    case BOX_DEVICE_CL:   TBAG_FALLTHROUGH
    case BOX_DEVICE_GLSL: TBAG_FALLTHROUGH
    case BOX_DEVICE_NONE: TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on
    return nullptr;
}

static void _box_free_backend(btype type, bdev device, void * ptr) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (device) {
    case BOX_DEVICE_CPU:  _box_free_cpu(type, ptr); break;
    case BOX_DEVICE_CUDA: TBAG_FALLTHROUGH
    case BOX_DEVICE_CL:   TBAG_FALLTHROUGH
    case BOX_DEVICE_GLSL: TBAG_FALLTHROUGH
    case BOX_DEVICE_NONE: TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on
}

Err box_malloc(box_data * box, btype type, bdev device, ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);

    auto const TOTAL_BYTE = box_get_type_byte(type) * box_get_total_length(dims, rank);
    assert(TOTAL_BYTE >= 1);

    box->type = type;
    box->device = device;
    box->data = _box_malloc_backend(type, device, TOTAL_BYTE);
    box->total_byte = TOTAL_BYTE;
    box->dims = box_dim_malloc_copy(dims, rank);
    box->rank = rank;
    return Err::E_SUCCESS;
}

Err box_free(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    _box_free_backend(box->type, box->device, box->data);
    box_dim_free(box->dims);
    return Err::E_SUCCESS;
}

bool box_exists_data(box_data const * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box->data != nullptr;
}

Err box_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    auto const CODE = box_malloc(dest, src->type, src->device, src->dims, src->rank);
    if (isFailure(CODE)) {
        return CODE;
    }
    return box_memcpy(dest, src);
}

Err box_checked_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    if (box_exists_data(dest)) {
        box_free(dest);
    }
    return box_clone(dest, src);
}

//Err _box_memcpy_cpu_backend(box_data * dest, box_data const * src) TBAG_NOEXCEPT
//{
//    auto const SIZE = (src->total_byte / box_get_type_byte(src->type));
//    for (ui32 i = 0; i < SIZE; ++i) {
//    }
//}

Err box_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
//    switch (dest->type) {
//    case BOX_TYPE_STRING:   return sizeof(char);
//    case BOX_TYPE_INT8:     return sizeof(si8);
//    case BOX_TYPE_INT16:    return sizeof(si16);
//    case BOX_TYPE_INT32:    return sizeof(si32);
//    case BOX_TYPE_INT64:    return sizeof(si64);
//    case BOX_TYPE_UINT8:    return sizeof(ui8);
//    case BOX_TYPE_UINT16:   return sizeof(ui16);
//    case BOX_TYPE_UINT32:   return sizeof(ui32);
//    case BOX_TYPE_UINT64:   return sizeof(ui64);
//    case BOX_TYPE_FLOAT32:  return sizeof(fp32);
//    case BOX_TYPE_FLOAT64:  return sizeof(fp64);
//    case BOX_TYPE_NONE:
//        TBAG_FALLTHROUGH
//    default:
//        TBAG_INACCESSIBLE_BLOCK_ASSERT();
//        break;
//    }
    return Err::E_ENOSYS;
}

Err box_checked_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    if (!box_dim_is_equals(dest->dims, dest->rank, src->dims, src->rank)) {
        return Err::E_ILLARGS;
    }
    if (dest->total_byte != src->total_byte) {
        return Err::E_ILLARGS;
    }
    return box_memcpy(dest, src);
}

Err box_memcpy_async(box_data * dest, box_data const * src, void * user, box_memcpy_async_cb cb) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    return Err::E_ENOSYS;
}

Err box_get_index(box_data const * box, std::size_t * index) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return Err::E_UNKNOWN;
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

