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

STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_NONE   , BT_NONE   );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_INT8   , BT_INT8   );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_INT16  , BT_INT16  );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_INT32  , BT_INT32  );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_INT64  , BT_INT64  );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_UINT8  , BT_UINT8  );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_UINT16 , BT_UINT16 );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_UINT32 , BT_UINT32 );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_UINT64 , BT_UINT64 );
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_FLOAT32, BT_FLOAT32);
STATIC_ASSERT_CHECK_IS_EQUALS(_BOX_TYPE_FLOAT64, BT_FLOAT64);

bool box_support_type(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BT_NONE:    return false;
    case BT_INT8:    return true;
    case BT_INT16:   return true;
    case BT_INT32:   return true;
    case BT_INT64:   return true;
    case BT_UINT8:   return true;
    case BT_UINT16:  return true;
    case BT_UINT32:  return true;
    case BT_UINT64:  return true;
    case BT_FLOAT32: return true;
    case BT_FLOAT64: return true;
    default:         return false;
    }
    // clang-format on
}

bool box_support_device(bdev dev) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dev) {
    case BD_NONE: return false;
    case BD_CPU:  return true;
    case BD_CUDA: return false;
    case BD_CL:   return false;
    default:      return false;
    }
    // clang-format on
}

char const * const box_get_type_name(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BT_NONE:    return "NONE";
    case BT_INT8:    return "INT8";
    case BT_INT16:   return "INT16";
    case BT_INT32:   return "INT32";
    case BT_INT64:   return "INT64";
    case BT_UINT8:   return "UINT8";
    case BT_UINT16:  return "UINT16";
    case BT_UINT32:  return "UINT32";
    case BT_UINT64:  return "UINT64";
    case BT_FLOAT32: return "FLOAT32";
    case BT_FLOAT64: return "FLOAT64";
    default:         return "UNKNOWN";
    }
    // clang-format on
}

char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dev) {
    case BD_CPU:  return "CPU";
    case BD_CUDA: return "CUDA";
    case BD_CL:   return "CL";
    case BD_NONE: return "NONE";
    default:      return "UNKNOWN";
    }
    // clang-format on
}

ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BT_NONE:    return 0;
    case BT_INT8:    return sizeof(si8);
    case BT_INT16:   return sizeof(si16);
    case BT_INT32:   return sizeof(si32);
    case BT_INT64:   return sizeof(si64);
    case BT_UINT8:   return sizeof(ui8);
    case BT_UINT16:  return sizeof(ui16);
    case BT_UINT32:  return sizeof(ui32);
    case BT_UINT64:  return sizeof(ui64);
    case BT_FLOAT32: return sizeof(fp32);
    case BT_FLOAT64: return sizeof(fp64);
    default:         return 0;
    }
    // clang-format on
}

