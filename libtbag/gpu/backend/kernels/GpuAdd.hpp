/**
 * @file   GpuAdd.hpp
 * @brief  GpuAdd class prototype.
 * @author zer0
 * @date   2018-01-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {
namespace kernels {

template <typename T>
void addByCpu(T const * v1, T const * v2, T * result, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i, ++v1, ++v2, ++result) {
        *result = *v1 + *v2;
    }
}

template <typename T>
void addByCpu2(void const * v1, void const * v2, void * result, std::size_t count)
{
    addByCpu<T>((T const *)v1, (T const *)v2, (T*)result, count);
}

TBAG_API void addByCuda(float  const * v1, float  const * v2, float  * result, std::size_t count);
TBAG_API void addByCuda(double const * v1, double const * v2, double * result, std::size_t count);

} // namespace kernels
} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_KERNELS_GPUADD_HPP__

