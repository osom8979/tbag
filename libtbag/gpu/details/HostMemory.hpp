/**
 * @file   HostMemory.hpp
 * @brief  HostMemory class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_HOSTMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_HOSTMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/gpu/details/MemoryWrapper.hpp>

#include <cstdlib>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

/**
 * HostMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API HostMemory : public MemoryWrapper
{
private:
    HostMemoryFlag _flag;

public:
    HostMemory(GpuContext const * c = nullptr);
    explicit HostMemory(MemoryWrapper const & mem);
    HostMemory(HostMemory const & obj);
    HostMemory(HostMemory && obj);
    virtual ~HostMemory();

public:
    HostMemory & operator =(HostMemory const & obj);
    HostMemory & operator =(HostMemory && obj);

public:
    void swap(HostMemory & obj);
    inline friend void swap(HostMemory & lh, HostMemory & rh) { lh.swap(rh); }

public:
    inline void set(void * d, std::size_t c, std::size_t s, HostMemoryFlag f) TBAG_NOEXCEPT
    { _data = d; _capacity = c; _size = s; _flag = f; }
    inline void clear() TBAG_NOEXCEPT
    { _data = nullptr; _capacity = 0; _size = 0; _flag = HostMemoryFlag::HMF_DEFAULT; }

public:
    Err alloc(std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT);
    Err free();

public:
    static HostMemory instance(GpuContext const * c, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT);
};

using HostMemories = std::vector<HostMemory>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_HOSTMEMORY_HPP__

