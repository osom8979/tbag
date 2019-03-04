/**
 * @file   box.hpp
 * @brief  C-based box method prototype.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-04 (Rename: libtbag/box/CBox -> libtbag/box/details/box)
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

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

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

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_HPP__

