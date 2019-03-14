/**
 * @file   box_api.hpp
 * @brief  C-based box method prototype.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
 * @date   2019-03-14 (Rename: libtbag/box/details/box -> libtbag/box/details/box_api)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/box/details/box_common.hpp>

#include <cstdarg>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

TBAG_API Err box_malloc_copy_dims(box_data * box, btype type, bdev device, ui32 const * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API Err box_malloc_move_dims(box_data * box, btype type, bdev device, ui32 * dims, ui32 rank) TBAG_NOEXCEPT;
TBAG_API Err box_malloc_args(box_data * box, btype type, bdev device, ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API Err box_malloc_vargs(box_data * box, btype type, bdev device, ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API Err box_free(box_data * box) TBAG_NOEXCEPT;

TBAG_API bool box_exists_data(box_data const * box) TBAG_NOEXCEPT;
TBAG_API Err  box_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err  box_checked_clone(box_data * dest, box_data const * src) TBAG_NOEXCEPT;

using box_memcpy_async_cb = void(*)(Err, void*, box_data const*, box_data*);

TBAG_API Err box_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err box_checked_memcpy(box_data * dest, box_data const * src) TBAG_NOEXCEPT;
TBAG_API Err box_memcpy_async(box_data * dest, box_data const * src, void * user, box_memcpy_async_cb cb) TBAG_NOEXCEPT;

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__

