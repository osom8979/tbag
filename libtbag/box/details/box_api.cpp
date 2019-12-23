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
#include <libtbag/Type.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <type_traits>

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

STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_NONE   , BT_NONE   );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_INT8   , BT_INT8   );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_INT16  , BT_INT16  );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_INT32  , BT_INT32  );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_INT64  , BT_INT64  );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_UINT8  , BT_UINT8  );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_UINT16 , BT_UINT16 );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_UINT32 , BT_UINT32 );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_UINT64 , BT_UINT64 );
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_FLOAT32, BT_FLOAT32);
STATIC_ASSERT_CHECK_IS_EQUALS(TBAG_BOX_TYPE_FLOAT64, BT_FLOAT64);

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

char const * box_get_type_name(btype type) TBAG_NOEXCEPT
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

char const * box_get_device_name(bdev dev) TBAG_NOEXCEPT
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

bool box_ext_is_equals(ui64 const * ext1, ui64 const * ext2) TBAG_NOEXCEPT
{
    for (int i = 0; i < TBAG_BOX_EXT_SIZE; ++i) {
        if (ext1[i] != ext2[i]) {
            return false;
        }
    }
    return true;
}

ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT
{
    if (rank == 0) {
        return nullptr;
    }
    assert(rank >= 1);
    return (ui32*)tbMalloc(GET_RANK_TO_TOTAL_DIMS_BYTE(rank));
}

ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT
{
    if (rank == 0) {
        return nullptr;
    }
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto * result = box_dim_malloc_vargs(rank, ap);
    assert(result != nullptr);
    va_end(ap);
    return result;
}

ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    if (rank == 0) {
        return nullptr;
    }
    assert(rank >= 1);
    auto * result = box_dim_malloc(rank);
    assert(result != nullptr);
    box_dim_set_vargs(result, rank, ap);
    return result;
}

ui32 * box_dim_malloc_dims(ui32 rank, ui32 const * dims) TBAG_NOEXCEPT
{
    if (rank == 0) {
        return nullptr;
    }
    assert(rank >= 1);
    auto * result = box_dim_malloc(rank);
    assert(dims != nullptr);
    box_dim_set_dims(result, rank, dims);
    return result;
}

void box_dim_free(ui32 * dims) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    tbFree(dims);
}

void box_dim_set_args(ui32 * TBAG_RESTRICT result, ui32 args_count, ...) TBAG_NOEXCEPT
{
    assert(result != nullptr);
    va_list ap;
    va_start(ap, args_count);
    box_dim_set_vargs(result, args_count, ap);
    va_end(ap);
}

void box_dim_set_vargs(ui32 * TBAG_RESTRICT result, ui32 args_count, va_list ap) TBAG_NOEXCEPT
{
    assert(result != nullptr);
    va_list ap2;
    va_copy(ap2, ap);
    for (ui32 i = 0; i < args_count; ++i) {
        result[i] = va_arg(ap2, ui32);
    }
    va_end(ap2);
}

void box_dim_set_dims(ui32 * TBAG_RESTRICT result, ui32 args_count, ui32 const * dims) TBAG_NOEXCEPT
{
    assert(result != nullptr);
    va_list ap2;
    for (ui32 i = 0; i < args_count; ++i) {
        result[i] = dims[i];
    }
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
    return box_dim_clone_with_alloc_size(src, rank, rank);
}

ui32 * box_dim_clone_with_alloc_size(ui32 const * src, ui32 alloc_size, ui32 rank) TBAG_NOEXCEPT
{
    assert(src != nullptr);
    assert(alloc_size >= 1);
    assert(rank >= 1);
    assert(alloc_size >= rank);
    ui32 * result = box_dim_malloc(alloc_size);
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
    auto const is_equals = box_dim_is_equals_vargs(dims1, rank1, rank2, ap);
    va_end(ap);
    return is_equals;
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

ui32 box_dim_get_total_size(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    auto size = dims[0];
    for (auto i = 1; i < rank; ++i) {
        size *= dims[i];
    }
    return size;
}

ui32 box_dim_get_total_size_args(ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto const size = box_dim_get_total_size_vargs(rank, ap);
    va_end(ap);
    return size;
}

ui32 box_dim_get_total_size_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT
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

ui32 box_dim_get_offset_args(ui32 const * dims, ui32 rank, ...) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    va_list ap;
    va_start(ap, rank);
    auto const offset = box_dim_get_offset_vargs(dims, rank, ap);
    va_end(ap);
    return offset;
}

