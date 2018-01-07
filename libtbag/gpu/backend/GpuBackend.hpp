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
#include <libtbag/type/TypeTable.hpp>

#include <string>
#include <vector>
#include <map>
#include <limits>
#include <type_traits>

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

static_assert(sizeof(GpuId) >= sizeof(void*), "The GpuId size is too small.");

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

/** [*] ASCII string identifying the device; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_NAME    = "name";
/** [*] Version string; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_VERSION = "version";

/** [CUDA] is the maximum amount of shared memory available to a thread block in bytes; @n
 * this amount is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_SHARED_MEM_PER_BLOCK = "sharedMemPerBlock";
/** [CUDA] is the maximum number of 32-bit registers available to a thread block; @n
 * this number is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_REGS_PER_BLOCK = "regsPerBlock";
/** [CUDA] is the warp size in threads; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_WARP_SIZE = "warpSize";
/** [CUDA] is the maximum pitch in bytes allowed by the memory copy functions @n
 * that involve memory regions allocated through cudaMallocPitch(); */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MEM_PITCH = "memPitch";
/** [CUDA] is the maximum number of threads per block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_PER_BLOCK = "maxThreadsPerBlock";
/** [CUDA] contains the maximum size of each dimension of a block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_0 = "maxThreadsDim0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_1 = "maxThreadsDim1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_2 = "maxThreadsDim2";
/** [CUDA] contains the maximum size of each dimension of a grid; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_0 = "maxGridSize0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_1 = "maxGridSize1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_2 = "maxGridSize2";
/** [CUDA] is the clock frequency in kilohertz; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CLOCK_RATE = "clockRate";
/** [CUDA] is the total amount of constant memory available on the device in bytes; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TOTAL_CONST_MEM = "totalConstMem";
/** [CUDA] is the alignment requirement; texture base addresses that are aligned @n
 * to textureAlignment bytes do not need an offset applied to texture fetches; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TEXTURE_ALIGNMENT = "textureAlignment";
/** [CUDA] is 1 if the device can concurrently copy memory between host and device while executing a kernel, or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_DEVICE_OVERLAP = "deviceOverlap";
/** [CUDA] is the number of multiprocessors on the device; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MULTI_PROCESSOR_COUNT = "multiProcessorCount";
/** [CUDA] is 1 if there is a run time limit for kernels executed on the device, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_KERNEL_EXEC_TIMEOUT_ENABLED = "kernelExecTimeoutEnabled";
/** [CUDA] is 1 if the device is an integrated (motherboard) GPU and 0 if it is a discrete (card) component. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_INTEGRATED = "integrated";
/** [CUDA] is 1 if the device can map host memory into the CUDA address space @n
 * for use with cudaHostAlloc()/cudaHostGetDevicePointer(), or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CAN_MAP_HOST_MEMORY = "canMapHostMemory";

/** [CUDA] is the compute mode that the device is currently in. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE = "computeMode";
/** [CUDA] Default mode - Device is not restricted and multiple threads can use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_DEFAULT = "cudaComputeModeDefault";
/** [CUDA] Compute-exclusive mode - Only one thread will be able to use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_EXCLUSIVE = "cudaComputeModeExclusive";
/** [CUDA] Compute-prohibited mode - No threads can use cudaSetDevice() with this device. @n
 * Any errors from calling cudaSetDevice() with an exclusive (and occupied) or prohibited device @n
 * will only show up after a non-device management runtime function is called. @n
 * At that time, cudaErrorNoDevice will be returned. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_PROHIBITED = "cudaComputeModeProhibited";

/** [CUDA] is 1 if the device supports executing multiple kernels within the same context simultaneously, or 0 if not. @n
 * It is not guaranteed that multiple kernels will be resident on the device concurrently @n
 * so this feature should not be relied upon for correctness; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CONCURRENT_KERNELS = "concurrentKernels";
/** [CUDA] is 1 if the device has ECC support turned on, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_ECC_ENABLED = "ECCEnabled";
/** [CUDA] is the PCI bus identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_BUS_ID = "pciBusID";
/** [CUDA] is the PCI device (sometimes called slot) identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_DEVICE_ID = "pciDeviceID";
/** [CUDA] is 1 if the device is using a TCC driver or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TCC_DRIVER = "tccDriver";

struct GpuDeviceInfo : public GpuDevice
{
    std::string name;           ///< Device name.
    std::string device_version; ///< Version string.
    std::string driver_version; ///< Driver version string.
    std::size_t global_memory;  ///< Size of global device memory in bytes.

    using Properties = std::map<std::string, std::string>;

    Properties properties;

    GpuDeviceInfo() : GpuDeviceInfo(GpuDevice()) { /* EMPTY. */ }
    GpuDeviceInfo(GpuDevice const & d) : GpuDevice(d) { /* EMPTY. */ }
    ~GpuDeviceInfo() { /* EMPTY. */ }

    template <typename ValueType>
    bool insert(std::string const & name, ValueType const & prop)
    {
        return insert(name, std::to_string(prop));
    }

    bool insert(std::string const & name, char const * prop)
    {
        return insert(name, std::string(prop));
    }

    bool insert(std::string const & name, std::string const & prop)
    {
        return properties.insert(Properties::value_type(name, prop)).second;
    }

    bool exists(std::string const & name) const
    {
        return properties.find(name) != properties.end();
    }

    std::string get(std::string const & name) const
    {
        auto itr = properties.find(name);
        if (itr != properties.end()) {
            return itr->second;
        }
        return std::string();
    }

    bool getInteger(std::string const & name, int & value) const
    {
        auto itr = properties.find(name);
        if (itr != properties.end()) {
            try {
                value = std::stoi(itr->second);
                return true;
            } catch (...) {
                // EMPTY.
            }
        }
        return false;
    }
};

