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
    return GpuBackendType::GBT_CPU;
}

bool AccelBackend::isSupport() const TBAG_NOEXCEPT
{
    return true;
}

int AccelBackend::getDeviceCount() const
{
    return static_cast<int>(uvpp::getCpuInfos().size());
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