ui32 box_dim_get_offset_vargs(ui32 const * dims, ui32 rank, va_list ap) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    va_list ap2;
    va_copy(ap2, ap);
    ui32 offset = 0;
    while (rank >= 2) {
        ++dims;
        --rank;
        offset = (va_arg(ap2, ui32) + offset) * (*dims);
    }
    assert(rank == 1);
    offset += va_arg(ap2, ui32);
    va_end(ap2);
    return offset;
}

ui32 box_dim_get_offset_dims(ui32 const * dims, ui32 rank, ui32 const * indexes) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    ui32 offset = 0;
    while (rank >= 2) {
        ++dims;
        --rank;
        offset = (*indexes + offset) * (*dims);
        ++indexes;
    }
    assert(rank == 1);
    offset += *indexes;
    return offset;
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

bool box_info_assign_buffer(ui8 * dest, ui32 dest_size, ui8 const * src, ui32 src_size) TBAG_NOEXCEPT
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
    return box_info_assign_buffer(dest, dest_size, (ui8 const *)src, strlen(src)*sizeof(char));
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
    assert(data_begin != nullptr);
    assert(size >= 1);
    assert(box_support_type(type));
    assert(check_data != nullptr);
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
static void _box_cpu_element_copy_impl(itype begin, itype end, otype output) TBAG_NOEXCEPT
{
    for (; begin != end; ++begin, (void)++output) {
        *output = static_cast<typename std::remove_pointer<otype>::type>(*begin);
    }
}

