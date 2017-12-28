/**
 * @file   CpuBackend.hpp
 * @brief  CpuBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/backend/GpuBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

/**
 * CpuBackend class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
struct TBAG_API CpuBackend : public GpuBackend
{
    virtual GpuBackendType getType() const TBAG_NOEXCEPT override;
    virtual bool isSupport() const TBAG_NOEXCEPT override;

    virtual int       getPlatformCount() const override;
    virtual Platforms getPlatformList () const override;

    virtual int     getDeviceCount() const override;
    virtual Devices getDeviceList () const override;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_CPUBACKEND_HPP__

