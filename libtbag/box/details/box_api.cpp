/**
 * @file   box_api.cpp
 * @brief  C-based box method implementation.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
 * @date   2019-03-14 (Rename: libtbag/box/details/box -> libtbag/box/details/box_api)
 */

#include <libtbag/box/details/box_api.hpp>
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

Err box_malloc_copy_dims(box_data * box, btype type, bdev device, ui64 const * ext, ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(box_support_type(type));
    assert(box_support_device(device));

    auto * cloned_box_dims = box_dim_clone(dims, rank);
    assert(cloned_box_dims != nullptr);
    return box_malloc_move_dims(box, type, device, ext, cloned_box_dims, rank);
}

Err box_malloc_move_dims(box_data * box, btype type, bdev device, ui64 const * ext, ui32 * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(box_support_type(type));
    assert(box_support_device(device));

    auto const TOTAL_BYTE = box_get_type_byte(type) * box_dim_get_size(dims, rank);
    assert(TOTAL_BYTE >= 1);

    void * data;
    // @formatter:off
    switch (device) {
    case BOX_DEVICE_CPU:  data = box_cpu_malloc(TOTAL_BYTE); break;
    case BOX_DEVICE_CUDA: data = nullptr; break;
    case BOX_DEVICE_CL:   data = nullptr; break;
    case BOX_DEVICE_GLSL: data = nullptr; break;
    case BOX_DEVICE_FBS:  data = nullptr; break;
    case BOX_DEVICE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_ILLARGS;
    }
    // @formatter:on

    box->type = type;
    box->device = device;
    if (ext != nullptr) {
        box->ext[0] = ext[0];
        box->ext[1] = ext[1];
        box->ext[2] = ext[2];
        box->ext[3] = ext[3];
    } else {
        box->ext[0] = 0;
        box->ext[1] = 0;
        box->ext[2] = 0;
        box->ext[3] = 0;
    }
    box->data = data;
    box->total_byte = TOTAL_BYTE;
    box->dims = dims;
    box->rank = rank;
    return E_SUCCESS;
}

Err box_malloc_args(box_data * box, btype type, bdev device, ui64 const * ext, ui32 rank, ...) TBAG_NOEXCEPT
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = box_malloc_vargs(box, type, device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err box_malloc_vargs(box_data * box, btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(rank >= 1);
    assert(box_support_type(type));
    assert(box_support_device(device));

    auto * dims = box_dim_malloc_vargs(rank, ap);
    assert(dims != nullptr);
    return box_malloc_move_dims(box, type, device, ext, dims, rank);
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
    return E_SUCCESS;
}

bool box_exists_data(box_data const * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box->data != nullptr;
}

Err box_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    auto const CODE = box_malloc_copy_dims(dest, src->type, src->device, src->ext, src->dims, src->rank);
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

Err box_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(dest != src);
    assert(dest->total_byte == src->total_byte);
    if (dest->device == BOX_DEVICE_CPU && src->device == BOX_DEVICE_CPU) {
        box_cpu_memcpy(dest->data, src->data, src->total_byte);
        return E_SUCCESS;
    }
    return E_ENOSYS;
}

Err box_checked_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(dest != src);
    if (!box_dim_is_equals(dest->dims, dest->rank, src->dims, src->rank)) {
        return E_ILLARGS;
    }
    if (dest->total_byte != src->total_byte) {
        return E_ILLARGS;
    }
    return box_memcpy(dest, src);
}

Err box_memcpy_async(box_data * dest, box_data const * src, void * user, box_memcpy_async_cb cb) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    return E_ENOSYS;
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