template <typename itype>
static void _box_cpu_element_copy_impl(itype src, ui32 size, void * dest, btype dest_type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (dest_type) {
    case BT_INT8:    _box_cpu_element_copy_impl(src, src + size, (si8  *)dest); break;
    case BT_INT16:   _box_cpu_element_copy_impl(src, src + size, (si16 *)dest); break;
    case BT_INT32:   _box_cpu_element_copy_impl(src, src + size, (si32 *)dest); break;
    case BT_INT64:   _box_cpu_element_copy_impl(src, src + size, (si64 *)dest); break;
    case BT_UINT8:   _box_cpu_element_copy_impl(src, src + size, (ui8  *)dest); break;
    case BT_UINT16:  _box_cpu_element_copy_impl(src, src + size, (ui16 *)dest); break;
    case BT_UINT32:  _box_cpu_element_copy_impl(src, src + size, (ui32 *)dest); break;
    case BT_UINT64:  _box_cpu_element_copy_impl(src, src + size, (ui64 *)dest); break;
    case BT_FLOAT32: _box_cpu_element_copy_impl(src, src + size, (fp32 *)dest); break;
    case BT_FLOAT64: _box_cpu_element_copy_impl(src, src + size, (fp64 *)dest); break;
    case BT_NONE:
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
    case BT_INT8:    _box_cpu_element_copy_impl((si8  *)src, size, dest, dest_type); break;
    case BT_INT16:   _box_cpu_element_copy_impl((si16 *)src, size, dest, dest_type); break;
    case BT_INT32:   _box_cpu_element_copy_impl((si32 *)src, size, dest, dest_type); break;
    case BT_INT64:   _box_cpu_element_copy_impl((si64 *)src, size, dest, dest_type); break;
    case BT_UINT8:   _box_cpu_element_copy_impl((ui8  *)src, size, dest, dest_type); break;
    case BT_UINT16:  _box_cpu_element_copy_impl((ui16 *)src, size, dest, dest_type); break;
    case BT_UINT32:  _box_cpu_element_copy_impl((ui32 *)src, size, dest, dest_type); break;
    case BT_UINT64:  _box_cpu_element_copy_impl((ui64 *)src, size, dest, dest_type); break;
    case BT_FLOAT32: _box_cpu_element_copy_impl((fp32 *)src, size, dest, dest_type); break;
    case BT_FLOAT64: _box_cpu_element_copy_impl((fp64 *)src, size, dest, dest_type); break;
    case BT_NONE:
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
    case BT_INT8:    _box_cpu_set_impl(in, (si8  *)out); break;
    case BT_INT16:   _box_cpu_set_impl(in, (si16 *)out); break;
    case BT_INT32:   _box_cpu_set_impl(in, (si32 *)out); break;
    case BT_INT64:   _box_cpu_set_impl(in, (si64 *)out); break;
    case BT_UINT8:   _box_cpu_set_impl(in, (ui8  *)out); break;
    case BT_UINT16:  _box_cpu_set_impl(in, (ui16 *)out); break;
    case BT_UINT32:  _box_cpu_set_impl(in, (ui32 *)out); break;
    case BT_UINT64:  _box_cpu_set_impl(in, (ui64 *)out); break;
    case BT_FLOAT32: _box_cpu_set_impl(in, (fp32 *)out); break;
    case BT_FLOAT64: _box_cpu_set_impl(in, (fp64 *)out); break;
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
    case BT_INT8:    _box_cpu_set_impl((si8  *)src, dest, dest_type); break;
    case BT_INT16:   _box_cpu_set_impl((si16 *)src, dest, dest_type); break;
    case BT_INT32:   _box_cpu_set_impl((si32 *)src, dest, dest_type); break;
    case BT_INT64:   _box_cpu_set_impl((si64 *)src, dest, dest_type); break;
    case BT_UINT8:   _box_cpu_set_impl((ui8  *)src, dest, dest_type); break;
    case BT_UINT16:  _box_cpu_set_impl((ui16 *)src, dest, dest_type); break;
    case BT_UINT32:  _box_cpu_set_impl((ui32 *)src, dest, dest_type); break;
    case BT_UINT64:  _box_cpu_set_impl((ui64 *)src, dest, dest_type); break;
    case BT_FLOAT32: _box_cpu_set_impl((fp32 *)src, dest, dest_type); break;
    case BT_FLOAT64: _box_cpu_set_impl((fp64 *)src, dest, dest_type); break;
    case BT_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // clang-format on
}

void * box_data_malloc(bdev device, ui32 byte) TBAG_NOEXCEPT
{
    assert(box_support_device(device));
    assert(byte >= 1);
    switch (device) {
    case BD_CPU:
        return box_cpu_malloc(byte);
    case BD_CUDA:
        // TODO
        return nullptr;
    case BD_CL:
        // TODO
        return nullptr;
    case BD_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return nullptr;
    }
}

void * box_data_malloc(btype type, bdev device, ui32 element_size) TBAG_NOEXCEPT
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
    switch (device) {
    case BD_CPU:
        box_cpu_free(data);
        break;
    case BD_CUDA:
        // TODO
        break;
    case BD_CL:
        // TODO
        break;
    case BD_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
}

// -----------------------
// box_data implementation
// -----------------------

box_data::box_data() TBAG_NOEXCEPT
{
    clear();
    clear_opaque();
}

box_data::box_data(box_data && obj) TBAG_NOEXCEPT : box_data()
{
    swap(obj);
}

box_data::~box_data()
{
    release();
}

box_data & box_data::operator =(box_data && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void box_data::swap(box_data & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(type, obj.type);
        std::swap(device, obj.device);
        std::swap(ext[0], obj.ext[0]);
        std::swap(ext[1], obj.ext[1]);
        std::swap(ext[2], obj.ext[2]);
        std::swap(ext[3], obj.ext[3]);
        std::swap(data, obj.data);
        std::swap(total_data_byte, obj.total_data_byte);
        std::swap(size, obj.size);
        std::swap(dims, obj.dims);
        std::swap(total_dims_byte, obj.total_dims_byte);
        std::swap(rank, obj.rank);
        std::swap(info, obj.info);
        std::swap(total_info_byte, obj.total_info_byte);
        std::swap(info_size, obj.info_size);
        std::swap(opaque, obj.opaque);
        std::swap(opaque_deleter, obj.opaque_deleter);
    }
}

void box_data::release()
{
    if (data) {
        box_data_free(device, data);
    }
    if (dims) {
        box_dim_free(dims);
    }
    if (info) {
        box_info_free(info);
    }
    if (opaque.pointer && opaque_deleter) {
        opaque_deleter(opaque.pointer);
    }
    clear();
}

void box_data::clear() TBAG_NOEXCEPT
{
    type = BT_NONE;
    device = BD_NONE;
    ext[0] = 0;
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;
    data = nullptr;
    total_data_byte = 0;
    size = 0;
    dims = nullptr;
    total_dims_byte = 0;
    rank = 0;
    info = nullptr;
    total_info_byte = 0;
    info_size = 0;
}

void box_data::clear_opaque() TBAG_NOEXCEPT
{
    memset(&opaque, 0x00, sizeof(opaque));
    opaque_deleter = nullptr;
}

Err box_data::alloc_args(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ...)
{
    va_list ap;
    va_start(ap, src_rank);
    auto const code = alloc_vargs(src_type, src_device, src_ext, src_rank, ap);
    va_end(ap);
    return code;
}

Err box_data::alloc_vargs(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, va_list ap)
{
    assert(src_rank >= 1);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));

    auto * allocated_dims = box_dim_malloc_vargs(src_rank, ap);
    assert(allocated_dims != nullptr);

    auto const src_dims_byte = GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank);
    auto const code = alloc_dims_move(src_type, src_device, src_ext, allocated_dims, src_dims_byte, src_rank);
    if (isFailure(code)) {
        box_dim_free(allocated_dims);
    }
    return code;
}

