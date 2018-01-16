/**
 * @file   OpenCLBackend.hpp
 * @brief  OpenCLBackend class prototype.
 * @author zer0
 * @date   2018-01-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuDetails.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int             getPlatformCount();
TBAG_API GpuPlatforms    getPlatformList ();
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

/**
 * OpenCLContext class prototype.
 *
 * @author zer0
 * @date   2018-01-13
 */
class TBAG_API OpenCLContext : public GpuContext
{
public:
    OpenCLContext(GpuDevice const & d, GpuId c);
    virtual ~OpenCLContext();

private:
    bool _write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem,
                std::size_t size, bool blocking = true, GpuEvent * event = nullptr) const;
    bool _read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem,
               std::size_t size, bool blocking = true, GpuEvent * event = nullptr) const;

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual bool    isHost() const TBAG_NOEXCEPT override;
    virtual bool  isDevice() const TBAG_NOEXCEPT override;
    virtual bool  isStream() const TBAG_NOEXCEPT override;

    virtual bool  createStream(GpuStream & stream) const override;
    virtual bool releaseStream(GpuStream & stream) const override;

    virtual bool  createEvent(GpuStream const & stream, GpuEvent & event) const override;
    virtual bool    syncEvent(GpuEvent  const &  event) const override;
    virtual bool elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const override;
    virtual bool releaseEvent(GpuEvent        &  event) const override;

    virtual bool  createProgram(std::string const &  source, GpuProgram & program) const override;
    virtual bool   buildProgram(GpuProgram        & program) const override;
    virtual bool releaseProgram(GpuProgram        & program) const override;

    virtual bool  createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const override;
    virtual bool releaseKernel(GpuKernel        & kernel) const override;

    virtual bool malloc(GpuMemory & memory, std::size_t size) const override;
    virtual bool   free(GpuMemory & memory) const override;

    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool writeAsync(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual bool  readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual bool  flush(GpuStream & stream) const override;
    virtual bool finish(GpuStream & stream) const override;
};

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLBACKEND_HPP__

