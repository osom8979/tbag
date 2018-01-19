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
#include <libtbag/gpu/details/GpuDetails.hpp>

#include <string>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

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

        auto const PLATFORMS = libtbag::gpu::getPlatformList(type);
        if (PLATFORMS.size() > platform_index) {
            continue;
        }

        auto const DEVICES = libtbag::gpu::getDeviceList(PLATFORMS[platform_index]);
        if (DEVICES.empty() > device_index) {
            continue;
        }

        auto context = libtbag::gpu::createContext(DEVICES[device_index]);
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

/**
 * Gpu class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API Gpu
{
public:
    using StreamMap  = std::unordered_map<GpuId, SharedGpuStream>;
    using StreamPair = StreamMap::value_type;

    using EventMap  = std::unordered_map<GpuId, SharedGpuEvent>;
    using EventPair = EventMap::value_type;

private:
    SharedGpuContext  _gpu;
    StreamMap     _streams;
    EventMap       _events;

private:
    GpuPlatformInfo _platform_info;
    GpuDeviceInfo     _device_info;

public:
    Gpu();
    Gpu(GpuType type, std::size_t platform_index = 0, std::size_t device_index = 0);
    explicit Gpu(SharedGpuContext const & gpu);
    explicit Gpu(WeakedGpuContext const & gpu);
    Gpu(Gpu const & obj);
    Gpu(Gpu && obj);
    ~Gpu();

public:
    Gpu & operator =(Gpu const & obj);
    Gpu & operator =(Gpu && obj);

public:
    void swap(Gpu & obj);
    friend void swap(Gpu & lh, Gpu & rh) { lh.swap(rh); }

public:
    inline GpuPlatformInfo const & atPlatformInfo() const TBAG_NOEXCEPT { return _platform_info; }
    inline GpuDeviceInfo   const &   atDeviceInfo() const TBAG_NOEXCEPT { return _device_info; }

public:
    bool validate() const;
    Err  init(GpuType type = GpuType::GT_CPU,
              std::size_t platform_index = 0,
              std::size_t   device_index = 0);
    void clear();

public:
    GpuType getType() const;
    std::string getTypeString() const;

public:
    bool isSupport() const;
    bool    isHost() const;
    bool  isDevice() const;
    bool  isStream() const;

public:
    SharedGpuStream newStream(bool auto_insert = true);
    SharedGpuEvent  newEvent(GpuStream const & stream, bool auto_insert = true);
    SharedGpuEvent  newEvent(SharedGpuStream const & stream, bool auto_insert = true);
    SharedGpuEvent  newEvent(WeakedGpuStream const & stream, bool auto_insert = true);
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

