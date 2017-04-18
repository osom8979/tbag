/**
 * @file   GpuBackend.hpp
 * @brief  GpuBackend class prototype.
 * @author zer0
 * @date   2017-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

struct GpuBackendInterface
{
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual int getDeviceCount() const = 0;
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUBACKEND_HPP__

