/**
 * @file   GpuContext.hpp
 * @brief  GpuContext class prototype.
 * @author zer0
 * @date   2018-01-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUCONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUCONTEXT_HPP__

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

class GpuStream;
class GpuEvent;
class GpuProgram;
class GpuKernel;
class GpuMemory;
class HostMemory;

/**
 * GpuContext structure.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API GpuContext : public GpuDevice
{
public:
    GpuId const CONTEXT;

    GpuPlatformInfo const PLATFORM_INFO;
    GpuDeviceInfo   const   DEVICE_INFO;

public:
    GpuContext(GpuDevice const & d, GpuId c);
    virtual ~GpuContext();

public:
    bool validateMemory(GpuStream const & stream, GpuMemory const & gpu_mem,
                        HostMemory const & host_mem, std::size_t size) const TBAG_NOEXCEPT;

public:
    virtual bool isSupport() const TBAG_NOEXCEPT = 0;
    virtual bool    isHost() const TBAG_NOEXCEPT = 0;
    virtual bool  isDevice() const TBAG_NOEXCEPT = 0;
    virtual bool  isStream() const TBAG_NOEXCEPT = 0;

// Object.
public:
    virtual bool  createStream(GpuStream & stream) const;
    virtual bool releaseStream(GpuStream & stream) const;

    virtual bool  createEvent(GpuStream const & stream, GpuEvent & event) const;
    virtual bool    syncEvent(GpuEvent  const &  event) const;
    virtual bool elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const;
    virtual bool releaseEvent(GpuEvent        &  event) const;

    virtual bool  createProgram(std::string const &  source, GpuProgram & program) const;
    virtual bool   buildProgram(GpuProgram        & program) const;
    virtual bool releaseProgram(GpuProgram        & program) const;

    virtual bool  createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const;
    virtual bool releaseKernel(GpuKernel        & kernel) const;

// Memory.
public:
    virtual bool malloc(GpuMemory & memory, std::size_t size) const;
    virtual bool   free(GpuMemory & memory) const;

    virtual bool mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const;
    virtual bool   freeHost(HostMemory & memory) const;

// Input/Output.
public:
    virtual bool write(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

    virtual bool writeAsync(GpuStream & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const;
    virtual bool  readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const;

// Memory.
public:
    virtual bool  flush(GpuStream & stream) const;
    virtual bool finish(GpuStream & stream) const;
};

using GpuPlatforms = std::vector<GpuPlatform>;
using GpuDevices   = std::vector<GpuDevice>;

using SharedGpuContext = std::shared_ptr<GpuContext>;
using WeakedGpuContext = std::weak_ptr<GpuContext>;

/**
 * GpuIdWrapper structure.
 *
 * @author zer0
 * @date   2018-01-15
 */
class TBAG_API GpuIdWrapper
{
protected:
    GpuContext const * _context;
    GpuId _id;

public:
    GpuIdWrapper(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuIdWrapper(GpuIdWrapper const & obj);
    GpuIdWrapper(GpuIdWrapper && obj);
    virtual ~GpuIdWrapper();

public:
    inline GpuIdWrapper & operator =(GpuIdWrapper const & obj);
    inline GpuIdWrapper & operator =(GpuIdWrapper && obj);

public:
    void assign(GpuIdWrapper const & obj);
    void swap(GpuIdWrapper & obj);

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline bool      existsId() const TBAG_NOEXCEPT { return _id != UNKNOWN_ID; }
    inline GpuId        getId() const TBAG_NOEXCEPT { return _id; }
    inline GpuId       & atId()       TBAG_NOEXCEPT { return _id; }
    inline GpuId const & atId() const TBAG_NOEXCEPT { return _id; }

    inline void clearId() TBAG_NOEXCEPT { _id = UNKNOWN_ID; }

    template <typename T> inline void setId(T i)    TBAG_NOEXCEPT { _id = (GpuId)i; }
    template <typename T> inline T   castId() const TBAG_NOEXCEPT { return (T)_id; }

    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && existsId(); }
};

