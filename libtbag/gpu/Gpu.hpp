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
#include <libtbag/gpu/backend/GpuBackend.hpp>

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
    using Backend       = backend::GpuBackend;
    using BackendType   = backend::GpuBackendType;
    using GpuDevice     = backend::GpuDevice;
    using UniqueBackend = std::unique_ptr<Backend>;

    using StringMap  = Backend::StringMap;
    using StringPair = Backend::StringPair;
    using Devices    = Backend::Devices;

private:
    BackendType const TYPE;
    UniqueBackend _backend;

public:
    Gpu(BackendType type = BackendType::GBT_CPU);
    ~Gpu();

public:
    BackendType    getType() const TBAG_NOEXCEPT { return TYPE; }
    bool         isSupport() const TBAG_NOEXCEPT;
    int     getDeviceCount() const;
    Devices  getDeviceList() const;

    StringMap getPlatformInfo(GpuDevice const & device) const;
    std::string getTypeString() const;
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

