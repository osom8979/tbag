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
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

#if defined(TBAG_COMP_MSVC)
# if defined(min)
TBAG_PUSH_MACRO(min);
# undef min
# define __RESTORE_MIN__
# endif // defined(min)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif // defined(TBAG_COMP_MSVC)

using GpuId = std::size_t;
TBAG_CONSTEXPR GpuId const UNKNOWN_GPU_ID = std::numeric_limits<GpuId>::max();

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MIN__)
TBAG_POP_MACRO(min);
# undef __RESTORE_MIN__
# endif // defined(__RESTORE_MIN__)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif // defined(TBAG_COMP_MSVC)

enum class GpuBackendType
{
    GBT_CPU,
    GBT_ACCEL,
    GBT_CUDA,
    GBT_OPENCL,
};

TBAG_API char const * getGpuBackendString(GpuBackendType type) TBAG_NOEXCEPT;

struct GpuPlatform
{
    GpuBackendType type;
    GpuId platform_id;

    GpuPlatform(GpuBackendType t = GpuBackendType::GBT_CPU, GpuId p = UNKNOWN_GPU_ID) : type(t), platform_id(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }

    inline bool isUnknownPlatform() const TBAG_NOEXCEPT { return platform_id == UNKNOWN_GPU_ID; }
};

struct GpuPlatformInfo : public GpuPlatform
{
    std::string profile;
    std::string name;
    std::string vendor;
    std::string version;
    std::string extensions;

    GpuPlatformInfo() : GpuPlatformInfo(GpuPlatform()) { /* EMPTY. */ }
    GpuPlatformInfo(GpuPlatform const & p) : GpuPlatform(p) { /* EMPTY. */ }
    ~GpuPlatformInfo() { /* EMPTY. */ }
};

struct GpuDevice : public GpuPlatform
{
    GpuId device_id;

    GpuDevice() : GpuDevice(GpuPlatform()) { /* EMPTY. */ }
    GpuDevice(GpuPlatform const & p, GpuId d = UNKNOWN_GPU_ID) : GpuPlatform(p), device_id(d) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }

    inline bool isUnknownDevice() const TBAG_NOEXCEPT { return device_id == UNKNOWN_GPU_ID; }
};

struct GpuDeviceInfo : public GpuDevice
{
    std::string name;

    GpuDeviceInfo() : GpuDeviceInfo(GpuDevice()) { /* EMPTY. */ }
    GpuDeviceInfo(GpuDevice const & d) : GpuDevice(d) { /* EMPTY. */ }
    ~GpuDeviceInfo() { /* EMPTY. */ }
};

struct GpuContext : public GpuDevice
{
    GpuId context_id;

    GpuContext() : GpuContext(GpuDevice()) { /* EMPTY. */ }
    GpuContext(GpuDevice const & d, GpuId c = UNKNOWN_GPU_ID) : GpuDevice(d), context_id(c) { /* EMPTY. */ }
    ~GpuContext() { /* EMPTY. */ }

    inline bool isUnknownContext() const TBAG_NOEXCEPT { return context_id == UNKNOWN_GPU_ID; }
};

struct GpuMemory : public GpuContext
{
    void * data;
    std::size_t size;

    GpuMemory() : GpuMemory(GpuContext()) { /* EMPTY. */ }
    GpuMemory(GpuContext const & c) : GpuContext(c), data(nullptr), size(0) { /* EMPTY. */ }
    ~GpuMemory() { /* EMPTY. */ }

    inline bool existsMemory() const TBAG_NOEXCEPT { return data != nullptr && size > 0U; }
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;
using GpuContexts  = std::vector<GpuContext>;

struct GpuTypeMismatchException : public std::exception
{
    virtual const char * what() const TBAG_NOEXCEPT override
    { return "Gpu type mismatch exception"; }
};

struct TBAG_API GpuBackend
{
    virtual GpuBackendType  getType() const TBAG_NOEXCEPT = 0;
    virtual bool          isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool             isHost() const TBAG_NOEXCEPT = 0;
    virtual bool           isDevice() const TBAG_NOEXCEPT = 0;

    virtual int             getPlatformCount() const = 0;
    virtual GpuPlatforms    getPlatformList () const = 0;
    virtual GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform) const = 0;

    virtual int             getDeviceCount(GpuPlatform const & platform) const = 0;
    virtual GpuDevices      getDeviceList (GpuPlatform const & platform) const = 0;
    virtual GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device)   const = 0;

    virtual GpuContext createContext(GpuDevice const &  device) const = 0;
    virtual bool      releaseContext(GpuContext      & context) const = 0;

    virtual GpuMemory malloc(GpuContext const & context, std::size_t size) const = 0;
    virtual bool free(GpuMemory & memory) const = 0;

    // --------------------
    // Non-virtual methods.
    // --------------------

    void checkType(GpuBackendType type) const throw(GpuTypeMismatchException);
    char const * getTypeString() const TBAG_NOEXCEPT;

    GpuContext createContext(int platform_index, int device_index) const;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKEND_HPP__

