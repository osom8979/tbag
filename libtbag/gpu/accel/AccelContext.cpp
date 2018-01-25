/**
 * @file   AccelContext.cpp
 * @brief  AccelContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/accel/AccelContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/gpu/accel/AccelRaw.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu   {
namespace accel {

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
    return {GpuPlatform(GpuType::GT_ACCEL, 0)};
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkAccelGpuType(platform);
    GpuPlatformInfo info;
    info.name    = "ACCEL";
    info.vendor  = "TBAG";
    info.version = util::getTbagVersion().toLongString();
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkAccelGpuType(platform);
    return 1;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkAccelGpuType(platform);
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkAccelGpuType(device);
    GpuDeviceInfo info;
    info.name = "ACCEL";
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkAccelGpuType(device);
    return SharedGpuContext(new AccelContext(device, 0));
}

// ----------------------------
// AccelContext implementation.
// ----------------------------

AccelContext::AccelContext(GpuDevice const & d, GpuId c) : cpu::CpuContext(d, c)
{
    // EMPTY.
}

AccelContext::~AccelContext()
{
    // EMPTY.
}

// @formatter:off
bool AccelContext::isSupport() const TBAG_NOEXCEPT { return  true; }
bool AccelContext::isHost   () const TBAG_NOEXCEPT { return  true; }
bool AccelContext::isDevice () const TBAG_NOEXCEPT { return false; }
bool AccelContext::isStream () const TBAG_NOEXCEPT { return  true; }
// @formatter:on

Err AccelContext::add(GpuStream const & stream, int const * in1, int const * in2, int * out, int count, GpuEvent * event) const
{ CpuEventGuard g(event); return tbAccelAdd_i(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err AccelContext::add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const
{ CpuEventGuard g(event); return tbAccelAdd_u(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err AccelContext::add(GpuStream const & stream, float const * in1, float const * in2, float * out, int count, GpuEvent * event) const
{ CpuEventGuard g(event); return tbAccelAdd_f(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }
Err AccelContext::add(GpuStream const & stream, double const * in1, double const * in2, double * out, int count, GpuEvent * event) const
{ CpuEventGuard g(event); return tbAccelAdd_d(in1, in2, out, count) == TB_TRUE ? Err::E_SUCCESS : Err::E_UNKNOWN; }

} // namespace accel
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

