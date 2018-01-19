/**
 * @file   AccelRaw.hpp
 * @brief  AccelRaw raw C++ prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/gpu/details/GpuDetails.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu   {
namespace accel {

TBAG_API bool runAccelAdd(type::TypeTable type, void const * v1, void const * v2, void * r, int count);

} // namespace accel
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__

