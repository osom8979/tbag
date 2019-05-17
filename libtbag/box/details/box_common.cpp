/**
 * @file   box_common.cpp
 * @brief  box common implementation.
 * @author zer0
 * @date   2019-03-04
 */

#include <libtbag/box/details/box_common.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/memory/Memory.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

STATIC_ASSERT_CHECK_SIZEOF(btype, 2);
STATIC_ASSERT_CHECK_SIZEOF(bdev , 2);
STATIC_ASSERT_CHECK_SIZEOF(si8  , 1);
STATIC_ASSERT_CHECK_SIZEOF(si16 , 2);
STATIC_ASSERT_CHECK_SIZEOF(si32 , 4);
STATIC_ASSERT_CHECK_SIZEOF(si64 , 8);
STATIC_ASSERT_CHECK_SIZEOF(ui8  , 1);
STATIC_ASSERT_CHECK_SIZEOF(ui16 , 2);
STATIC_ASSERT_CHECK_SIZEOF(ui32 , 4);
STATIC_ASSERT_CHECK_SIZEOF(ui64 , 8);
STATIC_ASSERT_CHECK_SIZEOF(fp32 , 4);
STATIC_ASSERT_CHECK_SIZEOF(fp64 , 8);

STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_NONE   , BT_NONE   );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT8   , BT_INT8   );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT16  , BT_INT16  );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT32  , BT_INT32  );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT64  , BT_INT64  );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT8  , BT_UINT8  );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT16 , BT_UINT16 );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT32 , BT_UINT32 );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT64 , BT_UINT64 );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_FLOAT32, BT_FLOAT32);
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_FLOAT64, BT_FLOAT64);

bool box_support_type(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BOX_TYPE_NONE:    return false;
    case BOX_TYPE_INT8:    return true;
    case BOX_TYPE_INT16:   return true;
    case BOX_TYPE_INT32:   return true;
    case BOX_TYPE_INT64:   return true;
    case BOX_TYPE_UINT8:   return true;
    case BOX_TYPE_UINT16:  return true;
    case BOX_TYPE_UINT32:  return true;
    case BOX_TYPE_UINT64:  return true;
    case BOX_TYPE_FLOAT32: return true;
    case BOX_TYPE_FLOAT64: return true;
    default:               return false;
    }
    // clang-format on
}

bool box_support_device(bdev dev) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dev) {
    case BOX_DEVICE_NONE: return false;
    case BOX_DEVICE_CPU:  return true;
    case BOX_DEVICE_CUDA: return false;
    case BOX_DEVICE_CL:   return false;
    case BOX_DEVICE_GLSL: return false;
    case BOX_DEVICE_FBS:  return false;
    default:              return false;
    }
    // clang-format on
}

char const * const box_get_type_name(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BOX_TYPE_NONE:    return "NONE";
    case BOX_TYPE_INT8:    return "INT8";
    case BOX_TYPE_INT16:   return "INT16";
    case BOX_TYPE_INT32:   return "INT32";
    case BOX_TYPE_INT64:   return "INT64";
    case BOX_TYPE_UINT8:   return "UINT8";
    case BOX_TYPE_UINT16:  return "UINT16";
    case BOX_TYPE_UINT32:  return "UINT32";
    case BOX_TYPE_UINT64:  return "UINT64";
    case BOX_TYPE_FLOAT32: return "FLOAT32";
    case BOX_TYPE_FLOAT64: return "FLOAT64";
    default:               return "UNKNOWN";
    }
    // clang-format on
}

char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dev) {
    case BOX_DEVICE_CPU:  return "CPU";
    case BOX_DEVICE_CUDA: return "CUDA";
    case BOX_DEVICE_CL:   return "CL";
    case BOX_DEVICE_GLSL: return "GLSL";
    case BOX_DEVICE_FBS:  return "FBS";
    case BOX_DEVICE_NONE: return "NONE";
    default:              return "UNKNOWN";
    }
    // clang-format on
}

ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BOX_TYPE_NONE:    return 0;
    case BOX_TYPE_INT8:    return sizeof(si8);
    case BOX_TYPE_INT16:   return sizeof(si16);
    case BOX_TYPE_INT32:   return sizeof(si32);
    case BOX_TYPE_INT64:   return sizeof(si64);
    case BOX_TYPE_UINT8:   return sizeof(ui8);
    case BOX_TYPE_UINT16:  return sizeof(ui16);
    case BOX_TYPE_UINT32:  return sizeof(ui32);
    case BOX_TYPE_UINT64:  return sizeof(ui64);
    case BOX_TYPE_FLOAT32: return sizeof(fp32);
    case BOX_TYPE_FLOAT64: return sizeof(fp64);
    default:               return 0;
    }
    // clang-format on
}

