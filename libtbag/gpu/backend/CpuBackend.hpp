/**
 * @file   CpuBackend.hpp
 * @brief  CpuBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/backend/GpuBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

/**
 * CpuBackend class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
struct TBAG_API CpuBackend : public GpuBackend
{
    virtual GpuBackendType  getType() const TBAG_NOEXCEPT override;
    virtual bool          isSupport() const TBAG_NOEXCEPT override;
    virtual bool             isHost() const TBAG_NOEXCEPT override;
    virtual bool           isDevice() const TBAG_NOEXCEPT override;

    virtual int             getPlatformCount() const override;
    virtual GpuPlatforms    getPlatformList () const override;
    virtual GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform) const override;

    virtual int             getDeviceCount(GpuPlatform const & platform) const override;
    virtual GpuDevices      getDeviceList (GpuPlatform const & platform) const override;
    virtual GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device)   const override;

    virtual GpuContext createContext(GpuDevice const &  device) const override;
    virtual bool      releaseContext(GpuContext      & context) const override;

    virtual GpuMemory malloc(GpuContext const & context, std::size_t size) const override;
    virtual bool free(GpuMemory & memory) const override;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__

