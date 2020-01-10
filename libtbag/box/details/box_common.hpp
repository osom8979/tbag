/**
 * @file   box_common.hpp
 * @brief  box_common class prototype.
 * @author zer0
 * @date   2020-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <cstdint>
#include <ccomplex>

#include <complex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

#define TBAG_BOX_TYPE_PREFIX_BIT  0xFF00
#define TBAG_BOX_TYPE_SUFFIX_BIT  0x00FF

#define TBAG_BOX_TYPE_PREFIX_UNKNOWN   0
#define TBAG_BOX_TYPE_PREFIX_BOOLEAN   1
#define TBAG_BOX_TYPE_PREFIX_STRING    2
#define TBAG_BOX_TYPE_PREFIX_SIGNED    3
#define TBAG_BOX_TYPE_PREFIX_UNSIGNED  4
#define TBAG_BOX_TYPE_PREFIX_FLOATING  5
#define TBAG_BOX_TYPE_PREFIX_COMPLEX   6

#define TBAG_BOX_TYPE_SIZE_SYSTEM   0
#define TBAG_BOX_TYPE_SIZE_0BIT     0
#define TBAG_BOX_TYPE_SIZE_8BIT     8
#define TBAG_BOX_TYPE_SIZE_16BIT   16
#define TBAG_BOX_TYPE_SIZE_32BIT   32
#define TBAG_BOX_TYPE_SIZE_64BIT   64
#define TBAG_BOX_TYPE_SIZE_128BIT 128

#define TBAG_BOX_TYPE_NONE        0x0000
#define TBAG_BOX_TYPE_BOOL        0x0100
#define TBAG_BOX_TYPE_INT8        0x0308
#define TBAG_BOX_TYPE_INT16       0x0310
#define TBAG_BOX_TYPE_INT32       0x0320
#define TBAG_BOX_TYPE_INT64       0x0340
#define TBAG_BOX_TYPE_UINT8       0x0408
#define TBAG_BOX_TYPE_UINT16      0x0410
#define TBAG_BOX_TYPE_UINT32      0x0420
#define TBAG_BOX_TYPE_UINT64      0x0440
#define TBAG_BOX_TYPE_FLOAT32     0x0520
#define TBAG_BOX_TYPE_FLOAT64     0x0540
#define TBAG_BOX_TYPE_COMPLEX64   0x0640
#define TBAG_BOX_TYPE_COMPLEX128  0x0680

#define TBAG_BOX_DEVICE_CPU   0
#define TBAG_BOX_DEVICE_CUDA  1
#define TBAG_BOX_DEVICE_CL    2

#define TBAG_BOX_EXT_SIZE 4
#define TBAG_BOX_TEMP_DIM_STACK_SIZE 16

#define GET_SIZE_TO_TOTAL_INFO_BYTE(size) (size*sizeof(ui8))
#define GET_TOTAL_INFO_BYTE_TO_SIZE(byte) (byte/sizeof(ui8))
#define GET_RANK_TO_TOTAL_DIMS_BYTE(rank) (rank*sizeof(ui32))
#define GET_TOTAL_DIMS_BYTE_TO_RANK(byte) (byte/sizeof(ui32))
#define CHECK_TOTAL_DIMS_BYTE(total_dims_byte) \
    (total_dims_byte >= sizeof(ui32) && total_dims_byte % sizeof(ui32) == 0)

#define TBAG_MAKE_BOX_TYPE(prefix, size) (((prefix)<<(8))|(size))
#define TBAG_GET_BOX_TYPE_PREFIX(type) (((type)&(TBAG_BOX_TYPE_PREFIX_BIT))>>(8))
#define TBAG_GET_BOX_TYPE_SUFFIX(type) ((type)&(TBAG_BOX_TYPE_SUFFIX_BIT))

enum BoxType
{
    BT_NONE       = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_UNKNOWN , TBAG_BOX_TYPE_SIZE_0BIT  ),
    BT_BOOL       = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_BOOLEAN , TBAG_BOX_TYPE_SIZE_SYSTEM),
    BT_INT8       = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_SIGNED  , TBAG_BOX_TYPE_SIZE_8BIT  ),
    BT_INT16      = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_SIGNED  , TBAG_BOX_TYPE_SIZE_16BIT ),
    BT_INT32      = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_SIGNED  , TBAG_BOX_TYPE_SIZE_32BIT ),
    BT_INT64      = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_SIGNED  , TBAG_BOX_TYPE_SIZE_64BIT ),
    BT_UINT8      = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_UNSIGNED, TBAG_BOX_TYPE_SIZE_8BIT  ),
    BT_UINT16     = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_UNSIGNED, TBAG_BOX_TYPE_SIZE_16BIT ),
    BT_UINT32     = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_UNSIGNED, TBAG_BOX_TYPE_SIZE_32BIT ),
    BT_UINT64     = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_UNSIGNED, TBAG_BOX_TYPE_SIZE_64BIT ),
    BT_FLOAT32    = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_FLOATING, TBAG_BOX_TYPE_SIZE_32BIT ),
    BT_FLOAT64    = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_FLOATING, TBAG_BOX_TYPE_SIZE_64BIT ),
    BT_COMPLEX64  = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_COMPLEX , TBAG_BOX_TYPE_SIZE_64BIT ),
    BT_COMPLEX128 = TBAG_MAKE_BOX_TYPE(TBAG_BOX_TYPE_PREFIX_COMPLEX , TBAG_BOX_TYPE_SIZE_128BIT),
};

enum BoxDevice
{
    BD_CPU  = TBAG_BOX_DEVICE_CPU,
    BD_CUDA = TBAG_BOX_DEVICE_CUDA,
    BD_CL   = TBAG_BOX_DEVICE_CL,
};

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

using c64  = std::complex<float>;
using c128 = std::complex<double>;

using nc64  = decltype(0.0f+0.0fi);
using nc128 = decltype(0.0+0.0i);

using btype = ui16;
using bdev  = ui16;

/**
 * Box any container.
 *
 * @author zer0
 * @date   2019-12-14
 */
union box_any
{
    void * pointer;
    bool   data_bool;
    si8    data_si8;
    si16   data_si16;
    si32   data_si32;
    si64   data_si64;
    ui8    data_ui8;
    ui16   data_ui16;
    ui32   data_ui32;
    ui64   data_ui64;
    fp32   data_fp32;
    fp64   data_fp64;
};

using box_opaque_delete_cb = void(*)(void*);

TBAG_CONSTEXPR int const box_nop = libtbag::type::TypeInfo<int>::maximum();

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_COMMON_HPP__

