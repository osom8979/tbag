/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>

#if !defined(USE_CUDA)
tbBOOL tbCudaAdd1i(int       const * v1, int       const * v2, int      * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1u(unsigned  const * v1, unsigned  const * v2, unsigned * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1f(float     const * v1, float     const * v2, float    * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1d(double    const * v1, double    const * v2, double   * r, int count, void * stream) { return TB_FALSE; }
#endif

