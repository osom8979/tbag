/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType CpuBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_CPU;
}

bool CpuBackend::isSupport() const TBAG_NOEXCEPT
{
    return true;
}

int CpuBackend::getDeviceCount() const
{
    return static_cast<int>(uvpp::getCpuInfos().size());
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

