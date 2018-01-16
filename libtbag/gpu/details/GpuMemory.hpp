/**
 * @file   GpuMemory.hpp
 * @brief  GpuMemory class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/gpu/details/MemoryWrapper.hpp>

#include <cstdlib>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class HostMemory;
class GpuEvent;

/**
 * GpuMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuMemory : public MemoryWrapper
{
public:
    GpuMemory(GpuContext const * c = nullptr, GpuStream * s = nullptr);
    explicit GpuMemory(MemoryWrapper const & mem);
    GpuMemory(GpuMemory const & obj);
    GpuMemory(GpuMemory && obj);
    virtual ~GpuMemory();

public:
    GpuMemory & operator =(GpuMemory const & obj);
    GpuMemory & operator =(GpuMemory && obj);

public:
    void swap(GpuMemory & obj);
    inline friend void swap(GpuMemory & lh, GpuMemory & rh) { lh.swap(rh); }

public:
    inline void set(void * d, std::size_t c, std::size_t s) TBAG_NOEXCEPT
    { _data = d; _capacity = c; _size = s; }
    inline void clear() TBAG_NOEXCEPT
    { _data = nullptr; _capacity = 0; _size = 0; }

public:
    Err alloc(std::size_t size);
    Err free();

public:
    Err      copy( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err      copy(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync( GpuMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;
    Err copyAsync(HostMemory & memory, std::size_t size, GpuEvent * event = nullptr) const;

public:
    static GpuMemory instance(GpuContext const * c, std::size_t size);
};

using GpuMemories = std::vector<GpuMemory>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUMEMORY_HPP__