void box_clear(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    box->type = BT_NONE;
    box->device = BD_NONE;
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

ui8 * box_info_malloc(ui32 info_size) TBAG_NOEXCEPT
{
    assert(info_size >= 1);
    return (ui8*)tbMalloc(GET_SIZE_TO_TOTAL_INFO_BYTE(info_size));
}

void box_info_free(ui8 * info) TBAG_NOEXCEPT
{
    assert(info != nullptr);
    tbFree(info);
}

bool box_info_assign(ui8 * dest, ui32 dest_size, ui8 const * src, ui32 src_size) TBAG_NOEXCEPT
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

bool box_info_assign_string(ui8 * dest, ui32 dest_size, char const * src) TBAG_NOEXCEPT
{
    return box_info_assign(dest, dest_size, (ui8 const *)src, strlen(src)*sizeof(char));
}

void * box_data_ptr_offset_raw(void * data, btype type, ui32 offset) TBAG_NOEXCEPT
{
    assert(data != nullptr);
    return (void*)(((ui8*)data) + (box_get_type_byte(type)*offset));
}

void const * box_data_ptr_offset_raw(void const * data, btype type, ui32 offset) TBAG_NOEXCEPT
{
    assert(data != nullptr);
    return (void const *)(((ui8 const *)data) + (box_get_type_byte(type)*offset));
}

bool box_data_check_address_raw(void const * data_begin, ui32 size, btype type, void const * check_data) TBAG_NOEXCEPT
{
    auto const min = (std::intptr_t)data_begin;
    auto const max = ((std::intptr_t)data_begin) + ((size-1) * box_get_type_byte(type));
    auto const pivot = (std::intptr_t)check_data;
    return (min <= COMPARE_AND(pivot) <= max);
}

int box_index_abs(int dim_size, int dim_index) TBAG_NOEXCEPT
{
    assert(dim_size >= 1);
    if (dim_index >= 0) {
        return dim_index;
    }
    return dim_size + dim_index;
}

bool box_index_check(int begin, int end, int step) TBAG_NOEXCEPT
{
    if (begin < end) {
        if (step <= 0) {
            return false;
        }
    } else {
        if (step >= 0) {
            return false;
        }
    }
    return true;
}

bool box_cursor_init(box_cursor * cursor, box_data const * box, void * data, ui32 dim_index,
                     int begin, int end, int step) TBAG_NOEXCEPT
{
    assert(cursor != nullptr);
    assert(box != nullptr);
    assert(data != nullptr);
    assert(box->rank >= 1);
    assert(step != 0);

    auto const dim_size = box->dims[dim_index];
    auto const begin_abs = box_index_abs(dim_size, begin);
    if (begin_abs < 0) {
        return false;
    }

    auto const end_abs = box_index_abs(dim_size, end);
    if (end_abs < 0) {
        return false;
    }

    if (!box_index_check(begin_abs, end_abs, step)) {
        return false;
    }

    auto const diff = end_abs - begin_abs;
    auto const last_one = (diff % step) ? 1 : 0;
    auto const exact_last_position = begin_abs + ((diff / step + last_one) * step);

    auto const stride = box_dim_get_stride(box->dims, box->rank, dim_index);
    auto const stride_byte = static_cast<int>(stride * box_get_type_byte(box->type));

    auto const integer_data_pointer = (std::intptr_t)data;
    cursor->begin = (void*)(integer_data_pointer + (stride_byte * begin_abs));
    cursor->end = (void*)(integer_data_pointer + (stride_byte * exact_last_position));
    cursor->stride_byte = stride_byte * step;
    return true;
}

bool box_cursor_init(box_cursor * cursor, box_data const * box, ui32 dim_index,
                     int begin, int end, int step) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box_cursor_init(cursor, box, box->data, dim_index, begin, end, step);
}

bool box_cursor_init(box_cursor * cursor, box_data const * box, ui32 dim_index, int begin, int end) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, dim_index, begin, end, 1);
}

bool box_cursor_init(box_cursor * cursor, box_data const * box, ui32 dim_index, int begin) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dim_index >= 0);
    assert(box->rank >= 1);
    assert(dim_index < box->rank);
    return box_cursor_init(cursor, box, dim_index, begin, box->dims[dim_index]);
}

bool box_cursor_init(box_cursor * cursor, box_data const * box, ui32 dim_index) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, dim_index, 0);
}

bool box_cursor_init(box_cursor * cursor, box_data const * box) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, box, 0);
}

bool box_cursor_init(box_cursor * cursor, box_cursor const * parent, box_data const * box, ui32 dim_index, int begin, int end, int step) TBAG_NOEXCEPT
{
    assert(parent != nullptr);
    return box_cursor_init(cursor, box, parent->begin, dim_index, begin, end, step);
}

bool box_cursor_init(box_cursor * cursor, box_cursor const * parent, box_data const * box, ui32 dim_index, int begin, int end) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, parent, box, dim_index, begin, end, 1);
}

bool box_cursor_init(box_cursor * cursor, box_cursor const * parent, box_data const * box, ui32 dim_index, int begin) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dim_index >= 0);
    assert(box->rank >= 1);
    assert(dim_index < box->rank);
    return box_cursor_init(cursor, parent, box, dim_index, begin, box->dims[dim_index]);
}

bool box_cursor_init(box_cursor * cursor, box_cursor const * parent, box_data const * box, ui32 dim_index) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, parent, box, dim_index, 0);
}

bool box_cursor_init(box_cursor * cursor, box_cursor const * parent, box_data const * box) TBAG_NOEXCEPT
{
    return box_cursor_init(cursor, parent, box, 0);
}

bool box_cursor_is_end(box_cursor const * cursor) TBAG_NOEXCEPT
{
    return cursor->begin != cursor->end;
}

bool box_cursor_next(box_cursor * cursor) TBAG_NOEXCEPT
{
    assert(cursor != nullptr);
    auto const integer_data_pointer = (std::intptr_t)cursor->begin;
    cursor->begin = (void*)(integer_data_pointer + cursor->stride_byte);
    return box_cursor_is_end(cursor);
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

