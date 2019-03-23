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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

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
#define BOX_TYPE_SIZE_8BIT     8
#define BOX_TYPE_SIZE_16BIT   16
#define BOX_TYPE_SIZE_32BIT   32
#define BOX_TYPE_SIZE_64BIT   64
#define BOX_TYPE_SIZE_128BIT 128

#define BOX_TYPE_NONE        0x0000
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
#define BOX_DEVICE_FBS   5

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

#define MAKE_BOX_TYPE(prefix, size) static_cast<btype>(((prefix)<<(8))|(size))

TBAG_CONSTEXPR btype const BT_NONE    = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNKNOWN , BOX_TYPE_SIZE_0BIT  );
TBAG_CONSTEXPR btype const BT_INT8    = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_8BIT  );
TBAG_CONSTEXPR btype const BT_INT16   = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_16BIT );
TBAG_CONSTEXPR btype const BT_INT32   = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR btype const BT_INT64   = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_SIGNED  , BOX_TYPE_SIZE_64BIT );
TBAG_CONSTEXPR btype const BT_UINT8   = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_8BIT  );
TBAG_CONSTEXPR btype const BT_UINT16  = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_16BIT );
TBAG_CONSTEXPR btype const BT_UINT32  = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR btype const BT_UINT64  = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_UNSIGNED, BOX_TYPE_SIZE_64BIT );
TBAG_CONSTEXPR btype const BT_FLOAT32 = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_FLOATING, BOX_TYPE_SIZE_32BIT );
TBAG_CONSTEXPR btype const BT_FLOAT64 = MAKE_BOX_TYPE(BOX_TYPE_PREFIX_FLOATING, BOX_TYPE_SIZE_64BIT );

struct box_data
{
    /** Data type. */
    btype type;

    /** Device type. */
    bdev device;

    /** Device extension information. */
    /** [PARALLEL] 0:Platform, 1:Device, 2:Context 3:Unused*/
    ui64 ext[4];

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
TBAG_API void box_clear(box_data * box) TBAG_NOEXCEPT;

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__

