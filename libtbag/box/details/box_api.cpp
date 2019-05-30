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
#include <libtbag/box/details/box_cpu.hpp>
#include <libtbag/box/details/box_cuda.hpp>
#include <libtbag/box/details/box_fbs.hpp>

#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

void * box_data_malloc(bdev device, ui32 byte) TBAG_NOEXCEPT
{
    assert(box_support_device(device));
    assert(byte >= 1);

    // clang-format off
    switch (device) {
    case BD_CPU:  return box_cpu_malloc(byte);
    case BD_CUDA: /* TODO */ return nullptr;
    case BD_CL:   /* TODO */ return nullptr;
    case BD_GLSL: /* TODO */ return nullptr;
    case BD_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return nullptr;
    }
    // clang-format on
}

void * box_data_malloc2(btype type, bdev device, ui32 element_size) TBAG_NOEXCEPT
{
    assert(box_support_type(type));
    assert(box_support_device(device));
    assert(element_size >= 1);
    return box_data_malloc(device, box_get_type_byte(type) * element_size);
}

void box_data_free(bdev device, void * data) TBAG_NOEXCEPT
{
    assert(box_support_device(device));
    assert(data != nullptr);

    // clang-format off
    switch (device) {
    case BD_CPU:  box_cpu_free(data); break;
    case BD_CUDA: /* TODO */ break;
    case BD_CL:   /* TODO */ break;
    case BD_GLSL: /* TODO */ break;
    case BD_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

Err box_malloc_copy_dims(box_data * box, btype type, bdev device, ui64 const * ext, ui32 const * dims, ui32 dims_byte, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(CHECK_TOTAL_DIMS_BYTE(dims_byte));
    assert(dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(rank));
    assert(box_support_type(type));
    assert(box_support_device(device));

    auto * cloned_box_dims = box_dim_clone_with_mem_size(dims, GET_TOTAL_DIMS_BYTE_TO_RANK(dims_byte), rank);
    assert(cloned_box_dims != nullptr);
    auto const CODE = box_malloc_move_dims(box, type, device, ext, cloned_box_dims, dims_byte, rank);
    if (isFailure(CODE)) {
        box_dim_free(cloned_box_dims);
    }
    return CODE;
}

Err box_malloc_move_dims(box_data * box, btype type, bdev device, ui64 const * ext, ui32 * dims, ui32 dims_byte, ui32 rank) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(CHECK_TOTAL_DIMS_BYTE(dims_byte));
    assert(dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(rank));
    assert(box_support_type(type));
    assert(box_support_device(device));

    auto const SIZE = box_dim_get_size(dims, rank);
    assert(SIZE >= 1);

    void * data = box_data_malloc2(type, device, SIZE);
    if (data == nullptr) {
        return E_BADALLOC;
    }
    assert(data != nullptr);

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
    box->total_data_byte = box_get_type_byte(type) * SIZE;
    box->size = SIZE;
    box->dims = dims;
    box->total_dims_byte = dims_byte;
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
    auto const CODE = box_malloc_move_dims(box, type, device, ext, dims, GET_RANK_TO_TOTAL_DIMS_BYTE(rank), rank);
    if (isFailure(CODE)) {
        box_dim_free(dims);
    }
    return CODE;
}

Err box_free(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    if (box->data) {
        box_data_free(box->device, box->data);
    }
    if (box->dims) {
        box_dim_free(box->dims);
    }
    if (box->info) {
        box_info_free(box->info);
    }
    box_clear(box);
    return E_SUCCESS;
}

Err box_resize_args(box_data * box, btype type, bdev device, ui64 const * ext, ui32 rank, ...) TBAG_NOEXCEPT
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = box_resize_vargs(box, type, device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err box_resize_vargs(box_data * box, btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(rank >= 1);
    assert(box_support_type(type));
    assert(box_support_device(device));

    if (box->data == nullptr && box->dims == nullptr) {
        return box_malloc_vargs(box, type, device, ext, rank, ap);
    }

    ui64 resize_ext[BOX_EXTENSION_SIZE] = {0,};
    if (ext) {
        memcpy(resize_ext, ext, sizeof(ui64)*BOX_EXTENSION_SIZE);
    }
    if (box->device != device ||
            box->ext[0] != resize_ext[0] ||
            box->ext[1] != resize_ext[1] ||
            box->ext[2] != resize_ext[2] ||
            box->ext[3] != resize_ext[3]) {
        if (box->data) {
            box_data_free(box->device, box->data);
        }
        if (box->dims) {
            box_dim_free(box->dims);
        }
        return box_malloc_vargs(box, type, device, ext, rank, ap);
    }

    assert(box->device == device);
    assert(box->ext[0] == resize_ext[0]);
    assert(box->ext[1] == resize_ext[1]);
    assert(box->ext[2] == resize_ext[2]);
    assert(box->ext[3] == resize_ext[3]);

    if (box_dim_is_equals_vargs(box->dims, box->rank, rank, ap)) {
        return E_SUCCESS;
    }

    if (box->total_dims_byte < GET_RANK_TO_TOTAL_DIMS_BYTE(rank)) {
        if (box->dims) {
            box_dim_free(box->dims);
        }
        box->dims = box_dim_malloc_vargs(rank, ap);
        if (box->dims == nullptr) {
            box->total_dims_byte = 0;
            box->rank = 0;
            return E_BADALLOC;
        }
        box->total_dims_byte = GET_RANK_TO_TOTAL_DIMS_BYTE(rank);
        box->rank = rank;
    } else {
        box_dim_set_vargs(box->dims, rank, ap);
        box->rank = rank;
    }
    assert(box->total_dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(rank));
    assert(box->dims != nullptr);
    assert(box->rank == rank);
    assert(box_dim_is_equals_vargs(box->dims, box->rank, rank, ap));

    auto const SIZE = box_dim_get_size(box->dims, rank);
    assert(SIZE >= 1);
    auto const TOTAL_BYTE = box_get_type_byte(type) * SIZE;
    assert(TOTAL_BYTE >= 1);

    if (box->total_data_byte < TOTAL_BYTE) {
        if (box->data) {
            box_data_free(device, box->data);
        }
        box->data = box_data_malloc2(type, device, SIZE);
        if (box->data == nullptr) {
            box->total_data_byte = 0;
            box->size = 0;
            return E_BADALLOC;
        }
        box->total_data_byte = TOTAL_BYTE;
        box->size = SIZE;
    } else {
        box->size = SIZE;
    }
    box->type = type;
    assert(box->total_data_byte >= TOTAL_BYTE);
    assert(box->data != nullptr);
    assert(box->size == SIZE);
    return E_SUCCESS;
}

bool box_exists_data(box_data const * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box->data != nullptr && box->total_data_byte >= 1 && box->size >= 1;
}

bool box_exists_dims(box_data const * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box->dims != nullptr && box->total_dims_byte >= 1 && box->rank >= 1;
}

Err box_clone(box_data * dest, btype type, btype device, ui64 const * ext, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(dest->data == nullptr);
    auto const CODE = box_malloc_copy_dims(dest, type, device, ext, src->dims, src->total_dims_byte, src->rank);
    if (isFailure(CODE)) {
        return CODE;
    }
    return box_data_copy(dest, src);
}

Err box_clone(box_data * dest, btype device, ui64 const * ext, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    return box_clone(dest, src->type, device, ext, src);
}

Err box_clone(box_data * dest, btype type, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    return box_clone(dest, type, src->device, src->ext, src);
}

Err box_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    return box_clone(dest, src->type, src->device, src->ext, src);
}

Err box_data_set(box_data * box, void const * data, btype data_type, bdev data_device, ui64 const * ext, ui32 box_data_offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));

    if (box->device == BD_CPU && data_device == BD_CPU) {
        box_cpu_set(box_data_ptr_offset(box, box_data_offset), box->type, data, data_type);
        return E_SUCCESS;
    }
    return E_ENOSYS;
}

