/**
 * @file   OpenCLContext.hpp
 * @brief  OpenCLContext class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLCONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLCONTEXT_HPP__

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
    struct Kernels
    {
        SharedGpuKernel i;
        SharedGpuKernel u;
        SharedGpuKernel f;
        SharedGpuKernel d;
    };

public:
    OpenCLContext(GpuDevice const & d, GpuId c);
    virtual ~OpenCLContext();

private:
    Err _write(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem,
               std::size_t size, bool blocking = true, GpuEvent * event = nullptr) const;
    Err _read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem,
              std::size_t size, bool blocking = true, GpuEvent * event = nullptr) const;

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual bool    isHost() const TBAG_NOEXCEPT override;
    virtual bool  isDevice() const TBAG_NOEXCEPT override;
    virtual bool  isStream() const TBAG_NOEXCEPT override;

    virtual Err  createStream(GpuStream & stream) const override;
    virtual Err releaseStream(GpuStream & stream) const override;

    virtual Err  createEvent(GpuStream const & stream, GpuEvent & event) const override;
    virtual Err    syncEvent(GpuEvent  const &  event) const override;
    virtual Err elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const override;
    virtual Err releaseEvent(GpuEvent        &  event) const override;

    virtual Err  createProgram(std::string const &  source, GpuProgram & program) const override;
    virtual Err   buildProgram(GpuProgram        & program) const override;
    virtual Err releaseProgram(GpuProgram        & program) const override;

    virtual Err  createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const override;
    virtual Err releaseKernel(GpuKernel        & kernel) const override;

    virtual Err malloc(GpuMemory & mem, std::size_t size) const override;
    virtual Err   free(GpuMemory & mem) const override;

    virtual Err write(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err  read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err writeAsync(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err  readAsync(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err      copy(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err copyAsync(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err  flush(GpuStream const & stream) const override;
    virtual Err finish(GpuStream const & stream) const override;

private:
    Kernels mutable _fill;
public:
    virtual Err fill(GpuStream const & stream, int      * out, int      data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, unsigned * out, unsigned data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, float    * out, float    data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, double   * out, double   data, int count, GpuEvent * event) const override;

private:
    Kernels mutable _add;
public:
    virtual Err add(GpuStream const & stream, int      const * in1, int      const * in2, int      * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, float    const * in1, float    const * in2, float    * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, double   const * in1, double   const * in2, double   * out, int count, GpuEvent * event) const override;
};

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLCONTEXT_HPP__

