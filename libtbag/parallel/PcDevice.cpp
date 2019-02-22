/**
 * @file   PcDevice.cpp
 * @brief  Parallel Computing Device class implementation.
 * @author zer0
 * @date   2019-02-21
 */

#include <libtbag/parallel/PcDevice.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/parallel/cl/ClDevice.hpp>
#include <libtbag/parallel/cpu/CpuDevice.hpp>
#include <libtbag/parallel/cuda/CudaDevice.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {

bool isSupport(PcTypeId type) TBAG_NOEXCEPT
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::isSupport();
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::isSupport();
    case PC_TYPE_CL:   return libtbag::parallel::cl::isSupport();
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return false;
}

int getPlatformCount(PcTypeId type)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getPlatformCount();
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getPlatformCount();
    case PC_TYPE_CL:   return libtbag::parallel::cl::getPlatformCount();
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

PcPlatformIds getPlatformList(PcTypeId type)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getPlatformList();
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getPlatformList();
    case PC_TYPE_CL:   return libtbag::parallel::cl::getPlatformList();
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return PcPlatformIds();
}

PcPlatformInfo getPlatformInfo(PcTypeId type, PcPlatformId platform)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getPlatformInfo(platform);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getPlatformInfo(platform);
    case PC_TYPE_CL:   return libtbag::parallel::cl::getPlatformInfo(platform);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return PcPlatformInfo();
}

int getDeviceCount(PcTypeId type, PcPlatformId platform)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getDeviceCount(platform);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getDeviceCount(platform);
    case PC_TYPE_CL:   return libtbag::parallel::cl::getDeviceCount(platform);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

PcDeviceIds getDeviceList(PcTypeId type, PcPlatformId platform)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getDeviceList(platform);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getDeviceList(platform);
    case PC_TYPE_CL:   return libtbag::parallel::cl::getDeviceList(platform);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return PcDeviceIds();
}

PcDeviceInfo getDeviceInfo(PcTypeId type, PcDeviceId device)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::getDeviceInfo(device);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::getDeviceInfo(device);
    case PC_TYPE_CL:   return libtbag::parallel::cl::getDeviceInfo(device);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return PcDeviceInfo();
}

PcContextId createContext(PcTypeId type, PcDeviceId device)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::createContext(device);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::createContext(device);
    case PC_TYPE_CL:   return libtbag::parallel::cl::createContext(device);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return UNKNOWN_ID;
}

bool deleteContext(PcTypeId type, PcContextId context)
{
    switch (type) {
    case PC_TYPE_CPU:  return libtbag::parallel::cpu::deleteContext(context);
    case PC_TYPE_CUDA: return libtbag::parallel::cuda::deleteContext(context);
    case PC_TYPE_CL:   return libtbag::parallel::cl::deleteContext(context);
    default: break;
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return false;
}

PcType getPcType(PcTypeId type)
{
    PcType result;
    result.type = type;
    return result;
}

PcPlatform getPcPlatform(PcType type, PcPlatformId platform)
{
    PcPlatform result;
    result.type = type.type;
    result.platform = platform;
    return result;
}

PcDevice getPcDevice(PcPlatform platform, PcDeviceId device)
{
    PcDevice result;
    result.type = platform.type;
    result.platform = platform.platform;
    result.device = device;
    return result;
}

PcContext getPcContext(PcDevice device, PcContextId context)
{
    PcContext result;
    result.type = device.type;
    result.platform = device.platform;
    result.device = device.device;
    result.context = context;
    return result;
}

} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