/**
 * GpuStream class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuStream : public GpuIdWrapper
{
public:
    GpuStream(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuStream(GpuStream const & obj);
    GpuStream(GpuStream && obj);
    virtual ~GpuStream();

public:
    GpuStream & operator =(GpuStream const & obj);
    GpuStream & operator =(GpuStream && obj);

public:
    void swap(GpuStream & obj);
    inline friend void swap(GpuStream & lh, GpuStream & rh) { lh.swap(rh); }

public:
    bool create();
    bool release();

public:
    static GpuStream create(GpuContext const * c);
};

using SharedGpuStream = std::shared_ptr<GpuStream>;
using WeakedGpuStream = std::weak_ptr<GpuStream>;

/**
 * GpuEvent class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuEvent
{
private:
    GpuContext const * _context;
    GpuStream  const * _stream;
    GpuId _start;
    GpuId _stop;

public:
    GpuEvent(GpuContext const * c = nullptr, GpuStream const * s = nullptr,
             GpuId start = UNKNOWN_ID, GpuId stop = UNKNOWN_ID);
    GpuEvent(GpuEvent const & obj);
    GpuEvent(GpuEvent && obj);
    virtual ~GpuEvent();

public:
    GpuEvent & operator =(GpuEvent const & obj);
    GpuEvent & operator =(GpuEvent && obj);

public:
    void swap(GpuEvent & obj);
    inline friend void swap(GpuEvent & lh, GpuEvent & rh) { lh.swap(rh); }

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline bool  isSameStream(GpuStream  const * s) const TBAG_NOEXCEPT { return  _stream == s; }

    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }
    inline GpuStream  const *  getStreamPtr() const TBAG_NOEXCEPT { return _stream; }

    inline bool      existsId() const TBAG_NOEXCEPT { return _start != UNKNOWN_ID; }
    inline GpuId        getId() const TBAG_NOEXCEPT { return _start; }
    inline GpuId       & atId()       TBAG_NOEXCEPT { return _start; }
    inline GpuId const & atId() const TBAG_NOEXCEPT { return _start; }

    template <typename T> inline void    setId(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void setStart(T i) TBAG_NOEXCEPT { _start = (GpuId)i; }
    template <typename T> inline void  setStop(T i) TBAG_NOEXCEPT {  _stop = (GpuId)i; }

    inline GpuId getStart() const TBAG_NOEXCEPT { return _start; }
    inline GpuId  getStop() const TBAG_NOEXCEPT { return  _stop; }

    template <typename T> inline T    castId() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T castStart() const TBAG_NOEXCEPT { return (T)_start; }
    template <typename T> inline T  castStop() const TBAG_NOEXCEPT { return (T) _stop; }

    inline void clearIds() TBAG_NOEXCEPT { _start = UNKNOWN_ID; _stop = UNKNOWN_ID; }

    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && existsId(); }

public:
    bool create();
    bool release();

public:
    bool sync();
    bool elapsed(float * millisec);
    float elapsed();

public:
    static GpuEvent create(GpuStream const * s);
};

/**
 * GpuProgram class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuProgram : public GpuIdWrapper
{
public:
    GpuProgram(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuProgram(GpuProgram const & obj);
    GpuProgram(GpuProgram && obj);
    virtual ~GpuProgram();

public:
    GpuProgram & operator =(GpuProgram const & obj);
    GpuProgram & operator =(GpuProgram && obj);

public:
    void swap(GpuProgram & obj);
    inline friend void swap(GpuProgram & lh, GpuProgram & rh) { lh.swap(rh); }
};

/**
 * GpuKernel class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuKernel : public GpuIdWrapper
{
public:
    GpuKernel(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuKernel(GpuKernel const & obj);
    GpuKernel(GpuKernel && obj);
    virtual ~GpuKernel();

public:
    GpuKernel & operator =(GpuKernel const & obj);
    GpuKernel & operator =(GpuKernel && obj);

public:
    void swap(GpuKernel & obj);
    inline friend void swap(GpuKernel & lh, GpuKernel & rh) { lh.swap(rh); }
};

/**
 * GpuMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuMemory
{
private:
    GpuContext const * _context;

private:
    std::size_t _capacity;
    std::size_t _size;
    void *      _data;

public:
    GpuMemory(GpuContext const * c = nullptr);
    GpuMemory(GpuMemory const & obj);
    GpuMemory(GpuMemory && obj);
    virtual ~GpuMemory();

public:
    GpuMemory & operator =(GpuMemory const & obj);
    GpuMemory & operator =(GpuMemory && obj);

public:
    void swap(GpuMemory & obj);
    inline friend void swap(GpuMemory & lh, GpuMemory & rh) { lh.swap(rh); }

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

    inline void set(void * d, std::size_t c, std::size_t s) TBAG_NOEXCEPT { _data = d; _capacity = c; _size = s; }
    inline void clear() TBAG_NOEXCEPT { _data = nullptr; _capacity = 0; _size = 0; }

    inline bool       exists() const TBAG_NOEXCEPT { return _data != nullptr && _capacity > 0; }
    inline void       * data()       TBAG_NOEXCEPT { return _data; }
    inline void const * data() const TBAG_NOEXCEPT { return _data; }

    template <typename T> inline T castData() const TBAG_NOEXCEPT { return (T)_data; }

    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && exists(); }
};

/**
 * HostMemory class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API HostMemory : public GpuMemory
{
private:
    HostMemoryFlag _flag = HostMemoryFlag::HMF_DEFAULT;

public:
    HostMemory(GpuContext const * c = nullptr);
    HostMemory(GpuMemory const & mem);
    HostMemory(HostMemory const & obj);
    HostMemory(HostMemory && obj);
    virtual ~HostMemory();

public:
    HostMemory & operator =(HostMemory const & obj);
    HostMemory & operator =(HostMemory && obj);

public:
    void swap(HostMemory & obj);
    inline friend void swap(HostMemory & lh, HostMemory & rh) { lh.swap(rh); }

public:
    inline void setFlag(HostMemoryFlag flag) TBAG_NOEXCEPT { _flag = flag; }
    inline HostMemoryFlag getFlag() const TBAG_NOEXCEPT { return _flag; }
};

using GpuMemories  = std::vector<GpuMemory>;
using HostMemories = std::vector<HostMemory>;

/**
 * CpuEventGuard class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API CpuEventGuard : private Noncopyable
{
private:
    GpuEvent * _event;

public:
    CpuEventGuard(GpuEvent * e = nullptr);
    ~CpuEventGuard();

public:
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPUCONTEXT_HPP__

