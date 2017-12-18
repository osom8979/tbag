/**
 * @file   CudaBackend.hpp
 * @brief  CudaBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CUDABACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CUDABACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/backend/GpuBackendInterface.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

TBAG_CONSTEXPR bool isCudaSupport() TBAG_NOEXCEPT
{
#if defined(USE_CUDA)
    return true;
#else
    return false;
#endif
}

/**
 * CudaBackend class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
struct TBAG_API CudaBackend : public GpuBackendInterface
{
    virtual GpuBackendType getType() const TBAG_NOEXCEPT override;
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual int getDeviceCount() const override;
    virtual StringMap getPlatformInfo(int device_index) const override;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CUDABACKEND_HPP__

