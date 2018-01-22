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

TBAG_API tbBOOL tbCudaAdd_i(int      const * in1, int      const * in2, int      * out, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd_u(unsigned const * in1, unsigned const * in2, unsigned * out, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd_f(float    const * in1, float    const * in2, float    * out, int count, void * stream = TB_NULL);
TBAG_API tbBOOL tbCudaAdd_d(double   const * in1, double   const * in2, double   * out, int count, void * stream = TB_NULL);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDARAW_H__

