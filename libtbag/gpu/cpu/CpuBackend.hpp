/**
 * @file   CpuBackend.hpp
 * @brief  CpuBackend class prototype.
 * @author zer0
 * @date   2018-01-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/Gpu.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int             getPlatformCount();
TBAG_API GpuPlatforms    getPlatformList ();
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

/**
 * CpuContext class prototype.
 *
 * @author zer0
 * @date   2018-01-13
 */
class TBAG_API CpuContext : public GpuContext
{
public:
    CpuContext(GpuDevice const & d, GpuId c);
    virtual ~CpuContext();

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual bool    isHost() const TBAG_NOEXCEPT override;
    virtual bool  isDevice() const TBAG_NOEXCEPT override;
    virtual bool  isStream() const TBAG_NOEXCEPT override;
};

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPUBACKEND_HPP__

