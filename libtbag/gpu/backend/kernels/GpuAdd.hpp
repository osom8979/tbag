/**
 * @file   GpuAdd.hpp
 * @brief  GpuAdd class prototype.
 * @author zer0
 * @date   2018-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {
namespace kernels {

TBAG_API bool addByCpu(float  const * v1, float  const * v2, float  * result, int count);
TBAG_API bool addByCpu(double const * v1, double const * v2, double * result, int count);

TBAG_API bool addByCuda(float  const * v1, float  const * v2, float  * result, int count);
TBAG_API bool addByCuda(double const * v1, double const * v2, double * result, int count);

} // namespace kernels
} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__

