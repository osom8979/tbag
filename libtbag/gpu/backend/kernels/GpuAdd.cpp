/**
 * @file   GpuAdd.cpp
 * @brief  GpuAdd class implementation.
 * @author zer0
 * @date   2018-01-07
 */

#include <libtbag/gpu/backend/kernels/GpuAdd.hpp>
#include <libtbag/gpu/cuda/CudaRaw.h>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {
namespace kernels {

// ---------------
namespace __impl {
// ---------------

template <typename T>
void addByCpu(T const * v1, T const * v2, T * result, int count)
{
    for (int i = 0; i < count; ++i, ++v1, ++v2, ++result) {
        *result = *v1 + *v2;
    }
}

// ------------------
} // namespace __impl
// ------------------

bool addByCpu(float const * v1, float const * v2, float * result, int count)
{
    __impl::addByCpu<float>(v1, v2, result, count);
    return true;
}

bool addByCpu(double const * v1, double const * v2, double * result, int count)
{
    __impl::addByCpu<double>(v1, v2, result, count);
    return true;
}

bool addByCuda(float const * v1, float const * v2, float * result, int count)
{
    return tbCudaAdd1f(v1, v2, result, count) == TB_CUDA_TRUE;
}

bool addByCuda(double const * v1, double const * v2, double * result, int count)
{
    return tbCudaAdd1d(v1, v2, result, count) == TB_CUDA_TRUE;
}

} // namespace kernels
} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

