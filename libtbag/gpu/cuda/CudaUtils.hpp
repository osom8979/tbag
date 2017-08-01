/**
 * @file   CudaUtils.hpp
 * @brief  CudaUtils class prototype.
 * @author zer0
 * @date   2017-08-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDAUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDAUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/GpuBackend.hpp>
#include <vector>

#if defined(__cplusplus)
extern "C" {
#endif

// ---------------------
// Naming rule:
// * Prefix: tbCuda
// * Suffix: ByCpu/ByGpu
// ---------------------

TBAG_API int tbCudaAddByCpu(int const * lh, int const * rh, int * result, int size);
TBAG_API int tbCudaAddByGpu(int const * lh, int const * rh, int * result, int size);

#if defined(__cplusplus)
} /* extern "C" */
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu  {
namespace cuda {

TBAG_API std::vector<int> add(std::vector<int> const & lh, std::vector<int> const & rh, BrewType type = BrewType::BT_UNKNOWN);

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDAUTILS_HPP__

