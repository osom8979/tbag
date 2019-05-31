/**
 * @file   box_common.hpp
 * @brief  box common prototype.
 * @author zer0
 * @date   2019-03-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <cstdarg>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

#define _BOX_TYPE_PREFIX_BIT  0xFF00
#define _BOX_TYPE_SUFFIX_BIT  0x00FF

#define _BOX_TYPE_PREFIX_UNKNOWN   0
#define _BOX_TYPE_PREFIX_BOOLEAN   1
#define _BOX_TYPE_PREFIX_STRING    2
#define _BOX_TYPE_PREFIX_SIGNED    3
#define _BOX_TYPE_PREFIX_UNSIGNED  4
#define _BOX_TYPE_PREFIX_FLOATING  5
#define _BOX_TYPE_PREFIX_COMPLEX   6

#define _BOX_TYPE_SIZE_0BIT     0
#define _BOX_TYPE_SIZE_8BIT     8
#define _BOX_TYPE_SIZE_16BIT   16
#define _BOX_TYPE_SIZE_32BIT   32
#define _BOX_TYPE_SIZE_64BIT   64
#define _BOX_TYPE_SIZE_128BIT 128

#define _BOX_TYPE_NONE        0x0000
#define _BOX_TYPE_INT8        0x0308
#define _BOX_TYPE_INT16       0x0310
#define _BOX_TYPE_INT32       0x0320
#define _BOX_TYPE_INT64       0x0340
#define _BOX_TYPE_UINT8       0x0408
#define _BOX_TYPE_UINT16      0x0410
#define _BOX_TYPE_UINT32      0x0420
#define _BOX_TYPE_UINT64      0x0440
#define _BOX_TYPE_FLOAT32     0x0520
#define _BOX_TYPE_FLOAT64     0x0540
#define _BOX_TYPE_COMPLEX64   0x0640
#define _BOX_TYPE_COMPLEX128  0x0680

#define _BOX_DEVICE_NONE  0
#define _BOX_DEVICE_CPU   1
#define _BOX_DEVICE_CUDA  2
#define _BOX_DEVICE_CL    3
#define _BOX_DEVICE_GLSL  4
#define _BOX_DEVICE_FBS   5

#define _BOX_EXT_SIZE 4

using si8  = int8_t;
using si16 = int16_t;
using si32 = int32_t;
using si64 = int64_t;

using ui8  = uint8_t;
using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

using fp32 = float;
using fp64 = double;

using btype = ui16;
using bdev  = ui16;

#define _MAKE_BOX_TYPE(prefix, size) (((prefix)<<(8))|(size))

enum BoxType
{
    BT_NONE    = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_UNKNOWN , _BOX_TYPE_SIZE_0BIT  ),
    BT_INT8    = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_SIGNED  , _BOX_TYPE_SIZE_8BIT  ),
    BT_INT16   = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_SIGNED  , _BOX_TYPE_SIZE_16BIT ),
    BT_INT32   = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_SIGNED  , _BOX_TYPE_SIZE_32BIT ),
    BT_INT64   = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_SIGNED  , _BOX_TYPE_SIZE_64BIT ),
    BT_UINT8   = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_UNSIGNED, _BOX_TYPE_SIZE_8BIT  ),
    BT_UINT16  = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_UNSIGNED, _BOX_TYPE_SIZE_16BIT ),
    BT_UINT32  = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_UNSIGNED, _BOX_TYPE_SIZE_32BIT ),
    BT_UINT64  = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_UNSIGNED, _BOX_TYPE_SIZE_64BIT ),
    BT_FLOAT32 = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_FLOATING, _BOX_TYPE_SIZE_32BIT ),
    BT_FLOAT64 = _MAKE_BOX_TYPE(_BOX_TYPE_PREFIX_FLOATING, _BOX_TYPE_SIZE_64BIT ),
};

enum BoxDevice
{
    BD_NONE = _BOX_DEVICE_NONE,
    BD_CPU  = _BOX_DEVICE_CPU ,
    BD_CUDA = _BOX_DEVICE_CUDA,
    BD_CL   = _BOX_DEVICE_CL  ,
    BD_GLSL = _BOX_DEVICE_GLSL,
};

TBAG_CONSTEXPR int const BOX_EXTENSION_SIZE = _BOX_EXT_SIZE;

/**
 * Box container information structure.
 *
 * @author zer0
 * @date   2019-03-04
 *
 * @remarks
 *  It is an alternative to the 1d vector container. @n
 *  Therefore, the entire memory must be connected in one dimension.
 */