struct GpuContext : public GpuDevice
{
    GpuId context_id;

    GpuContext() : GpuContext(GpuDevice()) { /* EMPTY. */ }
    GpuContext(GpuDevice const & d, GpuId c = UNKNOWN_GPU_ID) : GpuDevice(d), context_id(c) { /* EMPTY. */ }
    ~GpuContext() { /* EMPTY. */ }

    inline bool isUnknownContext() const TBAG_NOEXCEPT { return context_id == UNKNOWN_GPU_ID; }
};

struct GpuStream : public GpuContext
{
    GpuId stream_id;

    GpuStream() : GpuStream(GpuContext()) { /* EMPTY. */ }
    GpuStream(GpuContext const & c, GpuId s = UNKNOWN_GPU_ID) : GpuContext(c), stream_id(s) { /* EMPTY. */ }
    ~GpuStream() { /* EMPTY. */ }

    inline bool isUnknownStream() const TBAG_NOEXCEPT { return stream_id == UNKNOWN_GPU_ID; }
};

struct GpuEvent : public GpuStream
{
    GpuId start;
    GpuId stop;

    GpuEvent() : GpuEvent(GpuStream()) { /* EMPTY. */ }
    GpuEvent(GpuStream const & q) : GpuStream(q), start(UNKNOWN_GPU_ID), stop(UNKNOWN_GPU_ID) { /* EMPTY. */ }
    ~GpuEvent() { /* EMPTY. */ }

    inline bool isUnknownEvent() const TBAG_NOEXCEPT { return start == UNKNOWN_GPU_ID; }
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

enum class HostMemoryFlag
{
    HMF_UNINITIALIZED,
    HMF_DEFAULT,
    HMF_PINNED,
};

struct HostMemory : public GpuContext
{
    void * data;
    std::size_t size;
    HostMemoryFlag flag;

    HostMemory() : HostMemory(GpuContext()) { /* EMPTY. */ }
    HostMemory(GpuContext const & c) : GpuContext(c), data(nullptr), size(0), flag(HostMemoryFlag::HMF_UNINITIALIZED) { /* EMPTY. */ }
    ~HostMemory() { /* EMPTY. */ }

    inline bool existsMemory() const TBAG_NOEXCEPT { return data != nullptr && size > 0U; }
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;
using GpuContexts  = std::vector<GpuContext>;

using GpuMemories  = std::vector<GpuMemory>;
using HostMemories = std::vector<HostMemory>;

struct GpuTypeMismatchException : public std::exception
{
    virtual const char * what() const TBAG_NOEXCEPT override
    { return "Mismatch exception of GPU type."; }
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

    virtual bool isStreamSupported(GpuDevice  const &  device) const = 0;
    virtual GpuStream createStream(GpuContext const & context) const = 0;
    virtual bool     releaseStream(GpuStream        &  stream) const = 0;

    virtual GpuEvent createEvent(GpuStream const & stream) const = 0;
    virtual bool       syncEvent(GpuEvent  const & event) const = 0;
    virtual bool    elapsedEvent(GpuEvent        & event, float * millisec = nullptr) const = 0;
    virtual bool    releaseEvent(GpuEvent        & event) const = 0;

    // -------
    // Memory.
    // -------

    virtual GpuMemory malloc(GpuContext const & context, std::size_t size) const = 0;
    virtual bool        free(GpuMemory & memory) const = 0;

    virtual HostMemory mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const = 0;
    virtual bool         freeHost(HostMemory & memory) const = 0;

    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const = 0;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const = 0;

    virtual bool enqueueWrite(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const = 0;
    virtual bool  enqueueRead(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const = 0;

    // -------
    // Memory.
    // -------

    virtual bool  flush(GpuStream & stream) const = 0;
    virtual bool finish(GpuStream & stream) const = 0;

    // -------
    // Kernel.
    // -------

    virtual bool runAdd(GpuStream & stream, GpuMemory const & v1, GpuMemory const & v2, GpuMemory & result,
                        type::TypeTable type, int count) const = 0;

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