void box_clear(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    box->type = BT_NONE;
    box->device = BOX_DEVICE_NONE;
    box->ext[0] = 0;
    box->ext[1] = 0;
    box->ext[2] = 0;
    box->ext[3] = 0;
    box->data = nullptr;
    box->total_data_byte = 0;
    box->size = 0;
    box->dims = nullptr;
    box->total_dims_byte = 0;
    box->rank = 0;
    box->info = nullptr;
    box->total_info_byte = 0;
    box->info_size = 0;
}

// clang-format off
void box_opaque_set(box_data * box, si8  v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, si16 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, si32 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, si64 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, ui8  v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, ui16 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, ui32 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_set(box_data * box, ui64 v) TBAG_NOEXCEPT { box->opaque = (void*)(std::intptr_t)v; }
void box_opaque_get(box_data const * box, si8  * v) TBAG_NOEXCEPT { *v = (si8 )(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, si16 * v) TBAG_NOEXCEPT { *v = (si16)(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, si32 * v) TBAG_NOEXCEPT { *v = (si32)(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, si64 * v) TBAG_NOEXCEPT { *v = (si64)(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, ui8  * v) TBAG_NOEXCEPT { *v = (ui8 )(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, ui16 * v) TBAG_NOEXCEPT { *v = (ui16)(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, ui32 * v) TBAG_NOEXCEPT { *v = (ui32)(std::intptr_t)box->opaque; }
void box_opaque_get(box_data const * box, ui64 * v) TBAG_NOEXCEPT { *v = (ui64)(std::intptr_t)box->opaque; }
// clang-format on

ui32 box_get_size_args(ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto const SIZE = box_get_size_vargs(rank, ap);
    va_end(ap);
    return SIZE;
}

ui32 box_get_size_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    va_list ap2;
    va_copy(ap2, ap);
    auto size = va_arg(ap2, ui32);
    for (ui32 i = 1; i < rank; ++i) {
        size *= va_arg(ap2, ui32);
    }
    va_end(ap2);
    return size;
}

ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    return (ui32*)tbMalloc(GET_RANK_TO_TOTAL_DIMS_BYTE(rank));
}

ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto * dims = box_dim_malloc_vargs(rank, ap);
    assert(dims != nullptr);
    va_end(ap);
    return dims;
}

ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    auto * dims = box_dim_malloc(rank);
    assert(dims != nullptr);
    box_dim_set_vargs(dims, rank, ap);
    return dims;
}

void box_dim_free(ui32 * dims) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    tbFree(dims);
}

void box_dim_set_args(ui32 * TBAG_RESTRICT dims, ui32 args_count, ...) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    va_list ap;
    va_start(ap, args_count);
    box_dim_set_vargs(dims, args_count, ap);
    va_end(ap);
}

void box_dim_set_vargs(ui32 * TBAG_RESTRICT dims, ui32 args_count, va_list ap) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    va_list ap2;
    va_copy(ap2, ap);
    for (ui32 i = 0; i < args_count; ++i) {
        dims[i] = va_arg(ap2, ui32);
    }
    va_end(ap2);
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

ui32 * box_dim_clone(ui32 const * src, ui32 rank) TBAG_NOEXCEPT
{
    assert(src != nullptr);
    assert(rank >= 1);
    return box_dim_clone_with_mem_size(src, rank, rank);
}

TBAG_API ui32 * box_dim_clone_with_mem_size(ui32 const * src, ui32 dims_size, ui32 rank) TBAG_NOEXCEPT
{
    assert(src != nullptr);
    assert(rank >= 1);
    assert(dims_size >= rank);
    ui32 * result = box_dim_malloc(dims_size);
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

bool box_dim_is_equals_args(ui32 const * dims1, ui32 rank1, ui32 rank2, ...) TBAG_NOEXCEPT
{
    va_list ap;
    va_start(ap, rank2);
    auto const EQUALS = box_dim_is_equals_vargs(dims1, rank1, rank2, ap);
    va_end(ap);
    return EQUALS;
}

bool box_dim_is_equals_vargs(ui32 const * dims1, ui32 rank1, ui32 rank2, va_list ap) TBAG_NOEXCEPT
{
    assert(dims1 != nullptr);
    if (rank1 != rank2) {
        return false;
    }
    assert(rank1 == rank2);

    va_list ap2;
    va_copy(ap2, ap);
    for (ui32 i = 0; i < rank1; ++i) {
        if (dims1[i] != va_arg(ap2, ui32)) {
            return false;
        }
    }
    va_end(ap2);
    return true;
}

ui32 box_dim_get_size(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    auto size = dims[0];
    for (auto i = 1; i < rank; ++i) {
        size *= dims[i];
    }
    return size;
}

ui32 box_dim_get_stride(ui32 const * dims, ui32 rank, ui32 dim_index) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    assert(dim_index < rank);

    ui32 stride = 1;
    for (auto i = dim_index+1; i < rank; ++i) {
        stride *= dims[i];
    }
    return stride;
}

ui32 box_dim_get_index_args(ui32 const * dims, ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto const INDEX = box_dim_get_index_vargs(dims, rank, ap);
    va_end(ap);
    return INDEX;
}

ui32 box_dim_get_index_vargs(ui32 const * dims, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);

    va_list ap2;
    va_copy(ap2, ap);
    ui32 index = 0;
    while (rank >= 2) {
        ++dims;
        --rank;
        index = (va_arg(ap2, ui32) + index) * (*dims);
    }
    assert(rank == 1);
    index += va_arg(ap2, ui32);
    va_end(ap2);
    return index;
}

