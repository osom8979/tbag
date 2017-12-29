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

TBAG_CONSTEXPR char const * const TBAG_GPU_PLATFORM_PROFILE    = "profile";
TBAG_CONSTEXPR char const * const TBAG_GPU_PLATFORM_VERSION    = "version";
TBAG_CONSTEXPR char const * const TBAG_GPU_PLATFORM_NAME       = "name";
TBAG_CONSTEXPR char const * const TBAG_GPU_PLATFORM_VENDOR     = "vendor";
TBAG_CONSTEXPR char const * const TBAG_GPU_PLATFORM_EXTENSIONS = "extensions";

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
    int platform_number;

    GpuPlatform(GpuBackendType t = GpuBackendType::GBT_CPU, int p = 0) : type(t), platform_number(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }
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
    int device_number;

    GpuDevice() : GpuDevice(GpuPlatform()) { /* EMPTY. */ }
    GpuDevice(GpuPlatform const & p, int d = 0) : GpuPlatform(p), device_number(d) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }
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
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;
using GpuContexts  = std::vector<GpuContext>;

//struct TBAG_API SyncedMemory
//{
//    enum class AllocFlag
//    {
//        AF_DEFAULT = 0,
//        AF_PINNED,
//        AF_DEVICE,
//    };
//
//    void * ptr = nullptr;
//    std::size_t size = 0;
//
//    virtual GpuBackendType getType() const TBAG_NOEXCEPT = 0;
//    virtual bool alloc(std::size_t size, AllocFlag flag = AllocFlag::AF_DEFAULT) = 0;
//    virtual bool copy(GpuMemoryBackend * memory) const = 0;
//    virtual void free() = 0;
//};

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

    // --------------------
    // Non-virtual methods.
    // --------------------

    char const * getTypeString() const TBAG_NOEXCEPT;
};

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_BACKEND_GPUBACKEND_HPP__

