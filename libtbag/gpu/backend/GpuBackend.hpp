/**
 * @file   GpuBackend.hpp
 * @brief  GpuBackend class prototype.
 * @author zer0
 * @date   2017-12-17
 * @date   2017-12-27 (Rename: GpuBackendInterface -> GpuBackend)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/Assert.hpp>

#include <string>
#include <vector>
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

inline char const * getGpuBackendString(GpuBackendType type) TBAG_NOEXCEPT
{
    switch (type) {
    case GpuBackendType::GBT_CPU:     return "CPU";
    case GpuBackendType::GBT_ACCEL:   return "ACCEL";
    case GpuBackendType::GBT_CUDA:    return "CUDA";
    case GpuBackendType::GBT_OPENCL:  return "OPENCL";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;
    }
    return "UNKNOWN";
}

struct GpuDevice
{
    GpuBackendType type;
    int number;

    GpuDevice(GpuBackendType t, int n) : type(t), number(n) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }

    inline operator int() const TBAG_NOEXCEPT { return number; }
};

struct TBAG_API GpuBackend
{
    TBAG_CONSTEXPR static std::size_t const STRING_TEMP_BUFFER_SIZE = 1024;

    TBAG_CONSTEXPR static char const * const PLATFORM_PROFILE    = "profile";
    TBAG_CONSTEXPR static char const * const PLATFORM_VERSION    = "version";
    TBAG_CONSTEXPR static char const * const PLATFORM_NAME       = "name";
    TBAG_CONSTEXPR static char const * const PLATFORM_VENDOR     = "vendor";
    TBAG_CONSTEXPR static char const * const PLATFORM_EXTENSIONS = "extensions";

    using StringMap  = std::map<std::string, std::string>;
    using StringPair = StringMap::value_type;
    using Devices    = std::vector<GpuDevice>;

    virtual GpuBackendType     getType() const TBAG_NOEXCEPT = 0;
    virtual bool             isSupport() const TBAG_NOEXCEPT = 0;

    virtual int      getDeviceCount() const;
    virtual Devices   getDeviceList() const;

    virtual StringMap getPlatformInfo(GpuDevice const & device) const;

    char const * getTypeString() const TBAG_NOEXCEPT;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKEND_HPP__

