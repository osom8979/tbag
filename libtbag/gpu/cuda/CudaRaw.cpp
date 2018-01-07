/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>

#if !defined(USE_CUDA)
int tbCudaAdd1f(float  const * gpu_v1, float  const * gpu_v2, float  * gpu_result, int size, void * stream) { return TB_CUDA_FALSE; }
int tbCudaAdd1d(double const * gpu_v1, double const * gpu_v2, double * gpu_result, int size, void * stream) { return TB_CUDA_FALSE; }
#endif

