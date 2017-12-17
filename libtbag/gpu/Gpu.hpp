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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/gpu/backend/GpuBackendInterface.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

/**
 * Gpu class prototype.
 *
 * @author zer0
 * @date   2017-12-17
 */
class TBAG_API Gpu : private Noncopyable
{
public:
    using BackendType      = backend::GpuBackendType;
    using BackendInterface = backend::GpuBackendInterface;
    using UniqueBackend    = std::unique_ptr<BackendInterface>;

private:
    BackendType const TYPE;
    UniqueBackend _backend;

public:
    Gpu(BackendType type = BackendType::GBT_CPU);
    ~Gpu();

public:
    BackendType getType() const TBAG_NOEXCEPT { return TYPE; }

public:
    bool isSupport() const;
    int  getDeviceCount() const;
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

