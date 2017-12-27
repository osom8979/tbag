/**
 * @file   GpuBackend.cpp
 * @brief  GpuBackend class implementation.
 * @author zer0
 * @date   2017-12-27
 */

#include <libtbag/gpu/backend/GpuBackend.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

int GpuBackend::getDeviceCount() const
{
    return 0;
}

GpuBackend::Devices GpuBackend::getDeviceList() const
{
    Devices result;
    for (int i = 0; i < getDeviceCount(); ++i) {
        result.emplace_back(getType(), i);
    }
    return result;
}

GpuBackend::StringMap GpuBackend::getPlatformInfo(GpuDevice const & device) const
{
    return StringMap();
}

char const * GpuBackend::getTypeString() const TBAG_NOEXCEPT
{
    return getGpuBackendString(getType());
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

