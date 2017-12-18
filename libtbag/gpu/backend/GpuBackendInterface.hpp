/**
 * @file   GpuBackendInterface.hpp
 * @brief  GpuBackendInterface class prototype.
 * @author zer0
 * @date   2017-12-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKENDINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKENDINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

enum class GpuBackendType
{
    GBT_CPU,
    GBT_ACCEL,
    GBT_CUDA,
    GBT_OPENCL,
};

TBAG_CONSTEXPR std::size_t const STRING_TEMP_BUFFER_SIZE = 1024;

TBAG_CONSTEXPR char const * const PLATFORM_PROFILE    = "profile";
TBAG_CONSTEXPR char const * const PLATFORM_VERSION    = "version";
TBAG_CONSTEXPR char const * const PLATFORM_NAME       = "name";
TBAG_CONSTEXPR char const * const PLATFORM_VENDOR     = "vendor";
TBAG_CONSTEXPR char const * const PLATFORM_EXTENSIONS = "extensions";

struct GpuBackendInterface
{
    using StringMap = std::map<std::string, std::string>;

    virtual GpuBackendType getType() const TBAG_NOEXCEPT = 0;
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual int getDeviceCount() const = 0;

    virtual StringMap getPlatformInfo(int device_index) const
    { return StringMap(); }
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKENDINTERFACE_HPP__

