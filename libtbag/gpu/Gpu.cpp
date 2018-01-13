/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

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

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

