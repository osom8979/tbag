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
#include <libtbag/box/details/box_dim.hpp>
#include <libtbag/box/details/box_cpu.hpp>
#include <libtbag/box/details/box_cuda.hpp>
#include <libtbag/box/details/box_fbs.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

static void * _box_malloc_backend(btype type, bdev device, ui32 total_byte) TBAG_NOEXCEPT
{
}

static void _box_free_backend(btype type, bdev device, void * ptr) TBAG_NOEXCEPT
{
}

Err box_malloc(box_data * box, btype type, bdev device, ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(box_support_type(type));
    assert(box_support_type(device));

    auto const TOTAL_BYTE = box_get_type_byte(type) * box_get_total_length(dims, rank);
    assert(TOTAL_BYTE >= 1);

    void * data;
    // @formatter:off
    switch (device) {
    case BOX_DEVICE_CPU:  data = box_cpu_malloc(TOTAL_BYTE);
    case BOX_DEVICE_CUDA: data = nullptr;
    case BOX_DEVICE_CL:   data = nullptr;
    case BOX_DEVICE_GLSL: data = nullptr;
    case BOX_DEVICE_FBS:  data = nullptr;
    case BOX_DEVICE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on

    box->type = type;
    box->device = device;
    box->data = data;
    box->total_byte = TOTAL_BYTE;
    box->dims = box_dim_malloc_copy(dims, rank);
    box->rank = rank;
    return Err::E_SUCCESS;
}

Err box_free(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);

    // @formatter:off
    switch (box->device) {
    case BOX_DEVICE_CPU:  box_cpu_free(box->data); break;
    case BOX_DEVICE_CUDA: break;
    case BOX_DEVICE_CL:   break;
    case BOX_DEVICE_GLSL: break;
    case BOX_DEVICE_FBS:  break;
    case BOX_DEVICE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on
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

