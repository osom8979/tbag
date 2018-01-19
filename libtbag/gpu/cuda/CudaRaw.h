/**
 * @file   CudaRaw.h
 * @brief  CUDA raw prototype.
 * @author zer0
 * @date   2017-08-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_H__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_H__

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

TBAG_API tbBOOL tbCudaAdd1i(int      const * v1, int      const * v2, int      * r, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1u(unsigned const * v1, unsigned const * v2, unsigned * r, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1f(float    const * v1, float    const * v2, float    * r, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd1d(double   const * v1, double   const * v2, double   * r, int count, void * stream = TB_NULL);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_H__

