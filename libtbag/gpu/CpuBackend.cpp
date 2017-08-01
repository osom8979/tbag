/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

CpuBackend::CpuBackend()
{
    // EMPTY.
}

CpuBackend::~CpuBackend()
{
    // EMPTY.
}

BackendType CpuBackend::getType() const TBAG_NOEXCEPT
{
    return BackendType::BT_CPU;
}

bool CpuBackend::isSupport() const TBAG_NOEXCEPT
{
    return true;
}

int CpuBackend::getDeviceCount() const
{
    return static_cast<int>(uvpp::getCpuInfos().size());
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

