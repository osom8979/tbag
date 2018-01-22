/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <libtbag/log/Log.hpp>

#if !defined(USE_CUDA)
tbBOOL tbCudaAdd_i(int       const * in1, int       const * in2, int      * out, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd_u(unsigned  const * in1, unsigned  const * in2, unsigned * out, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd_f(float     const * in1, float     const * in2, float    * out, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd_d(double    const * in1, double    const * in2, double   * out, int count, void * stream) { return TB_FALSE; }
#endif

