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
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <cstdlib>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// Forward declaration.
class GpuStream;
class GpuEvent;
class GpuProgram;
class GpuKernel;
class GpuMemory;
class HostMemory;
class MemoryWrapper;

/**
 * GpuContext structure.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API GpuContext : public GpuDevice, private Noncopyable
{
public:
    using TypeTable = type::TypeTable;

public:
    GpuId const CONTEXT_ID;

public:
    GpuContext(GpuDevice const & d, GpuId c);
    virtual ~GpuContext();

public:
    inline bool  existsContextId() const TBAG_NOEXCEPT { return CONTEXT_ID != UNKNOWN_ID; }
    inline GpuId    getContextId() const TBAG_NOEXCEPT { return CONTEXT_ID; }

    template <typename T> inline T castContextId() const TBAG_NOEXCEPT { return (T)CONTEXT_ID; }

public:
    template <typename Stream, typename Mem1, typename Mem2>
    inline bool validateMemory(Stream const & stream, Mem1 const & mem1, Mem2 const & mem2,
                               std::size_t size) const TBAG_NOEXCEPT
    {
        return stream.validate(*this)
               && mem1.validate(stream) && size <= mem1.size()
               && mem2.validate(stream) && size <= mem2.size();
    }

public:
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool    isHost() const TBAG_NOEXCEPT = 0;
    virtual bool  isDevice() const TBAG_NOEXCEPT = 0;
    virtual bool  isStream() const TBAG_NOEXCEPT = 0;

// Object.
public:
    virtual Err  createStream(GpuStream & stream) const;
    virtual Err releaseStream(GpuStream & stream) const;

    virtual Err  createEvent(GpuStream const & stream, GpuEvent & event) const;
    virtual Err    syncEvent(GpuEvent  const &  event) const;
    virtual Err elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const;
    virtual Err releaseEvent(GpuEvent        &  event) const;

    virtual Err  createProgram(std::string const &  source, GpuProgram & program) const;
    virtual Err   buildProgram(GpuProgram        & program) const;
    virtual Err releaseProgram(GpuProgram        & program) const;

    virtual Err  createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const;
    virtual Err releaseKernel(GpuKernel        & kernel) const;

// Memory.
public:
    virtual Err malloc(GpuMemory & mem, std::size_t size) const;
    virtual Err   free(GpuMemory & mem) const;

    virtual Err mallocHost(HostMemory & mem, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const;
    virtual Err   freeHost(HostMemory & mem) const;

// Input/Output.
public:
    virtual Err write(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual Err  read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    virtual Err writeAsync(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual Err  readAsync(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    virtual Err copy(GpuStream const & stream,  GpuMemory const & src,  GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const;
    virtual Err copy(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event = nullptr) const;

    virtual Err copyAsync(GpuStream const & stream,  GpuMemory const & src,  GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const;
    virtual Err copyAsync(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event = nullptr) const;

// Memory.
public:
    virtual Err  flush(GpuStream const & stream) const;
    virtual Err finish(GpuStream const & stream) const;

// Kernels.
public:
    virtual Err add(GpuStream const & stream, int      const * in1, int      const * in2, int      * out, int count, GpuEvent * event) const { return Err::E_UNSUPOP; }
    virtual Err add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const { return Err::E_UNSUPOP; }
    virtual Err add(GpuStream const & stream, float    const * in1, float    const * in2, float    * out, int count, GpuEvent * event) const { return Err::E_UNSUPOP; }
    virtual Err add(GpuStream const & stream, double   const * in1, double   const * in2, double   * out, int count, GpuEvent * event) const { return Err::E_UNSUPOP; }

    Err add(GpuStream const & stream, TypeTable type, GpuMemory const & in1, GpuMemory const & in2, GpuMemory & out, int count, GpuEvent * event = nullptr);
};

using SharedGpuContext = std::shared_ptr<GpuContext>;
using WeakedGpuContext = std::weak_ptr<GpuContext>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCONTEXT_HPP__