Err box_data::alloc_dims_copy(btype src_type, bdev src_device, ui64 const * src_ext,
                              ui32 const * src_dims, ui32 src_dims_byte, ui32 src_rank)
{
    assert(src_dims != nullptr);
    assert(src_rank >= 1);
    assert(CHECK_TOTAL_DIMS_BYTE(src_dims_byte));
    assert(src_dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank));
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));

    auto const alloc_size = GET_TOTAL_DIMS_BYTE_TO_RANK(src_dims_byte);
    auto * cloned_box_dims = box_dim_clone_with_alloc_size(src_dims, alloc_size, src_rank);
    assert(cloned_box_dims != nullptr);

    auto const code = alloc_dims_move(src_type, src_device, src_ext, cloned_box_dims, src_dims_byte, src_rank);
    if (isFailure(code)) {
        box_dim_free(cloned_box_dims);
    }
    return code;
}

Err box_data::alloc_dims_move(btype src_type, bdev src_device, ui64 const * src_ext,
                              ui32 * src_dims, ui32 src_dims_byte, ui32 src_rank)
{
    assert(src_dims != nullptr);
    assert(src_rank >= 1);
    assert(CHECK_TOTAL_DIMS_BYTE(src_dims_byte));
    assert(src_dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank));
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));

    auto const total_elem_size = box_dim_get_total_size(src_dims, src_rank);
    assert(total_elem_size >= 1);

    void * allocated_data = box_data_malloc(src_type, src_device, total_elem_size);
    if (allocated_data == nullptr) {
        return E_BADALLOC;
    }
    assert(allocated_data != nullptr);

    type = src_type;
    device = src_device;

    if (src_ext != nullptr) {
        ext[0] = src_ext[0];
        ext[1] = src_ext[1];
        ext[2] = src_ext[2];
        ext[3] = src_ext[3];
    } else {
        ext[0] = 0;
        ext[1] = 0;
        ext[2] = 0;
        ext[3] = 0;
    }

    data = allocated_data;
    total_data_byte = box_get_type_byte(src_type) * total_elem_size;
    size = total_elem_size;

    dims = src_dims;
    total_dims_byte = src_dims_byte;
    rank = src_rank;

    return E_SUCCESS;
}

bool box_data::exists_data() const TBAG_NOEXCEPT
{
    return data != nullptr && total_data_byte >= 1 && size >= 1;
}

bool box_data::exists_dims() const TBAG_NOEXCEPT
{
    return dims != nullptr && total_dims_byte >= 1 && rank >= 1;
}

bool box_data::exists_info() const TBAG_NOEXCEPT
{
    return info != nullptr && total_info_byte >= 1 && info_size >= 1;
}

void box_data::set_opaque(void * v) TBAG_NOEXCEPT { opaque.pointer = v; }
void box_data::set_opaque(si8  v) TBAG_NOEXCEPT { opaque.data_si8  = v; }
void box_data::set_opaque(si16 v) TBAG_NOEXCEPT { opaque.data_si16 = v; }
void box_data::set_opaque(si32 v) TBAG_NOEXCEPT { opaque.data_si32 = v; }
void box_data::set_opaque(si64 v) TBAG_NOEXCEPT { opaque.data_si64 = v; }
void box_data::set_opaque(ui8  v) TBAG_NOEXCEPT { opaque.data_ui8  = v; }
void box_data::set_opaque(ui16 v) TBAG_NOEXCEPT { opaque.data_ui16 = v; }
void box_data::set_opaque(ui32 v) TBAG_NOEXCEPT { opaque.data_ui32 = v; }
void box_data::set_opaque(ui64 v) TBAG_NOEXCEPT { opaque.data_ui64 = v; }
void box_data::set_opaque(fp32 v) TBAG_NOEXCEPT { opaque.data_fp32 = v; }
void box_data::set_opaque(fp64 v) TBAG_NOEXCEPT { opaque.data_fp64 = v; }

