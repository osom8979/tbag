/**
 * @file   AccelBackend.hpp
 * @brief  AccelBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_ACCELBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_ACCELBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/backend/CpuBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

/**
 * AccelBackend class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
struct TBAG_API AccelBackend : public CpuBackend
{
    virtual GpuBackendType getType() const TBAG_NOEXCEPT override;

    virtual GpuMemory malloc(GpuContext const & context, std::size_t size) const override;
    virtual bool        free(GpuMemory & memory) const override;

    virtual HostMemory mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const override;
    virtual bool         freeHost(HostMemory & memory) const override;

    virtual bool runAdd(GpuStream & stream, GpuMemory const & v1, GpuMemory const & v2, GpuMemory & result,
                        type::TypeTable type, int count, GpuEvent * event = nullptr) const override;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_ACCELBACKEND_HPP__

