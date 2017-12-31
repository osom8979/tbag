/**
 * @file   OpenCLBackend.hpp
 * @brief  OpenCLBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_OPENCLBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_OPENCLBACKEND_HPP__

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

TBAG_CONSTEXPR bool isOpenCLSupport() TBAG_NOEXCEPT
{
#if defined(USE_OPENCL)
    return true;
#else
    return false;
#endif
}

/**
 * OpenCLBackend class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
struct TBAG_API OpenCLBackend : public GpuBackend
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

    virtual GpuQueue createQueue(GpuContext const & context) const override;
    virtual bool    releaseQueue(GpuQueue         &   queue) const override;

    virtual GpuEvent createEvent(GpuQueue const & queue) const override;
    virtual bool       syncEvent(GpuEvent const & event) const override;
    virtual bool    elapsedEvent(GpuEvent       & event, float * millisec = nullptr) const override;
    virtual bool    releaseEvent(GpuEvent       & event) const override;

    virtual GpuMemory malloc(GpuContext const & context, std::size_t size) const override;
    virtual bool        free(GpuMemory & memory) const override;

    virtual HostMemory mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const override;
    virtual bool         freeHost(HostMemory & memory) const override;

    virtual bool write(GpuQueue & queue, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  read(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool enqueueWrite(GpuQueue & queue, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  enqueueRead(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool  flush(GpuQueue & queue) const override;
    virtual bool finish(GpuQueue & queue) const override;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_OPENCLBACKEND_HPP__