void box_data::get_opaque(void ** v) const TBAG_NOEXCEPT { *v = opaque.pointer; }
void box_data::get_opaque(si8  * v) const TBAG_NOEXCEPT { *v = opaque.data_si8 ; }
void box_data::get_opaque(si16 * v) const TBAG_NOEXCEPT { *v = opaque.data_si16; }
void box_data::get_opaque(si32 * v) const TBAG_NOEXCEPT { *v = opaque.data_si32; }
void box_data::get_opaque(si64 * v) const TBAG_NOEXCEPT { *v = opaque.data_si64; }
void box_data::get_opaque(ui8  * v) const TBAG_NOEXCEPT { *v = opaque.data_ui8 ; }
void box_data::get_opaque(ui16 * v) const TBAG_NOEXCEPT { *v = opaque.data_ui16; }
void box_data::get_opaque(ui32 * v) const TBAG_NOEXCEPT { *v = opaque.data_ui32; }
void box_data::get_opaque(ui64 * v) const TBAG_NOEXCEPT { *v = opaque.data_ui64; }
void box_data::get_opaque(fp32 * v) const TBAG_NOEXCEPT { *v = opaque.data_fp32; }
void box_data::get_opaque(fp64 * v) const TBAG_NOEXCEPT { *v = opaque.data_fp64; }

void * box_data::get_opaque_pointer() const TBAG_NOEXCEPT { return opaque.pointer; }
si8  box_data::get_opaque_si8 () const TBAG_NOEXCEPT { return opaque.data_si8 ; }
si16 box_data::get_opaque_si16() const TBAG_NOEXCEPT { return opaque.data_si16; }
si32 box_data::get_opaque_si32() const TBAG_NOEXCEPT { return opaque.data_si32; }
si64 box_data::get_opaque_si64() const TBAG_NOEXCEPT { return opaque.data_si64; }
ui8  box_data::get_opaque_ui8 () const TBAG_NOEXCEPT { return opaque.data_ui8 ; }
ui16 box_data::get_opaque_ui16() const TBAG_NOEXCEPT { return opaque.data_ui16; }
ui32 box_data::get_opaque_ui32() const TBAG_NOEXCEPT { return opaque.data_ui32; }
ui64 box_data::get_opaque_ui64() const TBAG_NOEXCEPT { return opaque.data_ui64; }
fp32 box_data::get_opaque_fp32() const TBAG_NOEXCEPT { return opaque.data_fp32; }
fp64 box_data::get_opaque_fp64() const TBAG_NOEXCEPT { return opaque.data_fp64; }

ui32 box_data::get_dims_total_size() const TBAG_NOEXCEPT
{
    if (rank == 0) {
        return 0;
    }
    assert(rank >= 1);
    assert(dims != nullptr);
    return box_dim_get_total_size(dims, rank);
}

ErrPair<box_cursor> box_data::init_cursor(void * data_pointer, ui32 dim_index, int begin_index, int end_index, int step_index) TBAG_NOEXCEPT
{
    assert(data_pointer != nullptr);
    assert(rank >= 1);

    if (step_index == 0) {
        return E_ILLARGS;
    }

    auto const dim_size = dims[dim_index];
    auto const begin_abs = box_index_abs(dim_size, begin_index);
    if (begin_abs < 0) {
        return E_INDEX;
    }

    auto const end_abs = box_index_abs(dim_size, end_index);
    if (end_abs < 0) {
        return E_INDEX;
    }

    if (!box_index_check(begin_abs, end_abs, step_index)) {
        return E_INDEX;
    }

    auto const diff = end_abs - begin_abs;
    auto const last_one = (diff % step_index) ? 1 : 0;
    auto const exact_last_position = begin_abs + ((diff / step_index + last_one) * step_index);

    auto const stride = box_dim_get_stride(dims, rank, dim_index);
    auto const stride_byte = static_cast<int>(stride * box_get_type_byte(type));

    ErrPair<box_cursor> result;
    result.code = E_SUCCESS;
    result.value.box = this;
    result.value.begin = (void*)(((std::intptr_t)data_pointer) + (stride_byte * begin_abs));
    result.value.end = (void*)(((std::intptr_t)data_pointer) + (stride_byte * exact_last_position));
    result.value.stride_byte = stride_byte * step_index;
    return result;
}

ErrPair<box_cursor> box_data::init_cursor(ui32 dim_index, int begin_index, int end_index, int step_index) TBAG_NOEXCEPT
{
    return init_cursor(data, dim_index, begin_index, end_index, step_index);
}

ErrPair<box_cursor> box_data::init_cursor(ui32 dim_index, int begin_index, int end_index) TBAG_NOEXCEPT
{
    return init_cursor(dim_index, begin_index, end_index, 1);
}

ErrPair<box_cursor> box_data::init_cursor(ui32 dim_index, int begin_index) TBAG_NOEXCEPT
{
    assert(rank >= 1);
    assert(dim_index < rank);
    return init_cursor(dim_index, begin_index, dims[dim_index]);
}

ErrPair<box_cursor> box_data::init_cursor(ui32 dim_index) TBAG_NOEXCEPT
{
    return init_cursor(dim_index, 0);
}

