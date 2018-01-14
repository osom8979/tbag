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
#include <libtbag/macro/types.hpp>

#define TBAG_CUDA_RAW_VERBOSE

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API tbBOOL tbCudaAdd1i(int       const * gpu_v1, int       const * gpu_v2, int      * gpu_result, unsigned size, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1u(unsigned  const * gpu_v1, unsigned  const * gpu_v2, unsigned * gpu_result, unsigned size, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1f(float     const * gpu_v1, float     const * gpu_v2, float    * gpu_result, unsigned size, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1d(double    const * gpu_v1, double    const * gpu_v2, double   * gpu_result, unsigned size, void * stream = TB_NULL);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_HPP__

