/**
 * @file   GpuContext.hpp
 * @brief  GpuContext class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCONTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <cstdlib>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuStream;
class GpuEvent;
class GpuProgram;
class GpuKernel;
class GpuMemory;
class HostMemory;

/**
 * GpuContext structure.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API GpuContext : public GpuDevice
{
public:
    GpuId const CONTEXT;

public:
    GpuContext(GpuDevice const & d, GpuId c);
    virtual ~GpuContext();

public:
    bool validateMemory(GpuStream const & stream, GpuMemory const & gpu_mem,
                        HostMemory const & host_mem, std::size_t size) const TBAG_NOEXCEPT;

public:
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool    isHost() const TBAG_NOEXCEPT = 0;
    virtual bool  isDevice() const TBAG_NOEXCEPT = 0;
    virtual bool  isStream() const TBAG_NOEXCEPT = 0;

// Object.
public:
    virtual bool  createStream(GpuStream & stream) const;
    virtual bool releaseStream(GpuStream & stream) const;

    virtual bool  createEvent(GpuStream const & stream, GpuEvent & event) const;
    virtual bool    syncEvent(GpuEvent  const &  event) const;
    virtual bool elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const;
    virtual bool releaseEvent(GpuEvent        &  event) const;

    virtual bool  createProgram(std::string const &  source, GpuProgram & program) const;
    virtual bool   buildProgram(GpuProgram        & program) const;
    virtual bool releaseProgram(GpuProgram        & program) const;

    virtual bool  createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const;
    virtual bool releaseKernel(GpuKernel        & kernel) const;

// Memory.
public:
    virtual bool malloc(GpuMemory & memory, std::size_t size) const;
    virtual bool   free(GpuMemory & memory) const;

    virtual bool mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const;
    virtual bool   freeHost(HostMemory & memory) const;

// Input/Output.
public:
    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    virtual bool writeAsync(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

// Memory.
public:
    virtual bool  flush(GpuStream & stream) const;
    virtual bool finish(GpuStream & stream) const;
};

using SharedGpuContext = std::shared_ptr<GpuContext>;
using WeakedGpuContext = std::weak_ptr<GpuContext>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCONTEXT_HPP__