ErrPair<box_cursor> box_data::init_cursor() TBAG_NOEXCEPT
{
    return init_cursor(0);
}

Err box_data::resize_args(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ...)
{
    va_list ap;
    va_start(ap, src_rank);
    auto const code = resize_vargs(src_type, src_device, src_ext, src_rank, ap);
    va_end(ap);
    return code;
}

Err box_data::resize_vargs(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, va_list ap)
{
    if (src_rank <= TBAG_BOX_TEMP_DIM_STACK_SIZE) {
        ui32 temp_dims[TBAG_BOX_TEMP_DIM_STACK_SIZE];
        box_dim_set_vargs(temp_dims, src_rank, ap);
        return resize_dims(src_type, src_device, src_ext, src_rank, temp_dims);
    } else {
        assert(src_rank > TBAG_BOX_TEMP_DIM_STACK_SIZE);
        ui32 * temp_dims = box_dim_malloc(src_rank);
        assert(temp_dims != nullptr);
        box_dim_set_vargs(temp_dims, src_rank, ap);
        auto const code = resize_dims(src_type, src_device, src_ext, src_rank, temp_dims);
        box_dim_free(temp_dims);
        return code;
    }
}

Err box_data::resize_dims(btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_rank, ui32 const * src_dims)
{
    if (src_rank == 0) {
        type = src_type;
        device = src_device;
        if (src_ext != nullptr) {
            ext[0] = src_ext[0];
            ext[1] = src_ext[1];
            ext[2] = src_ext[2];
            ext[3] = src_ext[3];
        } else {
            ext[0] = 0;
            ext[1] = 0;
            ext[2] = 0;
            ext[3] = 0;
        }
        size = 0;
        rank = 0;
        return E_SUCCESS;
    }

    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    if (data == nullptr && dims == nullptr) {
        auto const src_dims_byte = GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank);
        return alloc_dims_copy(src_type, src_device, src_ext, src_dims, src_dims_byte, src_rank);
    }

    ui64 resize_ext[TBAG_BOX_EXT_SIZE] = {0,};
    if (src_ext) {
        memcpy(resize_ext, src_ext, sizeof(ui64)*TBAG_BOX_EXT_SIZE);
    }
    if (device != src_device ||
        ext[0] != resize_ext[0] ||
        ext[1] != resize_ext[1] ||
        ext[2] != resize_ext[2] ||
        ext[3] != resize_ext[3]) {
        if (data) {
            box_data_free(device, data);
        }
        if (dims) {
            box_dim_free(dims);
        }
        auto const src_dims_byte = GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank);
        return alloc_dims_copy(src_type, src_device, src_ext, src_dims, src_dims_byte, src_rank);
    }

    assert(device == src_device);
    assert(ext[0] == resize_ext[0]);
    assert(ext[1] == resize_ext[1]);
    assert(ext[2] == resize_ext[2]);
    assert(ext[3] == resize_ext[3]);

    if (box_dim_is_equals(dims, rank, src_dims, src_rank)) {
        return E_SUCCESS;
    }

    if (total_dims_byte < GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank)) {
        if (dims) {
            box_dim_free(dims);
        }
        dims = box_dim_clone(src_dims, src_rank);
        assert(dims != nullptr);
        total_dims_byte = GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank);
        rank = src_rank;
    } else {
        box_dim_copy(dims, src_dims, src_rank);
        rank = src_rank;
    }
    assert(total_dims_byte >= GET_RANK_TO_TOTAL_DIMS_BYTE(src_rank));
    assert(dims != nullptr);
    assert(rank == src_rank);
    assert(box_dim_is_equals(dims, rank, src_dims, src_rank));

    auto const dims_total_size = box_dim_get_total_size(src_dims, src_rank);
    assert(dims_total_size >= 1);
    assert(dims_total_size == box_dim_get_total_size(dims, rank));

    auto const dims_total_byte = box_get_type_byte(src_type) * dims_total_size;
    assert(dims_total_byte >= 1);

    if (total_data_byte < dims_total_byte) {
        if (data) {
            box_data_free(src_device, data);
        }
        data = box_data_malloc(src_type, src_device, dims_total_size);
        if (data == nullptr) {
            total_data_byte = 0;
            size = 0;
            return E_BADALLOC;
        }
        total_data_byte = dims_total_byte;
        size = dims_total_size;
    } else {
        size = dims_total_size;
    }
    type = src_type;
    assert(total_data_byte >= dims_total_byte);
    assert(data != nullptr);
    assert(size == dims_total_size);
    return E_SUCCESS;
}

