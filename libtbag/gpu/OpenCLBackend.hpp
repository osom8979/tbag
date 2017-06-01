/**
 * @file   OpenCLBackend.hpp
 * @brief  OpenCLBackend class prototype.
 * @author zer0
 * @date   2017-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCLBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCLBACKEND_HPP__

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
 * OpenCLBackend class prototype.
 *
 * @author zer0
 * @date   2017-04-18
 */
class TBAG_API OpenCLBackend : public GpuBackendInterface, private Noncopyable
{
public:
    OpenCLBackend();
    virtual ~OpenCLBackend();

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual int getDeviceCount() const override;
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCLBACKEND_HPP__

