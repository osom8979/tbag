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
#include <libtbag/id/Id.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <exception>

//#define TBAG_GPU_VERBOSE

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

enum class HostMemoryFlag
{
    HMF_DEFAULT,
    HMF_PINNED,
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

TBAG_CONSTEXPR bool isGpuVerbose() TBAG_NOEXCEPT
{
#if defined(TBAG_GPU_VERBOSE)
    return true;
#else
    return false;
#endif
}


struct GpuPlatform
{
    GpuType const TYPE;
    GpuId   const PLATFORM;

    GpuPlatform(GpuType t, GpuId p) : TYPE(t), PLATFORM(p) { /* EMPTY. */ }
    ~GpuPlatform() { /* EMPTY. */ }

    inline char const * getTypeString() TBAG_NOEXCEPT { return getGpuTypeString(TYPE); }
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

struct GpuStream;
struct GpuEvent;
struct GpuProgram;
struct GpuKernel;
struct GpuMemory;
struct HostMemory;

struct GpuContext : public GpuDevice
{
    GpuId const CONTEXT;

    GpuPlatformInfo platform_info;
    GpuDeviceInfo     device_info;

    GpuContext(GpuDevice const & d, GpuId c);
    virtual ~GpuContext();

    inline void checkType(GpuType type) const throw(GpuTypeMismatchException)
    { if (type != TYPE) { throw GpuTypeMismatchException(); } }

    bool checkMemory(GpuStream const & stream, GpuMemory const & gpu_mem,
                     HostMemory const & host_mem, std::size_t size) const TBAG_NOEXCEPT;

    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool    isHost() const TBAG_NOEXCEPT = 0;
    virtual bool  isDevice() const TBAG_NOEXCEPT = 0;
    virtual bool  isStream() const TBAG_NOEXCEPT = 0;

    // -------
    // Object.
    // -------

    virtual GpuStream createStream() const;
    virtual bool     releaseStream(GpuStream & stream) const;

    virtual GpuEvent createEvent(GpuStream const & stream) const;
    virtual bool       syncEvent(GpuEvent  const &  event) const;
    virtual bool    elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const;
    virtual bool    releaseEvent(GpuEvent        &  event) const;

    virtual GpuProgram createProgram(std::string const &  source) const;
    virtual bool        buildProgram(GpuProgram        & program) const;
    virtual bool      releaseProgram(GpuProgram        & program) const;

    virtual GpuKernel createKernel(GpuProgram const & program, std::string const & kernel_symbol) const;
    virtual bool     releaseKernel(GpuKernel        & kernel) const;

    // -------
    // Memory.
    // -------

    virtual GpuMemory malloc(std::size_t size) const;
    virtual bool        free(GpuMemory & memory) const;

    virtual HostMemory mallocHost(std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const;
    virtual bool         freeHost(HostMemory & memory) const;

    // -------------
    // Input/Output.
    // -------------

    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    virtual bool writeAsync(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    // -------
    // Memory.
    // -------

    virtual bool  flush(GpuStream & stream) const;
    virtual bool finish(GpuStream & stream) const;
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;

using SharedGpuContext = std::shared_ptr<GpuContext>;
using WeakedGpuContext = std::weak_ptr<GpuContext>;

struct GpuStream
{
    GpuContext const & CONTEXT;
    GpuId stream = UNKNOWN_ID;

    GpuStream(GpuContext const & c, GpuId s = UNKNOWN_ID) : CONTEXT(c), stream(s) { /* EMPTY. */ }
    ~GpuStream() { /* EMPTY. */ }

    inline bool exists() const TBAG_NOEXCEPT { return stream != UNKNOWN_ID; }
};

struct GpuEvent
{
    GpuContext const & CONTEXT;
    GpuId start = UNKNOWN_ID;
    GpuId  stop = UNKNOWN_ID;

    GpuEvent(GpuContext const & c, GpuId st = UNKNOWN_ID, GpuId sp = UNKNOWN_ID)
            : CONTEXT(c), start(st), stop(sp)
    { /* EMPTY. */ }
    ~GpuEvent() { /* EMPTY. */ }

    inline bool exists() const TBAG_NOEXCEPT { return start != UNKNOWN_ID; }
};

struct GpuProgram
{
    GpuContext const & CONTEXT;
    GpuId program = UNKNOWN_ID;

    GpuProgram(GpuContext const & c, GpuId p = UNKNOWN_ID) : CONTEXT(c), program(p) { /* EMPTY. */ }
    ~GpuProgram() { /* EMPTY. */ }

    inline bool exists() const TBAG_NOEXCEPT { return program != UNKNOWN_ID; }
};

struct GpuKernel
{
    GpuContext const & CONTEXT;
    GpuId kernel = UNKNOWN_ID;

    GpuKernel(GpuContext const & c, GpuId k = UNKNOWN_ID) : CONTEXT(c), kernel(k) { /* EMPTY. */ }
    ~GpuKernel() { /* EMPTY. */ }

    inline bool exists() const TBAG_NOEXCEPT { return kernel != UNKNOWN_ID; }
};

struct GpuMemory
{
    using TypeTable = type::TypeTable;

    GpuContext const & CONTEXT;

    TypeTable       type = TypeTable::TT_UNKNOWN;
    std::size_t capacity = 0;
    std::size_t     size = 0;
    void *          data = nullptr;

    GpuMemory(GpuContext const & c) : CONTEXT(c) { /* EMPTY. */ }
    ~GpuMemory() { /* EMPTY. */ }

    inline bool exists() const TBAG_NOEXCEPT { return data != nullptr && capacity > 0U; }
};

struct HostMemory : public GpuMemory
{
    HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT;

    HostMemory(GpuContext const & c) : GpuMemory(c) { /* EMPTY. */ }
    HostMemory(GpuMemory const & m) : GpuMemory(m) { /* EMPTY. */ }
    ~HostMemory() { /* EMPTY. */ }
};

using GpuMemories  = std::vector<GpuMemory>;
using HostMemories = std::vector<HostMemory>;

struct CpuEventGuard : private Noncopyable
{
    GpuEvent * event;

    CpuEventGuard(GpuEvent * e = nullptr);
    ~CpuEventGuard();

    static GpuId nowNano();
};

// -----------------
// Global interface.
// -----------------

TBAG_API bool isSupport(GpuType type) TBAG_NOEXCEPT;

TBAG_API int             getPlatformCount(GpuType type);
TBAG_API GpuPlatforms    getPlatformList (GpuType type);
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

template <typename Predicated>
void runIfSupported(std::vector<GpuType> const & types,
                    std::size_t platform_index,
                    std::size_t device_index,
                    Predicated predicated)
{
    for (auto & type : types) {
        if (isSupport(type) == false) {
            continue;
        }

        auto const PLATFORMS = getPlatformList(type);
        if (PLATFORMS.size() > platform_index) {
            continue;
        }

        auto const DEVICES = getDeviceList(PLATFORMS[platform_index]);
        if (DEVICES.empty() > device_index) {
            continue;
        }

        auto context = createContext(DEVICES[device_index]);
        if (static_cast<bool>(context)) {
            predicated(context);
        }
    }
}

template <typename Predicated>
void runAllIfSupported(Predicated predicated, std::size_t platform_index = 0, std::size_t device_index = 0)
{
    runIfSupported({GpuType::GT_CPU,
                    GpuType::GT_ACCEL,
                    GpuType::GT_CUDA,
                    GpuType::GT_OPENCL}, platform_index, device_index, predicated);
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

