/**
 * @file   CpuContext.cpp
 * @brief  CpuContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/cpu/CpuContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

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
    // EMPTY.
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

template <typename T>
static Err addByCpu(T const * in1, T const * in2, T * out, int count, GpuEvent * event)
{
    CpuEventGuard const EVENT_LOCK(event);
    for (; count > 0; ++in1, ++in2, ++out, --count) {
        *out = *in1 + *in2;
    }
    return Err::E_SUCCESS;
}

Err CpuContext::add(GpuStream const & stream, int const * in1, int const * in2, int * out, int count, GpuEvent * event) const
{ return addByCpu(in1, in2, out, count, event); }
Err CpuContext::add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const
{ return addByCpu(in1, in2, out, count, event); }
Err CpuContext::add(GpuStream const & stream, float const * in1, float const * in2, float * out, int count, GpuEvent * event) const
{ return addByCpu(in1, in2, out, count, event); }
Err CpuContext::add(GpuStream const & stream, double const * in1, double const * in2, double * out, int count, GpuEvent * event) const
{ return addByCpu(in1, in2, out, count, event); }

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

