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
    int number;

    std::string profile;
    std::string name;
    std::string vendor;
    std::string version;
    std::string extensions;

    GpuPlatform(GpuBackendType t, int n) : type(t), number(n) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }

    inline operator int() const TBAG_NOEXCEPT { return number; }
};

struct GpuDevice
{
    GpuBackendType type;
    int number;

    GpuDevice(GpuBackendType t, int n) : type(t), number(n) { /* EMPTY. */ }
    ~GpuDevice() { /* EMPTY. */ }

    inline operator int() const TBAG_NOEXCEPT { return number; }
};

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
    using StringMap  = std::map<std::string, std::string>;
    using StringPair = StringMap::value_type;
    using Devices    = std::vector<GpuDevice>;
    using Platforms  = std::vector<GpuPlatform>;

    virtual GpuBackendType  getType() const TBAG_NOEXCEPT = 0;
    virtual bool          isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool             isHost() const TBAG_NOEXCEPT = 0;
    virtual bool           isDevice() const TBAG_NOEXCEPT = 0;

    virtual int       getPlatformCount() const = 0;
    virtual Platforms getPlatformList () const = 0;

    virtual int     getDeviceCount() const = 0;
    virtual Devices getDeviceList () const = 0;

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

