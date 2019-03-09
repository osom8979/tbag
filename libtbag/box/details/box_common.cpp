/**
 * @file   box_common.cpp
 * @brief  box common implementation.
 * @author zer0
 * @date   2019-03-04
 */

#include <libtbag/box/details/box_common.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cassert>

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
    // @formatter:off
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
    // @formatter:on
}

bool box_support_device(bdev dev) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (dev) {
    case BOX_DEVICE_NONE: return false;
    case BOX_DEVICE_CPU:  return true;
    case BOX_DEVICE_CUDA: return false;
    case BOX_DEVICE_CL:   return false;
    case BOX_DEVICE_GLSL: return false;
    case BOX_DEVICE_FBS:  return false;
    default:              return false;
    }
    // @formatter:on
}

char const * const box_get_type_name(btype type) TBAG_NOEXCEPT
{
    // @formatter:off
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
    // @formatter:on
}

char const * const box_get_device_name(bdev dev) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (dev) {
    case BOX_DEVICE_CPU:  return "CPU";
    case BOX_DEVICE_CUDA: return "CUDA";
    case BOX_DEVICE_CL:   return "CL";
    case BOX_DEVICE_GLSL: return "GLSL";
    case BOX_DEVICE_FBS:  return "FBS";
    case BOX_DEVICE_NONE: return "NONE";
    default:              return "UNKNOWN";
    }
    // @formatter:on
}

ui32 box_get_type_byte(btype type) TBAG_NOEXCEPT
{
    // @formatter:off
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
    // @formatter:on
}

void box_clear(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    box->type = BOX_TYPE_NONE;
    box->device = BOX_DEVICE_NONE;
    box->data = nullptr;
    box->total_byte = 0;
    box->dims = nullptr;
    box->rank = 0;
}

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

