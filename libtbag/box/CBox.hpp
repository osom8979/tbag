/**
 * @file   CBox.hpp
 * @brief  C-based box method prototype.
 * @author zer0
 * @date   2019-02-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_CBOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_CBOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <ccomplex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

#define BOX_TYPE_PREFIX_BIT  0xFF00
#define BOX_TYPE_SUFFIX_BIT  0x00FF

#define BOX_TYPE_PREFIX_UNKNOWN   0
#define BOX_TYPE_PREFIX_BOOLEAN   1
#define BOX_TYPE_PREFIX_STRING    2
#define BOX_TYPE_PREFIX_SIGNED    3
#define BOX_TYPE_PREFIX_UNSIGNED  4
#define BOX_TYPE_PREFIX_FLOATING  5
#define BOX_TYPE_PREFIX_COMPLEX   6

#define BOX_TYPE_SIZE_0BIT     0
#define BOX_TYPE_SIZE_1BIT     1
#define BOX_TYPE_SIZE_8BIT     8
#define BOX_TYPE_SIZE_16BIT   16
#define BOX_TYPE_SIZE_32BIT   32
#define BOX_TYPE_SIZE_64BIT   64
#define BOX_TYPE_SIZE_128BIT 128

#define BOX_TYPE_NONE        0x0000
#define BOX_TYPE_BOOL        0x0101
#define BOX_TYPE_STRING      0x0208
#define BOX_TYPE_INT8        0x0308
#define BOX_TYPE_INT16       0x0310
#define BOX_TYPE_INT32       0x0320
#define BOX_TYPE_INT64       0x0340
#define BOX_TYPE_UINT8       0x0408
#define BOX_TYPE_UINT16      0x0410
#define BOX_TYPE_UINT32      0x0420
#define BOX_TYPE_UINT64      0x0440
#define BOX_TYPE_FLOAT32     0x0520
#define BOX_TYPE_FLOAT64     0x0540
#define BOX_TYPE_COMPLEX64   0x0640
#define BOX_TYPE_COMPLEX128  0x0680

#define BOX_DEVICE_NONE  0
#define BOX_DEVICE_CPU   1
#define BOX_DEVICE_CUDA  2
#define BOX_DEVICE_CL    3
#define BOX_DEVICE_GLSL  4

using btype = uint16_t;
using bdev  = uint16_t;

using si8  = int8_t ;
using si16 = int16_t;
using si32 = int32_t;
using si64 = int64_t;

using ui8  = uint8_t;
using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

using fp32 = float;
using fp64 = double;

#if 0
using cp64  = float  _Complex;
using cp128 = double _Complex;
#endif

#define MAKE_BOX_TYPE(prefix, size) static_cast<btype>(((prefix)<<(8))|(size))

TBAG_CONSTEXPR   btype const BT_NONE       = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNKNOWN , BOX_TYPE_SIZE_0BIT  );
//TBAG_CONSTEXPR btype const BT_BOOL       = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_BOOLEAN , BOX_TYPE_SIZE_1BIT  );
TBAG_CONSTEXPR   btype const BT_STRING     = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_STRING  , BOX_TYPE_SIZE_8BIT  );
TBAG_CONSTEXPR   btype const BT_INT8       = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_8BIT  );
TBAG_CONSTEXPR   btype const BT_INT16      = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_16BIT );
TBAG_CONSTEXPR   btype const BT_INT32      = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR   btype const BT_INT64      = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_64BIT );
TBAG_CONSTEXPR   btype const BT_UINT8      = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_8BIT  );
TBAG_CONSTEXPR   btype const BT_UINT16     = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_16BIT );
TBAG_CONSTEXPR   btype const BT_UINT32     = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR   btype const BT_UINT64     = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_64BIT );
TBAG_CONSTEXPR   btype const BT_FLOAT32    = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_FLOATING, BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR   btype const BT_FLOAT64    = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_FLOATING, BOX_TYPE_SIZE_64BIT );
//TBAG_CONSTEXPR btype const BT_COMPLEX64  = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_COMPLEX , BOX_TYPE_SIZE_64BIT );
//TBAG_CONSTEXPR btype const BT_COMPLEX128 = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_COMPLEX , BOX_TYPE_SIZE_128BIT);

struct box_data
{
    /** Data type. */
    btype type;

    /** Device type. */
    bdev device;

    /** Real data. */
    void * data;

    /** Real data byte. */
    ui32 total_byte;

    /** Dimension data. */
    ui32 * dims;

    /** Dimension data length. */
    ui32 rank;

    /** User's data pointer. */
    void * opaque;
};

TBAG_API bool box_support_type(btype type) TBAG_NOEXCEPT;
TBAG_API bool box_support_device(bdev dev) TBAG_NOEXCEPT;

TBAG_API char const * const box_get_type_name(btype type) TBAG_NOEXCEPT;
TBAG_API char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT;

TBAG_API ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT;
TBAG_API ui32 box_get_total_length(ui32 const * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 box_get_align_byte(btype type, bdev device) TBAG_NOEXCEPT;

TBAG_API Err box_clear(box_data * box) TBAG_NOEXCEPT;

TBAG_API ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT;
TBAG_API void   box_dim_free(ui32 * dims) TBAG_NOEXCEPT;
TBAG_API void   box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_copy(ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API bool   box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT;

TBAG_API Err  box_malloc(box_data * box, btype type, bdev device, ui32 const * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API Err  box_free(box_data * box) TBAG_NOEXCEPT;
TBAG_API bool box_exists_data(box_data const * box) TBAG_NOEXCEPT;
TBAG_API Err  box_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err  box_checked_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT;

using box_memcpy_async_cb = void(*)(Err, void*, box_data const*, box_data*);

TBAG_API Err box_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err box_checked_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err box_memcpy_async(box_data * dest, box_data const * src, void * user, box_memcpy_async_cb cb) TBAG_NOEXCEPT;

TBAG_API Err box_get_index(box_data const * box, std::size_t * index) TBAG_NOEXCEPT;

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_CBOX_HPP__