Err box_data_set_args(box_data * box, void const * data, btype data_type, bdev data_device, ui64 const * ext, ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));
    assert(rank >= 1);

    va_list ap;
    va_start(ap, rank);
    auto const CODE = box_data_set_vargs(box, data, data_type, data_device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err box_data_set_vargs(box_data * box, void const * data, btype data_type, bdev data_device, ui64 const * ext, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));
    assert(rank >= 1);
    return box_data_set(box, data, data_type, data_device, ext, box_dim_get_index_vargs(box->dims, rank, ap));
}

Err box_data_get(box_data const * box, void * data, btype data_type, bdev data_device, ui64 const * ext, ui32 box_data_offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));

    if (box->device == BD_CPU && data_device == BD_CPU) {
        box_cpu_set(data, data_type, box_data_ptr_offset(box, box_data_offset), box->type);
        return E_SUCCESS;
    }
    return E_ENOSYS;
}

Err box_data_get_args(box_data const * box, void * data, btype data_type, bdev data_device, ui64 const * ext, ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));
    assert(rank >= 1);

    va_list ap;
    va_start(ap, rank);
    auto const CODE = box_data_get_vargs(box, data, data_type, data_device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err box_data_get_vargs(box_data const * box, void * data, btype data_type, bdev data_device, ui64 const * ext, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));
    assert(rank >= 1);
    return box_data_get(box, data, data_type, data_device, ext, box_dim_get_index_vargs(box->dims, rank, ap));
}