char * box_info_malloc(ui32 info_size) TBAG_NOEXCEPT
{
    assert(info_size >= 1);
    return (char*)tbMalloc(GET_SIZE_TO_TOTAL_INFO_BYTE(info_size));
}

void box_info_free(char * info) TBAG_NOEXCEPT
{
    assert(info != nullptr);
    tbFree(info);
}

bool box_info_assign(char * dest, ui32 dest_size, char const * src, ui32 src_size) TBAG_NOEXCEPT
{
    assert(dest != nullptr);
    assert(dest_size >= 1);
    assert(src != nullptr);
    assert(src_size >= 1);
    if (dest_size < src_size) {
        return false;
    }
    memcpy(dest, src, src_size);
    return true;
}

bool box_info_assign(char * dest, ui32 dest_size, char const * src) TBAG_NOEXCEPT
{
    return box_info_assign(dest, dest_size, src, strlen(src));
}

void * box_data_ptr_offset(box_data * box, ui32 offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return (void*)(((ui8*)(box->data)) + (box_get_type_byte(box->type)*offset));
}

void const * box_data_ptr_offset(box_data const * box, ui32 offset) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return (void const *)(((ui8 const *)(box->data)) + (box_get_type_byte(box->type)*offset));
}

bool box_data_check_address(box_data const * box, void const * data) TBAG_NOEXCEPT
{
    auto const min = (std::intptr_t)box->data;
    auto const max = ((std::intptr_t)(box->data)) + (((box->size)-1) * box_get_type_byte(box->type));
    auto const pivot = (std::intptr_t)data;
    return (min <= COMPARE_AND(pivot) <= max);
}

