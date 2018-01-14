/**
 * @file   CudaBackend.hpp
 * @brief  CudaBackend class prototype.
 * @author zer0
 * @date   2018-01-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDABACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDABACKEND_HPP__

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

namespace gpu  {
namespace cuda {

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int             getPlatformCount();
TBAG_API GpuPlatforms    getPlatformList ();
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

/**
 * CudaContext class prototype.
 *
 * @author zer0
 * @date   2018-01-13
 */
class TBAG_API CudaContext : public GpuContext
{
public:
    CudaContext(GpuDevice const & d, GpuId c);
    virtual ~CudaContext();

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual bool    isHost() const TBAG_NOEXCEPT override;
    virtual bool  isDevice() const TBAG_NOEXCEPT override;
    virtual bool  isStream() const TBAG_NOEXCEPT override;

    virtual GpuStream createStream() const override;
    virtual bool     releaseStream(GpuStream & stream) const override;

    virtual GpuEvent createEvent(GpuStream const & stream) const override;
    virtual bool       syncEvent(GpuEvent  const &  event) const override;
    virtual bool    elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const override;
    virtual bool    releaseEvent(GpuEvent        &  event) const override;

    virtual GpuMemory malloc(std::size_t size) const override;
    virtual bool        free(GpuMemory & memory) const override;

    virtual HostMemory mallocHost(std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const override;
    virtual bool         freeHost(HostMemory & memory) const override;

    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool writeAsync(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool  flush(GpuStream & stream) const override;
    virtual bool finish(GpuStream & stream) const override;
};

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDABACKEND_HPP__

