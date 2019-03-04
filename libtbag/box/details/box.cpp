/**
 * @file   box.cpp
 * @brief  C-based box method implementation.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
 */

#include <libtbag/box/details/box.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/memory/Memory.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

STATIC_ASSERT_CHECK_SIZEOF(btype,  2);
STATIC_ASSERT_CHECK_SIZEOF(bdev ,  2);
STATIC_ASSERT_CHECK_SIZEOF(si8  ,  1);
STATIC_ASSERT_CHECK_SIZEOF(si16 ,  2);
STATIC_ASSERT_CHECK_SIZEOF(si32 ,  4);
STATIC_ASSERT_CHECK_SIZEOF(si64 ,  8);
STATIC_ASSERT_CHECK_SIZEOF(ui8  ,  1);
STATIC_ASSERT_CHECK_SIZEOF(ui16 ,  2);
STATIC_ASSERT_CHECK_SIZEOF(ui32 ,  4);
STATIC_ASSERT_CHECK_SIZEOF(ui64 ,  8);
STATIC_ASSERT_CHECK_SIZEOF(fp32 ,  4);
STATIC_ASSERT_CHECK_SIZEOF(fp64 ,  8);
//STATIC_ASSERT_CHECK_SIZEOF(cp64 ,  8);
//STATIC_ASSERT_CHECK_SIZEOF(cp128, 16);

STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_NONE   , BT_NONE      );
//STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_BOOL , BT_BOOL      );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_STRING , BT_STRING    );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT8   , BT_INT8      );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT16  , BT_INT16     );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT32  , BT_INT32     );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_INT64  , BT_INT64     );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT8  , BT_UINT8     );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT16 , BT_UINT16    );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT32 , BT_UINT32    );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_UINT64 , BT_UINT64    );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_FLOAT32, BT_FLOAT32   );
STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_FLOAT64, BT_FLOAT64   );
//STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_COMPLEX64 , BT_COMPLEX64 );
//STATIC_ASSERT_CHECK_IS_EQUALS(BOX_TYPE_COMPLEX128, BT_COMPLEX128);

bool box_support_type(btype type) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (type) {
    case BOX_TYPE_STRING:  TBAG_FALLTHROUGH
    case BOX_TYPE_INT8:    TBAG_FALLTHROUGH
    case BOX_TYPE_INT16:   TBAG_FALLTHROUGH
    case BOX_TYPE_INT32:   TBAG_FALLTHROUGH
    case BOX_TYPE_INT64:   TBAG_FALLTHROUGH
    case BOX_TYPE_UINT8:   TBAG_FALLTHROUGH
    case BOX_TYPE_UINT16:  TBAG_FALLTHROUGH
    case BOX_TYPE_UINT32:  TBAG_FALLTHROUGH
    case BOX_TYPE_UINT64:  TBAG_FALLTHROUGH
    case BOX_TYPE_FLOAT32: TBAG_FALLTHROUGH
    case BOX_TYPE_FLOAT64:
        return true;
    case BOX_TYPE_NONE:
    default:
        break;
    }
    // @formatter:on
    return false;
}

bool box_support_device(bdev dev) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (dev) {
    case BOX_DEVICE_CPU:    return true;
    case BOX_DEVICE_CUDA:   return false;
    case BOX_DEVICE_CL:     return false;
    case BOX_DEVICE_GLSL:   return false;
    case BOX_DEVICE_NONE:   return false;
    default:                return false;
    }
    // @formatter:on
}

char const * const box_get_type_name(btype type) TBAG_NOEXCEPT
{
    switch (type) {
    case BOX_TYPE_STRING:   return "STRING";
    case BOX_TYPE_INT8:     return "INT8";
    case BOX_TYPE_INT16:    return "INT16";
    case BOX_TYPE_INT32:    return "INT32";
    case BOX_TYPE_INT64:    return "INT64";
    case BOX_TYPE_UINT8:    return "UINT8";
    case BOX_TYPE_UINT16:   return "UINT16";
    case BOX_TYPE_UINT32:   return "UINT32";
    case BOX_TYPE_UINT64:   return "UINT64";
    case BOX_TYPE_FLOAT32:  return "FLOAT32";
    case BOX_TYPE_FLOAT64:  return "FLOAT64";
    case BOX_TYPE_NONE:     return "NONE";
    default:                return "UNKNOWN";
    }
}

char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT
{
    switch (dev) {
    case BOX_DEVICE_CPU:    return "CPU";
    case BOX_DEVICE_CUDA:   return "CUDA";
    case BOX_DEVICE_CL:     return "CL";
    case BOX_DEVICE_GLSL:   return "GLSL";
    case BOX_DEVICE_NONE:   return "NONE";
    default:                return "UNKNOWN";
    }
}

ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT
{
    switch (type) {
    case BOX_TYPE_STRING:   return sizeof(char);
    case BOX_TYPE_INT8:     return sizeof(si8);
    case BOX_TYPE_INT16:    return sizeof(si16);
    case BOX_TYPE_INT32:    return sizeof(si32);
    case BOX_TYPE_INT64:    return sizeof(si64);
    case BOX_TYPE_UINT8:    return sizeof(ui8);
    case BOX_TYPE_UINT16:   return sizeof(ui16);
    case BOX_TYPE_UINT32:   return sizeof(ui32);
    case BOX_TYPE_UINT64:   return sizeof(ui64);
    case BOX_TYPE_FLOAT32:  return sizeof(fp32);
    case BOX_TYPE_FLOAT64:  return sizeof(fp64);
    case BOX_TYPE_NONE:
        TBAG_FALLTHROUGH
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    return 0;
}

ui32 box_get_total_length(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT
{
    assert(dims != nullptr);
    assert(rank >= 1);
    ui32 total_length = dims[0];
    for (std::size_t i = 1; i < rank; ++i) {
        total_length *= dims[i];
    }
    return total_length;
}

Err box_clear(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    box->type = BOX_TYPE_NONE;
    box->device = BOX_DEVICE_NONE;
    box->data = nullptr;
    box->total_byte = 0;
    box->dims = nullptr;
    box->rank = 0;
    return Err::E_UNKNOWN;
}

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

