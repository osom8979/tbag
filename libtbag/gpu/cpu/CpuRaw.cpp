/**
 * @file   CpuRaw.cpp
 * @brief  CpuRaw class implementation.
 * @author zer0
 * @date   2018-01-10
 */

#include <libtbag/gpu/cpu/CpuRaw.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

template <typename T>
static void addByCpu(T const * v1, T const * v2, T * result, int count)
{
    for (int i = 0; i < count; ++i, ++v1, ++v2, ++result) {
        *result = *v1 + *v2;
    }
}

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

tbBOOL tbCpuAdd1f(float const * v1, float const * v2, float * result, int count)
{
    using namespace libtbag::gpu::cpu;
    addByCpu<float>(v1, v2, result, count);
    return true;
}

tbBOOL tbCpuAdd1d(double const * v1, double const * v2, double * result, int count)
{
    using namespace libtbag::gpu::cpu;
    addByCpu<double>(v1, v2, result, count);
    return true;
}

