/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/gpu/accel/AccelContext.hpp>
#include <libtbag/gpu/cpu/CpuContext.hpp>
#include <libtbag/gpu/cuda/CudaContext.hpp>
#include <libtbag/gpu/opencl/OpenCLContext.hpp>

#include <utility>
#include <algorithm>

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

Gpu::Gpu() : Gpu(SharedGpuContext())
{
    // EMPTY.
}

Gpu::Gpu(GpuType type, std::size_t platform_index, std::size_t device_index) : Gpu(SharedGpuContext())
{
    if (isFailure(init(type, platform_index, device_index))) {
        throw std::bad_alloc();
    }
}

Gpu::Gpu(SharedGpuContext const & gpu) : _gpu(gpu), _streams(), _platform_info(), _device_info()
{
    // EMPTY.
}

Gpu::Gpu(WeakedGpuContext const & gpu) : Gpu(gpu.lock())
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
    clear();
}

Gpu & Gpu::operator =(Gpu const & obj)
{
    if (this != &obj) {
        _gpu           = obj._gpu;
        _streams       = obj._streams;
        _platform_info = obj._platform_info;
        _device_info   = obj._device_info;
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
        _streams.swap(obj._streams);
        std::swap(_platform_info, obj._platform_info);
        std::swap(_device_info, obj._device_info);
    }
}

bool Gpu::validate() const
{
    if (_gpu) {
        return _gpu->existsPlatformId() && _gpu->existsDeviceId() && _gpu->existsContextId();
    }
    return false;
}

Err Gpu::init(GpuType type, std::size_t platform_index, std::size_t device_index)
{
    if (libtbag::gpu::isSupport(type) == false) {
        return Err::E_UNSUPOP;
    }

    auto const PLATFORMS = libtbag::gpu::getPlatformList(type);
    if (PLATFORMS.size() > platform_index) {
        return Err::E_OORANGE;
    }

    auto const DEVICES = libtbag::gpu::getDeviceList(PLATFORMS[platform_index]);
    if (DEVICES.empty() > device_index) {
        return Err::E_OORANGE;
    }

    auto context = libtbag::gpu::createContext(DEVICES[device_index]);
    if (static_cast<bool>(context)) {
        _gpu = context;
        _streams.clear();
        _platform_info = libtbag::gpu::getPlatformInfo(*_gpu);
        _device_info   = libtbag::gpu::getDeviceInfo(*_gpu);
        return Err::E_SUCCESS;
    }
    return Err::E_EXPIRED;
}

void Gpu::clear()
{
    _streams.clear();
    _gpu.reset();
}

GpuType Gpu::getType() const
{
    return (_gpu ? _gpu->getType() : GpuType::GT_CPU);
}

std::string Gpu::getTypeString() const
{
    return libtbag::gpu::getGpuTypeString(getType());
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

SharedGpuStream Gpu::newStream(bool auto_insert)
{
    SharedGpuStream stream(_gpu ? new GpuStream(GpuStream::instance(_gpu.get())) : nullptr);
    if (auto_insert && stream) {
        if (_streams.insert(StreamPair(stream->getId(), stream)).second == false) {
            tDLogW("Gpu::newStream() Insert failed.");
        }
    }
    return stream;
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

