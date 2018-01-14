/**
 * @file   Gpu.hpp
 * @brief  Gpu class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/GpuContext.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

/**
 * Gpu class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API Gpu
{
private:
    SharedGpuContext _gpu;

public:
    Gpu();
    Gpu(GpuType type, std::size_t platform_index = 0, std::size_t device_index = 0);
    explicit Gpu(SharedGpuContext const & gpu);
    explicit Gpu(WeakedGpuContext const & gpu);
    Gpu(Gpu const & obj);
    Gpu(Gpu && obj);
    ~Gpu();

public:
    Gpu & operator =(Gpu const & obj);
    Gpu & operator =(Gpu && obj);

public:
    void swap(Gpu & obj);
    friend void swap(Gpu & lh, Gpu & rh) { lh.swap(rh); }

public:
    bool init(GpuType type = GpuType::GT_CPU, std::size_t platform_index = 0, std::size_t device_index = 0);
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