Err box_data_copy(box_data * box, void const * data, btype data_type, bdev data_device, ui64 const * ext, ui32 size) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box->data != data);
    assert(box_support_type(data_type));
    assert(box_support_device(data_device));
    assert(size >= 1);

    if (box->size < size) {
        return E_ILLARGS;
    }

    if (box->device == BD_CPU && data_device == BD_CPU) {
        if (box->type == data_type) {
            box_cpu_memcpy(box->data, data, box_get_type_byte(data_type)*size);
        } else {
            box_cpu_element_copy(box->data, box->type, data, data_type, size);
        }
        return E_SUCCESS;
    }
    return E_ENOSYS;
}

Err box_data_copy(box_data * dest, box_data const * src, ui32 size) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    assert(size >= 1);

    if (dest->size < size) {
        return E_ILLARGS;
    }
    if (src->size < size) {
        return E_ILLARGS;
    }
    if (dest == src) {
        return E_SUCCESS;
    }

    assert(dest != src);
    return box_data_copy(dest, src->data, src->type, src->device, src->ext, size);
}

Err box_data_copy(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(src != nullptr);
    return box_data_copy(dest, src, src->size);
}

void * box_data_ptr_offset(box_data * box, ui32 offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box_data_ptr_offset_raw(box->data, box->type, offset);
}

void const * box_data_ptr_offset(box_data const * box, ui32 offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box_data_ptr_offset_raw((void const *)box->data, box->type, offset);
}

bool box_data_check_address(box_data const * box, void const * data) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(data != nullptr);
    return box_data_check_address_raw(box->data, box->size, box->type, data);
}

bool box_info_checked_assign(box_data * dest, char const * src, ui32 src_size) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    if (src == nullptr || src_size == 0) {
        if (dest->info && dest->total_info_byte >= 1) {
            dest->info[0] = '\0';
        }
        dest->info_size = 0;
        return true;
    }

    if (dest->info != nullptr && dest->total_info_byte < src_size) {
        box_info_free(dest->info);
        dest->info = nullptr;
        dest->total_info_byte = 0;
        dest->size = 0;
    }
    if (dest->info == nullptr) {
        dest->info = box_info_malloc(src_size);
        assert(dest->info != nullptr);
        dest->total_info_byte = GET_SIZE_TO_TOTAL_INFO_BYTE(src_size);
        assert(dest->total_info_byte >= 1);
    }

    assert(dest->info != nullptr);
    assert(dest->total_info_byte >= GET_SIZE_TO_TOTAL_INFO_BYTE(src_size));
    auto const CODE = box_info_assign(dest->info, GET_TOTAL_INFO_BYTE_TO_SIZE(dest->total_info_byte), src, src_size);
    assert(CODE);
    dest->info_size = src_size;
    return true;
}

bool box_info_checked_assign(box_data * dest, box_data const * src) TBAG_NOEXCEPT
{
    return box_info_checked_assign(dest, src->info, src->info_size);
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

