/**
 * @file   CudaRaw.h
 * @brief  CUDA raw prototype.
 * @author zer0
 * @date   2017-08-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/macro/attributes.hpp>

#ifndef TB_CUDA_TRUE
#define TB_CUDA_TRUE 1
#endif

#ifndef TB_CUDA_FALSE
#define TB_CUDA_FALSE 0
#endif

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API int tbCudaAdd1f(float  const * gpu_v1, float  const * gpu_v2, float  * gpu_result, int size, void * stream = nullptr);
TBAG_API int tbCudaAdd1d(double const * gpu_v1, double const * gpu_v2, double * gpu_result, int size, void * stream = nullptr);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_HPP__