ErrPair<box_data> box_data::clone(btype change_type, btype change_device, ui64 const * change_ext) const
{
    ErrPair<box_data> result;
    auto const code1 = result.value.alloc_dims_copy(change_type, change_device, change_ext,
                                                    dims, total_dims_byte, rank);
    if (isFailure(code1)) {
        return code1;
    }
    auto const code2 = result.value.assign_data(data, type, device, ext, size);
    if (isFailure(code2)) {
        return code2;
    }
    result.value.checked_assign_info_buffer(info, info_size);
    memcpy(&(result.value.opaque), &opaque, sizeof(opaque));
    result.code = E_SUCCESS;
    return result;
}

ErrPair<box_data> box_data::clone(btype change_device, ui64 const * change_ext) const
{
    return clone(type, change_device, change_ext);
}

ErrPair<box_data> box_data::clone(btype change_type) const
{
    return clone(change_type, device, ext);
}

ErrPair<box_data> box_data::clone() const
{
    return clone(type, device, ext);
}

void * box_data::get_data_ptr_by_offset(ui32 offset) TBAG_NOEXCEPT
{
    return box_data_ptr_offset_raw(data, type, offset);
}

void const * box_data::get_data_ptr_by_offset(ui32 offset) const TBAG_NOEXCEPT
{
    return box_data_ptr_offset_raw(data, type, offset);
}

bool box_data::check_data_address(void const * test_data_pointer) const TBAG_NOEXCEPT
{
    return box_data_check_address_raw(data, size, type, test_data_pointer);
}

void box_data::checked_assign_info_buffer(ui8 const * src, ui32 src_size)
{
    if (src == nullptr || src_size == 0) {
        if (info && total_info_byte >= 1) {
            info[0] = '\0';
        }
        info_size = 0;
        return;
    }

    if (info != nullptr && total_info_byte < src_size) {
        box_info_free(info);
        info = nullptr;
        total_info_byte = 0;
        size = 0;
    }

    if (info == nullptr) {
        info = box_info_malloc(src_size);
        assert(info != nullptr);
        total_info_byte = GET_SIZE_TO_TOTAL_INFO_BYTE(src_size);
        assert(total_info_byte >= 1);
    }

    assert(info != nullptr);
    assert(total_info_byte >= GET_SIZE_TO_TOTAL_INFO_BYTE(src_size));
    auto const result = box_info_assign_buffer(info, GET_TOTAL_INFO_BYTE_TO_SIZE(total_info_byte), src, src_size);
    assert(result);
    info_size = src_size;
}

void box_data::checked_assign_info_string(char const * src)
{
    assert(src != nullptr);
    checked_assign_info_buffer((ui8 const *)src, strlen(src)*sizeof(char));
}

Err box_data::set_data(void const * src_data, btype src_type, bdev src_device, ui64 const * src_ext,
                       ui32 box_data_offset)
{
    assert(src_data != nullptr);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    if (device == BD_CPU && src_device == BD_CPU) {
        box_cpu_set(get_data_ptr_by_offset(box_data_offset), type, src_data, src_type);
        return E_SUCCESS;
    } else if (device == BD_CUDA && src_device == BD_CUDA) {
        // TODO
    } else if (device == BD_CL && src_device == BD_CL) {
        // TODO
    }
    return E_ENOSYS;
}

Err box_data::set_data_args(void const * src_data, btype src_type, bdev src_device, ui64 const * src_ext,
                            ui32 box_rank, ...)
{
    assert(src_data != nullptr);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    assert(box_rank >= 1);
    va_list box_ap;
    va_start(box_ap, box_rank);
    auto const code = set_data_vargs(src_data, src_type, src_device, src_ext, box_rank, box_ap);
    va_end(box_ap);
    return code;
}

Err box_data::set_data_vargs(void const * src_data, btype src_type, bdev src_device, ui64 const * src_ext,
                             ui32 box_rank, va_list box_ap)
{
    assert(src_data != nullptr);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    assert(box_rank >= 1);
    return set_data(src_data, src_type, src_device, src_ext, box_dim_get_offset_vargs(dims, box_rank, box_ap));
}

Err box_data::set_data_dims(void const * src_data, btype src_type, bdev src_device, ui64 const * src_ext,
                            ui32 box_rank, ui32 const * box_indexes)
{
    assert(src_data != nullptr);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    assert(box_rank >= 1);
    return set_data(src_data, src_type, src_device, src_ext, box_dim_get_offset_dims(dims, box_rank, box_indexes));
}