struct box_data
{
    /** Data type. */
    btype type;

    /** Device type. */
    bdev device;

    /** Device extension information. */
    /** [PARALLEL] 0:Platform, 1:Device, 2:Context 3:Unused*/
    ui64 ext[BOX_EXTENSION_SIZE];

    /** Real data. */
    void * data;

    /** Real data byte. */
    ui32 total_data_byte;

    /** Total number of entries. */
    ui32 size;

    /**
     * Dimension data.
     *
     * @remarks
     *  Example:
     *  - Index: 0,1,2,3
     *  - Tensor: batch,height,width,color
     *  - 2D-Matrix: rows,cols
     *  - 2D-Image: height,width
     *  - Coordinate: w,z,y,x
     */
    ui32 * dims;

    /** Dimension data byte. */
    ui32 total_dims_byte;

    /** Number of dimensions. */
    ui32 rank;

    /** Extra information. */
    ui8 * info;

    /** Extra information data byte. */
    ui32 total_info_byte;

    /** Extra information size. */
    ui32 info_size;

    /** User's data pointer. */
    void * opaque;
};

#define GET_SIZE_TO_TOTAL_INFO_BYTE(size) (size*sizeof(ui8))
#define GET_TOTAL_INFO_BYTE_TO_SIZE(byte) (byte/sizeof(ui8))
#define GET_RANK_TO_TOTAL_DIMS_BYTE(rank) (rank*sizeof(ui32))
#define GET_TOTAL_DIMS_BYTE_TO_RANK(byte) (byte/sizeof(ui32))
#define CHECK_TOTAL_DIMS_BYTE(total_dims_byte) \
    (total_dims_byte >= sizeof(ui32) && total_dims_byte % sizeof(ui32) == 0)

#define BOX_TEMP_DIM_STACK_SIZE 16

TBAG_API bool box_support_type(btype type) TBAG_NOEXCEPT;
TBAG_API bool box_support_device(bdev dev) TBAG_NOEXCEPT;

TBAG_API char const * const box_get_type_name(btype type) TBAG_NOEXCEPT;
TBAG_API char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT;

TBAG_API ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT;
TBAG_API void box_clear(box_data * box) TBAG_NOEXCEPT;

