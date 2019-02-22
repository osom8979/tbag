/**
 * @file   CBox.cpp
 * @brief  C-based box method implementation.
 * @author zer0
 * @date   2019-02-21
 */

#include <libtbag/box/CBox.hpp>
#include <libtbag/Type.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

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

Err box_init(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    box->type = BOX_TYPE_NONE;
    box->device = BOX_DEVICE_NONE;
    box->data = nullptr;
    box->dims = nullptr;
    box->total_byte = 0;
    box->rank = 0;
    box->padding = 0;
    return Err::E_UNKNOWN;
}

Err box_malloc(box_data * box, btype type, bdev device, ui32 * dims, ui32 rank, ui32 padding) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    assert(dims != nullptr);

//    switch (type) {
//    case BOX_TYPE_NONE:       break;
//    case BOX_TYPE_STRING:     break;
//    case BOX_TYPE_INT8:       break;
//    case BOX_TYPE_INT16:      break;
//    case BOX_TYPE_INT32:      break;
//    case BOX_TYPE_INT64:      break;
//    case BOX_TYPE_UINT8:      break;
//    case BOX_TYPE_UINT16:     break;
//    case BOX_TYPE_UINT32:     break;
//    case BOX_TYPE_UINT64:     break;
//    case BOX_TYPE_FLOAT32:    break;
//    case BOX_TYPE_FLOAT64:    break;
//    default:                  break;
//    }

    return Err::E_UNKNOWN;
}

Err box_free(box_data * box) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return Err::E_UNKNOWN;
}

Err box_memcopy(box_data const * source, box_data * destination) TBAG_NOEXCEPT
{
    assert(source != nullptr);
    assert(destination != nullptr);
    return Err::E_UNKNOWN;
}

Err box_memcpy_async(box_data const * source, box_data * destination, void * user, box_memcpy_async_cb cb) TBAG_NOEXCEPT
{
    assert(source != nullptr);
    assert(destination != nullptr);
    return Err::E_UNKNOWN;
}

Err box_get_index(box_data const * box, std::size_t * index) TBAG_NOEXCEPT
{
    assert(box != nullptr);
    return Err::E_UNKNOWN;
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