Err box_data::get_data(void * out_data, btype out_type, bdev out_device, ui64 const * out_ext,
                       ui32 box_data_offset) const
{
    assert(out_data != nullptr);
    assert(box_support_type(out_type));
    assert(box_support_device(out_device));
    if (device == BD_CPU && out_device == BD_CPU) {
        box_cpu_set(out_data, out_type, get_data_ptr_by_offset(box_data_offset), type);
        return E_SUCCESS;
    } else if (device == BD_CUDA && out_device == BD_CUDA) {
        // TODO
    } else if (device == BD_CL && out_device == BD_CL) {
        // TODO
    }
    return E_ENOSYS;
}

Err box_data::get_data_args(void * out_data, btype out_type, bdev out_device, ui64 const * out_ext,
                            ui32 box_rank, ...) const
{
    assert(out_data != nullptr);
    assert(box_support_type(out_type));
    assert(box_support_device(out_device));
    assert(rank >= 1);
    va_list box_ap;
    va_start(box_ap, box_rank);
    auto const code = get_data_vargs(out_data, out_type, out_device, out_ext, box_rank, box_ap);
    va_end(box_ap);
    return code;
}

Err box_data::get_data_vargs(void * out_data, btype out_type, bdev out_device, ui64 const * out_ext,
                             ui32 box_rank, va_list box_ap) const
{
    assert(out_data != nullptr);
    assert(box_support_type(out_type));
    assert(box_support_device(out_device));
    assert(rank >= 1);
    auto const box_data_offset = box_dim_get_offset_vargs(dims, box_rank, box_ap);
    return get_data(out_data, out_type, out_device, out_ext, box_data_offset);
}

Err box_data::get_data_dims(void * out_data, btype out_type, bdev out_device, ui64 const * out_ext,
                            ui32 box_rank, ui32 const * box_indexes) const
{
    assert(out_data != nullptr);
    assert(box_support_type(out_type));
    assert(box_support_device(out_device));
    assert(rank >= 1);
    auto const box_data_offset = box_dim_get_offset_dims(dims, box_rank, box_indexes);
    return get_data(out_data, out_type, out_device, out_ext, box_data_offset);
}

Err box_data::assign_data(void const * src_data, btype src_type, bdev src_device, ui64 const * src_ext, ui32 src_size)
{
    assert(src_data != nullptr);
    assert(data != src_data);
    assert(box_support_type(src_type));
    assert(box_support_device(src_device));
    assert(src_size >= 1);
    assert(src_size <= size);
    if (device == BD_CPU && src_device == BD_CPU) {
        if (type == src_type) {
            box_cpu_memcpy(data, src_data, box_get_type_byte(src_type)*src_size);
        } else {
            box_cpu_element_copy(data, type, src_data, src_type, src_size);
        }
        return E_SUCCESS;
    } else if (device == BD_CUDA && src_device == BD_CUDA) {
        // TODO
    } else if (device == BD_CL && src_device == BD_CL) {
        // TODO
    }
    return E_ENOSYS;
}

// -------------------------
// box_cursor implementation
// -------------------------

box_cursor::box_cursor()
        : box(nullptr),
          begin(nullptr),
          end(nullptr),
          stride_byte(0)
{
    // EMPTY.
}

box_cursor::~box_cursor()
{
    // EMPTY.
}

ErrPair<box_cursor> box_cursor::init_cursor(ui32 dim_index, int begin_index, int end_index, int step_index) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return box->init_cursor(begin, dim_index, begin_index, end_index, step_index);
}

ErrPair<box_cursor> box_cursor::init_cursor(ui32 dim_index, int begin_index, int end_index) TBAG_NOEXCEPT
{
    return init_cursor(dim_index, begin_index, end_index, 1);
}

ErrPair<box_cursor> box_cursor::init_cursor(ui32 dim_index, int begin_index) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dim_index >= 0);
    assert(box->rank >= 1);
    assert(dim_index < box->rank);
    assert(box->dims != nullptr);
    return init_cursor(dim_index, begin_index, box->dims[dim_index]);
}

ErrPair<box_cursor> box_cursor::init_cursor(ui32 dim_index) TBAG_NOEXCEPT
{
    return init_cursor(dim_index, 0);
}

ErrPair<box_cursor> box_cursor::init_cursor() TBAG_NOEXCEPT
{
    return init_cursor(0);
}

bool box_cursor::is_end() const TBAG_NOEXCEPT
{
    return begin != end;
}

bool box_cursor::next() TBAG_NOEXCEPT
{
    auto const integer_data_pointer = (std::intptr_t)begin;
    begin = (void*)(integer_data_pointer + stride_byte);
    return is_end();
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

