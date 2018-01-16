/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/gpu/accel/AccelBackend.hpp>
#include <libtbag/gpu/cpu/CpuBackend.hpp>
#include <libtbag/gpu/cuda/CudaBackend.hpp>
#include <libtbag/gpu/opencl/OpenCLBackend.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

#ifndef _TBAG_SELECT_GPU_METHOD
#define _TBAG_SELECT_GPU_METHOD(gpu_type, default_return_value, method_name, param) \
    switch (gpu_type) {                                          \
    case GpuType::GT_CPU:     return    cpu::method_name(param); \
    case GpuType::GT_ACCEL:   return  accel::method_name(param); \
    case GpuType::GT_CUDA:    return   cuda::method_name(param); \
    case GpuType::GT_OPENCL:  return opencl::method_name(param); \
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;            \
    } return default_return_value;                               \
    /* -- END -- */
#endif

bool isSupport(GpuType type) TBAG_NOEXCEPT
{
    _TBAG_SELECT_GPU_METHOD(type, false, isSupport,);
}

int getPlatformCount(GpuType type)
{
    _TBAG_SELECT_GPU_METHOD(type, 0, getPlatformCount,);
}

GpuPlatforms getPlatformList(GpuType type)
{
    _TBAG_SELECT_GPU_METHOD(type, GpuPlatforms(), getPlatformList,);
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, GpuPlatformInfo(), getPlatformInfo, platform);
}

int getDeviceCount(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, 0, getDeviceCount, platform);
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    _TBAG_SELECT_GPU_METHOD(platform.TYPE, GpuDevices(), getDeviceList, platform);
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    _TBAG_SELECT_GPU_METHOD(device.TYPE, GpuDeviceInfo(), getDeviceInfo, device);
}

SharedGpuContext createContext(GpuDevice const & device)
{
    _TBAG_SELECT_GPU_METHOD(device.TYPE, SharedGpuContext(), createContext, device);
}

#if defined(_TBAG_SELECT_GPU_METHOD)
#undef _TBAG_SELECT_GPU_METHOD
#endif

// -------------------
// Gpu implementation.
// -------------------

Gpu::Gpu() : _gpu()
{
    // EMPTY.
}

Gpu::Gpu(GpuType type, std::size_t platform_index, std::size_t device_index) : _gpu()
{
    if (init(type, platform_index, device_index) == false) {
        throw std::bad_alloc();
    }
}

Gpu::Gpu(SharedGpuContext const & gpu) : _gpu(gpu)
{
    // EMPTY.
}

Gpu::Gpu(WeakedGpuContext const & gpu) : _gpu(gpu)
{
    // EMPTY.
}

Gpu::Gpu(Gpu const & obj) : Gpu()
{
    (*this) = obj;
}

Gpu::Gpu(Gpu && obj) : Gpu()
{
    (*this) = std::move(obj);
}

Gpu::~Gpu()
{
    // EMPTY.
}

Gpu & Gpu::operator =(Gpu const & obj)
{
    if (this != &obj) {
        _gpu = obj._gpu;
    }
    return *this;
}

Gpu & Gpu::operator =(Gpu && obj)
{
    swap(obj);
    return *this;
}

void Gpu::swap(Gpu & obj)
{
    if (this != &obj) {
        _gpu.swap(obj._gpu);
    }
}

bool Gpu::init(GpuType type, std::size_t platform_index, std::size_t device_index)
{
    if (libtbag::gpu::isSupport(type) == false) {
        return false;
    }

    auto const PLATFORMS = libtbag::gpu::getPlatformList(type);
    if (PLATFORMS.size() > platform_index) {
        return false;
    }

    auto const DEVICES = libtbag::gpu::getDeviceList(PLATFORMS[platform_index]);
    if (DEVICES.empty() > device_index) {
        return false;
    }

    auto context = libtbag::gpu::createContext(DEVICES[device_index]);
    if (static_cast<bool>(context)) {
        _gpu = context;
        return true;
    }
    return false;
}

bool Gpu::isSupport() const
{
    return (_gpu ? _gpu->isSupport() : false);
}

bool Gpu::isHost() const
{
    return (_gpu ? _gpu->isHost() : false);
}

bool Gpu::isDevice() const
{
    return (_gpu ? _gpu->isDevice() : false);
}

bool Gpu::isStream() const
{
    return (_gpu ? _gpu->isStream() : false);
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

