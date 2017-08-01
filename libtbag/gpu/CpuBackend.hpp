/**
 * @file   CpuBackend.hpp
 * @brief  CpuBackend class prototype.
 * @author zer0
 * @date   2017-08-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/gpu/GpuBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

/**
 * CpuBackend class prototype.
 *
 * @author zer0
 * @date   2017-08-01
 */
class TBAG_API CpuBackend : public GpuBackendInterface, private Noncopyable
{
public:
    CpuBackend();
    virtual ~CpuBackend();

public:
    virtual BackendType getType() const TBAG_NOEXCEPT override;
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual int getDeviceCount() const override;
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPUBACKEND_HPP__

