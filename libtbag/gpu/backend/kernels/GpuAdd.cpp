/**
 * @file   GpuAdd.cpp
 * @brief  GpuAdd class implementation.
 * @author zer0
 * @date   2018-01-07
 */

#include <libtbag/gpu/backend/kernels/GpuAdd.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {
namespace kernels {

#if !defined(USE_CUDA)
void addByCuda(float  const * v1, float  const * v2, float  * result, std::size_t count) { /* EMPTY. */ }
void addByCuda(double const * v1, double const * v2, double * result, std::size_t count) { /* EMPTY. */ }
#endif

} // namespace kernels
} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

