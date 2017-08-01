/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <cassert>

int tbCudaAddByCpu(int const * lh, int const * rh, int * result, int size)
{
    for (int i = 0; i < size; ++i) {
        *(result + i) = *(lh + i) + *(rh + i);
    }
    return TB_CUDA_TRUE;
}

// -------------------
#if !defined(USE_CUDA)
// -------------------

int tbCudaAddByGpu(int const * lh, int const * rh, int * result, int size)
{
    assert(false && "Unsupported operation.");
    return TB_CUDA_FALSE;
}

// -------------------------
#endif // !defined(USE_CUDA)
// -------------------------

