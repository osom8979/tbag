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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
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

// @formatter:off
template <typename T> struct GpuMemoryTypeSuffix;
template <> struct GpuMemoryTypeSuffix<     int> { TBAG_CONSTEXPR static char const * const prefix = "i"; };
template <> struct GpuMemoryTypeSuffix<unsigned> { TBAG_CONSTEXPR static char const * const prefix = "u"; };
template <> struct GpuMemoryTypeSuffix<   float> { TBAG_CONSTEXPR static char const * const prefix = "f"; };
template <> struct GpuMemoryTypeSuffix<  double> { TBAG_CONSTEXPR static char const * const prefix = "d"; };
// @formatter:on

struct GpuPlatform
{
    GpuType const TYPE;
    GpuId   const PLATFORM_ID;

    GpuPlatform(GpuType t, GpuId p) : TYPE(t), PLATFORM_ID(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }

    inline GpuType getType() const TBAG_NOEXCEPT { return TYPE; }
    inline char const * getTypeString() const TBAG_NOEXCEPT { return getGpuTypeString(TYPE); }

    inline bool existsPlatformId() const TBAG_NOEXCEPT { return PLATFORM_ID != UNKNOWN_ID; }
    inline GpuId   getPlatformId() const TBAG_NOEXCEPT { return PLATFORM_ID; }

    template <typename T> inline T castPlatformId() const TBAG_NOEXCEPT { return (T)PLATFORM_ID; }
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
    GpuId const DEVICE_ID;

    GpuDevice(GpuPlatform const & p, GpuId d) : GpuPlatform(p), DEVICE_ID(d) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }

    inline bool existsDeviceId() const TBAG_NOEXCEPT { return DEVICE_ID!= UNKNOWN_ID; }
    inline GpuId   getDeviceId() const TBAG_NOEXCEPT { return DEVICE_ID; }

    template <typename T> inline T castDeviceId() const TBAG_NOEXCEPT { return (T)DEVICE_ID; }
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

// Forward declaration.
class GpuContext;

/**
 * GpuIdWrapper structure.
 *
 * @author zer0
 * @date   2018-01-15
 */
class GpuIdWrapper : private Noncopyable
{
private:
    GpuContext const & _context;

protected:
    GpuId _id;

public:
    GpuIdWrapper(GpuContext const & context, GpuId id = UNKNOWN_ID) : _context(context), _id(id)
    { /* EMPTY. */ }
    ~GpuIdWrapper()
    { /* EMPTY. */ }

public:
    inline bool isSameContext(GpuContext const & context) const TBAG_NOEXCEPT { return (&_context) == (&context); }
    inline GpuContext const & atContext() const TBAG_NOEXCEPT { return _context; }

    inline bool validate() const TBAG_NOEXCEPT { return _id != UNKNOWN_ID; }
    inline bool validate(GpuContext const & context) const TBAG_NOEXCEPT { return isSameContext(context) && validate(); }

    inline GpuId getId() const TBAG_NOEXCEPT { return _id; }
    inline void clearId() TBAG_NOEXCEPT { _id = UNKNOWN_ID; }

    template <typename T> inline void setId(T i)    TBAG_NOEXCEPT { _id = (GpuId)i; }
    template <typename T> inline T   castId() const TBAG_NOEXCEPT { return (T)_id; }
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUCOMMON_HPP__