// clang-format off
TBAG_API void box_opaque_set(box_data * box, si8  v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, si16 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, si32 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, si64 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, ui8  v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, ui16 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, ui32 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_set(box_data * box, ui64 v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, si8  * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, si16 * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, si32 * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, si64 * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, ui8  * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, ui16 * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, ui32 * v) TBAG_NOEXCEPT;
TBAG_API void box_opaque_get(box_data const * box, ui64 * v) TBAG_NOEXCEPT;
// clang-format on

TBAG_API ui32 box_get_size_args(ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 box_get_size_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT;

TBAG_API ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API void   box_dim_free(ui32 * dims) TBAG_NOEXCEPT;
TBAG_API void   box_dim_set_args(ui32 * TBAG_RESTRICT dims, ui32 args_count, ...) TBAG_NOEXCEPT;
TBAG_API void   box_dim_set_vargs(ui32 * TBAG_RESTRICT dims, ui32 args_count, va_list ap) TBAG_NOEXCEPT;
TBAG_API void   box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_clone(ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_clone_with_mem_size(ui32 const * src, ui32 dims_size, ui32 rank) TBAG_NOEXCEPT;
TBAG_API bool   box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT;
TBAG_API bool   box_dim_is_equals_args(ui32 const * dims1, ui32 rank1, ui32 rank2, ...) TBAG_NOEXCEPT;
TBAG_API bool   box_dim_is_equals_vargs(ui32 const * dims1, ui32 rank1, ui32 rank2, va_list ap) TBAG_NOEXCEPT;
TBAG_API ui32   box_dim_get_size(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32   box_dim_get_stride(ui32 const * dims, ui32 rank, ui32 dim_index) TBAG_NOEXCEPT;

/**
 * The formula to obtain the index is as follows:
 * calc 1rank index: x
 * calc 2rank index: x + (y * width)
 * calc 3rank index: x + (y * width) + (z * width * height)
 * calc 4rank index: x + (y * width) + (z * width * height) + (w * width * height * depth)
 * ...
 *
 * Therefore, it can be converted into the following formula:
 * ...
 * calc 4rank index: x + ((y + (z + (w * depth)) * height) * width)
 * calc 3rank index: x + (y + (z * height)) * width
 * calc 2rank index: x + (y * width)
 * calc 1rank index: x
 *
 * Calculated from inside parentheses.
 */
TBAG_API ui32 box_dim_get_index_args(ui32 const * dims, ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 box_dim_get_index_vargs(ui32 const * dims, ui32 rank, va_list ap) TBAG_NOEXCEPT;

TBAG_API ui8 * box_info_malloc(ui32 info_size) TBAG_NOEXCEPT;
TBAG_API void  box_info_free(ui8 * info) TBAG_NOEXCEPT;
TBAG_API bool  box_info_assign(ui8 * dest, ui32 dest_size, ui8 const * src, ui32 src_size) TBAG_NOEXCEPT;
TBAG_API bool  box_info_assign_string(ui8 * dest, ui32 dest_size, char const * src) TBAG_NOEXCEPT;

TBAG_API void       * box_data_ptr_offset_raw(void       * data, btype type, ui32 offset) TBAG_NOEXCEPT;
TBAG_API void const * box_data_ptr_offset_raw(void const * data, btype type, ui32 offset) TBAG_NOEXCEPT;

TBAG_API bool box_data_check_address_raw(void const * data_begin, ui32 size, btype type,
                                         void const * check_data) TBAG_NOEXCEPT;

/**
 * Box container iterator cursor structure.
 *
 * @author zer0
 * @date   2019-05-16
 */
struct box_cursor
{
    box_data * box;

    void * data;
    int dim_index;
    int stride;

    int begin;
    int end;
    int step;

    int index;
};

TBAG_CONSTEXPR int const BOX_CURSOR_INIT_INDEX = -1;

TBAG_API int  box_index_abs(int dim_size, int index) TBAG_NOEXCEPT;
TBAG_API bool box_index_check(int begin, int end, int step) TBAG_NOEXCEPT;

TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                              int stride, int begin, int end, int step, int index) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                              int stride, int begin, int end, int step) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                              int stride, int begin, int end) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                              int stride, int begin) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index,
                              int stride) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data, int dim_index) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box, void * data) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init(box_cursor * cursor, box_data * box) TBAG_NOEXCEPT;

TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                                  int stride, int begin, int end, int step, int index) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                                  int stride, int begin, int end, int step) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                                  int stride, int begin, int end) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                                  int stride, int begin) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data, int dim_index,
                                  int stride) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data,
                                  int dim_index) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor, void * data) TBAG_NOEXCEPT;
TBAG_API bool box_cursor_init_sub(box_cursor * cursor, box_cursor * parent_cursor) TBAG_NOEXCEPT;

TBAG_API bool box_cursor_next(box_cursor * cursor) TBAG_NOEXCEPT;

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__