bool box_cursor_index_check(int begin, int end, int step) TBAG_NOEXCEPT
{
    if (begin < end) {
        if (step <= 0) {
            return false;
        }
        assert(step > 0);
    } else {
        if (step >= 0) {
            return false;
        }
        assert(step < 0);
    }
    return (abs(end - begin) % abs(step) == 0);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                     int stride, int begin, int end, int step, int index) TBAG_NOEXCEPT
{
    assert(cursor != nullptr);
    assert(box != nullptr);
    assert(data != nullptr);
    assert(dim_index >= 0);
    assert(stride >= 1);
    assert(begin >= 0);
    assert(end >= 0);
    assert(index >= 0);
    assert(box->dims != nullptr);
    assert(box->rank >= 1);
    assert(dim_index < box->rank);
    assert(box_support_type(box->type));

    cursor->box = box;
    cursor->data = data;
    cursor->dim_index = dim_index;
    cursor->stride = stride;
    cursor->begin = begin;
    cursor->end = end;
    cursor->step = step;
    cursor->index = index;

    return box_cursor_index_check(begin, end, step);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                     int stride, int begin, int end, int step) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, data, dim_index, stride, begin, end, step, BOX_CURSOR_INIT_INDEX);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                     int stride, int begin, int end) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, data, dim_index, stride, begin, end, 1);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                     int stride, int begin) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(box->dims != nullptr);
    assert(box->rank >= 1);
    assert(dim_index >= 0);
    assert(dim_index < box->rank);
    return box_cursor_init(cursor, box, data, dim_index, stride, begin, box->dims[dim_index]);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index, int stride) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, data, dim_index, stride, 0);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(box->dims != nullptr);
    assert(box->rank >= 1);
    auto stride = box_dim_get_stride(box->dims, box->rank, dim_index);
    assert(stride >= 1);
    return box_cursor_init(cursor, box, data, dim_index, stride);
}

bool box_cursor_init(box_cursor * cursor, box_data * box, void * data) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, data, 0);
}

bool box_cursor_init(box_cursor * cursor, box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(box->data != nullptr);
    return box_cursor_init(cursor, box, box->data);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                         int stride, int begin, int end, int step, int index) TBAG_NOEXCEPT
{
    assert(parent_cursor != nullptr);
    assert(parent_cursor->box != nullptr);
    return box_cursor_init(cursor, parent_cursor->box, data, dim_index, stride, begin, end, step, index);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                         int stride, int begin, int end, int step) TBAG_NOEXCEPT
{
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index, stride, begin, end, step, BOX_CURSOR_INIT_INDEX);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                         int stride, int begin, int end) TBAG_NOEXCEPT
{
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index, stride, begin, end, 1);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                         int stride, int begin) TBAG_NOEXCEPT
{
    assert(parent_cursor != nullptr);
    auto * box = parent_cursor->box;
    assert(box != nullptr);
    assert(box->dims != nullptr);
    assert(box->rank >= 1);
    assert(dim_index >= 0);
    assert(dim_index < box->rank);
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index, stride, begin, box->dims[dim_index]);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                         int stride) TBAG_NOEXCEPT
{
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index, stride, 0);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index) TBAG_NOEXCEPT
{
    assert(parent_cursor != nullptr);
    auto * box = parent_cursor->box;
    assert(box != nullptr);
    assert(box->dims != nullptr);
    assert(box->rank >= 1);
    auto stride = box_dim_get_stride(box->dims, box->rank, dim_index);
    assert(stride >= 1);
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index, stride);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data) TBAG_NOEXCEPT
{
    assert(parent_cursor != nullptr);
    auto dim_index = parent_cursor->dim_index - 1;
    assert(dim_index >= 0);
    return box_cursor_init_sub(cursor, parent_cursor, data, dim_index);
}

bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor) TBAG_NOEXCEPT
{
    assert(parent_cursor != nullptr);
    assert(parent_cursor->data != nullptr);
    return box_cursor_init_sub(cursor, parent_cursor, parent_cursor->data);
}

bool box_cursor_next(box_cursor * cursor) TBAG_NOEXCEPT
{
    if (cursor->index == BOX_CURSOR_INIT_INDEX) {
        cursor->index = cursor->begin;
    } else {
        cursor->index += cursor->step;

        auto const next_offset_byte = cursor->stride * cursor->step * box_get_type_byte(cursor->box->type);
        cursor->data = (void*) (((ui8*)(cursor->data)) + next_offset_byte);
        if (!box_data_check_address(cursor->box, cursor->data)) {
            return false;
        }
    }
    return (cursor->index != cursor->end);
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

