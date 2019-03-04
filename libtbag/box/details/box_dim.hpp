/**
 * @file   box_dim.hpp
 * @brief  box dim prototype.
 * @author zer0
 * @date   2019-03-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_DIM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_DIM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/box/details/box_common.hpp>

#include <cstdarg>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

TBAG_API ui32 * box_dim_malloc(ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_args(ui32 rank, ...) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_vargs(ui32 rank, va_list ap) TBAG_NOEXCEPT;
TBAG_API void   box_dim_free(ui32 * dims) TBAG_NOEXCEPT;
TBAG_API void   box_dim_set_args(ui32 * TBAG_RESTRICT dims, ui32 args_count, ...) TBAG_NOEXCEPT;
TBAG_API void   box_dim_set_vargs(ui32 * TBAG_RESTRICT dims, ui32 args_count, va_list ap) TBAG_NOEXCEPT;
TBAG_API void   box_dim_copy(ui32 * dest, ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API ui32 * box_dim_malloc_copy(ui32 const * src, ui32 rank) TBAG_NOEXCEPT;
TBAG_API bool   box_dim_is_equals(ui32 const * dims1, ui32 rank1, ui32 const * dims2, ui32 rank2) TBAG_NOEXCEPT;

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_DIM_HPP__

