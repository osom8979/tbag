/**
 * @file   box_cpu.hpp
 * @brief  box cpu prototype.
 * @author zer0
 * @date   2019-03-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/box/details/box_common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box     {
namespace details {

TBAG_API void * box_cpu_malloc(ui32 byte_size) TBAG_NOEXCEPT;
TBAG_API void   box_cpu_free(void * ptr) TBAG_NOEXCEPT;
TBAG_API void   box_cpu_memcpy(void * TBAG_RESTRICT dest, void const * TBAG_RESTRICT src, ui32 size) TBAG_NOEXCEPT;
TBAG_API void   box_cpu_element_copy(void * TBAG_RESTRICT dest, btype dest_type,
                                     void const * TBAG_RESTRICT src, btype src_type, ui32 size) TBAG_NOEXCEPT;

} // namespace details
} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_DETAILS_BOX_CPU_HPP__

