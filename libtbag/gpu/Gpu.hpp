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

/**
 * Gpu class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 */
class TBAG_API Gpu
{
private:
    SharedGpuContext  _gpu;

private:
    GpuPlatformInfo _platform_info;
    GpuDeviceInfo     _device_info;

public:
    Gpu();
    explicit Gpu(GpuType type, GpuId platform_id, GpuId device_id);
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
    Err  init(GpuType type, GpuId platform_id, GpuId device_id);
    void clear();

public:
    GpuType getType() const;
    std::string getTypeString() const;

    GpuId getPlatformId() const;
    GpuId   getDeviceId() const;
    GpuId  getContextId() const;

public:
    bool isSupport() const;
    bool    isHost() const;
    bool  isDevice() const;
    bool  isStream() const;

public:
    SharedGpuStream newStream();
    SharedGpuEvent  newEvent(GpuStream const & stream);
    SharedGpuEvent  newEvent(SharedGpuStream const & stream);
    SharedGpuEvent  newEvent(WeakedGpuStream const & stream);
};

template <typename Predicated>
void runIfSupported(std::vector<GpuType> const & types, Predicated predicated)
{
    for (auto & type : types) {
        if (isSupport(type) == false) {
            continue;
        }

        try {
            for (auto & plat : getPlatformList(type)) {
                for (auto & dev : getDeviceList(plat)) {
                    auto gpu = Gpu(type, plat.getPlatformId(), dev.getDeviceId());
                    predicated(gpu);
                }
            }
        } catch (...) {
            // EMPTY.
        }
    }
}

template <typename Predicated>
void runAllIfSupported(Predicated predicated)
{
    runIfSupported({GpuType::GT_CPU, GpuType::GT_ACCEL, GpuType::GT_CUDA, GpuType::GT_OPENCL}, predicated);
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_GPU_HPP__

