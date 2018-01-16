/**
 * @file   GpuCommon.hpp
 * @brief  GpuCommon class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/id/Id.hpp>

#include <exception>
#include <vector>
#include <string>
#include <map>

//#define TBAG_GPU_VERBOSE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

TBAG_CONSTEXPR bool isGpuVerbose() TBAG_NOEXCEPT
{
#if defined(TBAG_GPU_VERBOSE)
    return true;
#else
    return false;
#endif
}

using GpuId = id::Id;

TBAG_CONSTEXPR GpuId const UNKNOWN_ID = ::libtbag::id::UNKNOWN_ID;

enum class GpuType
{
    GT_CPU,
    GT_ACCEL,
    GT_CUDA,
    GT_OPENCL,
};

enum class HostMemoryFlag
{
    HMF_DEFAULT, ///< Aligned memory.
    HMF_PINNED,  ///< Page-locked memory.
};

TBAG_API char const * getGpuTypeString(GpuType type) TBAG_NOEXCEPT;
TBAG_API char const * getHostMemoryTypeString(HostMemoryFlag flag) TBAG_NOEXCEPT;

struct GpuPlatform
{
    GpuType const TYPE;
    GpuId   const PLATFORM;

    GpuPlatform(GpuType t, GpuId p) : TYPE(t), PLATFORM(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }

    inline GpuType getType() const TBAG_NOEXCEPT { return TYPE; }
    inline char const * getTypeString() const TBAG_NOEXCEPT { return getGpuTypeString(TYPE); }

    inline bool  existsPlatformId() const TBAG_NOEXCEPT { return PLATFORM != UNKNOWN_ID; }
    inline GpuId    getPlatformId() const TBAG_NOEXCEPT { return PLATFORM; }

    template <typename T> inline T castPlatformId() const TBAG_NOEXCEPT { return (T)PLATFORM; }
};

struct GpuPlatformInfo
{
    std::string  profile;
    std::string  name;
    std::string  vendor;
    std::string  version;
    std::string  extensions;
};

struct GpuDevice : public GpuPlatform
{
    GpuId const DEVICE;

    GpuDevice(GpuPlatform const & p, GpuId d) : GpuPlatform(p), DEVICE(d) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }

    inline bool  existsDeviceId() const TBAG_NOEXCEPT { return DEVICE != UNKNOWN_ID; }
    inline GpuId    getDeviceId() const TBAG_NOEXCEPT { return DEVICE; }

    template <typename T> inline T castDeviceId() const TBAG_NOEXCEPT { return (T)DEVICE; }
};

struct GpuDeviceInfo
{
    using Properties = std::map<std::string, std::string>;

    std::string  name;           ///< Device name.
    std::string  device_version; ///< Version string.
    std::string  driver_version; ///< Driver version string.
    Properties   properties;     ///< Extension properties.
    std::size_t  global_memory;  ///< Size of global device memory in bytes.

    GpuDeviceInfo() : global_memory(0) { /* EMPTY. */ }
    ~GpuDeviceInfo() { /* EMPTY. */ }

    template <typename ValueType>
    bool insert(std::string const & name, ValueType const & prop)
    { return insert(name, std::to_string(prop)); }

    bool insert(std::string const & name, char const * prop)
    { return insert(name, std::string(prop)); }

    bool insert(std::string const & name, std::string const & prop)
    { return properties.insert(Properties::value_type(name, prop)).second; }

    bool exists(std::string const & name) const
    { return properties.find(name) != properties.end(); }

    std::string get(std::string const & name) const
    {
        auto itr = properties.find(name);
        if (itr != properties.end()) {
            return itr->second;
        }
        return std::string();
    }
};

struct GpuTypeMismatchException : public std::exception
{
    virtual const char * what() const TBAG_NOEXCEPT override
    { return "Mismatch exception of GPU type."; }
};

inline void checkGpuType(GpuPlatform const & platform, GpuType type) throw(GpuTypeMismatchException)
{
    if (platform.TYPE != type) {
        throw GpuTypeMismatchException();
    }
}

// @formatter:off
inline void  checkAccelGpuType(GpuPlatform const & platform) { checkGpuType(platform, GpuType::GT_ACCEL);  }
inline void    checkCpuGpuType(GpuPlatform const & platform) { checkGpuType(platform, GpuType::GT_CPU);    }
inline void   checkCudaGpuType(GpuPlatform const & platform) { checkGpuType(platform, GpuType::GT_CUDA);   }
inline void checkOpenCLGpuType(GpuPlatform const & platform) { checkGpuType(platform, GpuType::GT_OPENCL); }
// @formatter:on

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;


} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCOMMON_HPP__

