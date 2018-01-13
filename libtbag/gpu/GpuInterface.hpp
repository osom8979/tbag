/**
 * @file   GpuInterface.hpp
 * @brief  GpuInterface class prototype.
 * @author zer0
 * @date   2018-01-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/id/Id.hpp>

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <exception>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

using GpuId = id::Id;

enum class GpuType
{
    GT_CPU,
    GT_ACCEL,
    GT_CUDA,
    GT_OPENCL,
};

TBAG_CONSTEXPR GpuId const UNKNOWN_ID = ::libtbag::id::UNKNOWN_ID;

inline char const * getGpuTypeString(GpuType type) TBAG_NOEXCEPT
{
    switch (type) {
    case GpuType::GT_CPU:     return "CPU";
    case GpuType::GT_ACCEL:   return "ACCEL";
    case GpuType::GT_CUDA:    return "CUDA";
    case GpuType::GT_OPENCL:  return "OPENCL";
    default: break;
    }
    return "UNKNOWN";
}

struct GpuPlatform
{
    GpuType const TYPE;
    GpuId   const PLATFORM;

    GpuPlatform(GpuType t, GpuId p) : TYPE(t), PLATFORM(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }
};

struct GpuPlatformInfo
{
    std::string profile;
    std::string name;
    std::string vendor;
    std::string version;
    std::string extensions;
};

struct GpuDevice : public GpuPlatform
{
    GpuId const DEVICE;

    GpuDevice(GpuPlatform const & p, GpuId d) : GpuPlatform(p), DEVICE(d) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }
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

struct GpuContext : public GpuDevice
{
    GpuId const CONTEXT;

    GpuContext(GpuDevice const & d, GpuId c) : GpuDevice(d), CONTEXT(c) { /* EMPTY. */ }
    ~GpuContext() { /* EMPTY. */ }

    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool    isHost() const TBAG_NOEXCEPT = 0;
    virtual bool  isDevice() const TBAG_NOEXCEPT = 0;
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;

using SharedGpuContext = std::shared_ptr<GpuContext>;
using   WeakGpuContext =   std::weak_ptr<GpuContext>;

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

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUINTERFACE_HPP__

