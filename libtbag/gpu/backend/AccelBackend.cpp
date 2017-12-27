/**
 * @file   AccelBackend.cpp
 * @brief  AccelBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/AccelBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType AccelBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_ACCEL;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

