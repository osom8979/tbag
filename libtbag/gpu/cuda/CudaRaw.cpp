/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>

#if !defined(USE_CUDA)
tbBOOL tbCudaAdd1i(int       const * gpu_v1, int       const * gpu_v2, int      * gpu_result, unsigned size, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1u(unsigned  const * gpu_v1, unsigned  const * gpu_v2, unsigned * gpu_result, unsigned size, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1f(float     const * gpu_v1, float     const * gpu_v2, float    * gpu_result, unsigned size, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1d(double    const * gpu_v1, double    const * gpu_v2, double   * gpu_result, unsigned size, void * stream) { return TB_FALSE; }
#endif

