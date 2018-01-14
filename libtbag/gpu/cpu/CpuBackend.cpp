/**
 * @file   CpuBackend.cpp
 * @brief  CpuBackend class implementation.
 * @author zer0
 * @date   2018-01-13
 */

#include <libtbag/gpu/cpu/CpuBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/algorithm/Pack.hpp>
#include <libtbag/memory/AlignedMemory.hpp>

#include <cstdlib>
#include <chrono>

//#define TBAG_CPU_BACKEND_DEBUGGING

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

// ---------------
namespace __impl {
// ---------------

TBAG_CONSTEXPR static bool isCpuBackendVerbose() TBAG_NOEXCEPT
{
#if defined(TBAG_CPU_BACKEND_DEBUGGING)
    return true;
#else
    return false;
#endif
}

// ------------------
} // namespace __impl
// ------------------

bool isSupport() TBAG_NOEXCEPT
{
    return true;
}

int getPlatformCount()
{
    return 1;
}

GpuPlatforms getPlatformList()
{
    return {GpuPlatform(GpuType::GT_CPU, 0)};
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    GpuPlatformInfo info;
    info.name    = "CPU";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    return 1;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkCpuGpuType(platform);
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkCpuGpuType(device);
    GpuDeviceInfo info;
    info.name = "CPU";
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkCpuGpuType(device);
    return SharedGpuContext(new CpuContext(device, 0));
}

// --------------------------
// CpuContext implementation.
// --------------------------

CpuContext::CpuContext(GpuDevice const & d, GpuId c) : GpuContext(d, c)
{
    platform_info = cpu::getPlatformInfo(*this);
    device_info = cpu::getDeviceInfo(*this);
}

CpuContext::~CpuContext()
{
    // EMPTY.
}

// @formatter:off
bool CpuContext::isSupport() const TBAG_NOEXCEPT { return  true; }
bool CpuContext::isHost   () const TBAG_NOEXCEPT { return  true; }
bool CpuContext::isDevice () const TBAG_NOEXCEPT { return false; }
bool CpuContext::isStream () const TBAG_NOEXCEPT { return  true; }
// @formatter:on

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

