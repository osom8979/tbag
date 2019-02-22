/**
 * @file   CudaDevice.hpp
 * @brief  CudaDevice class prototype.
 * @author zer0
 * @date   2019-02-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CUDA_CUDADEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CUDA_CUDADEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/parallel/PcCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cuda     {

/** is the maximum amount of shared memory available to a thread block in bytes; @n
 * this amount is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_SHARED_MEM_PER_BLOCK = "sharedMemPerBlock";
/** is the maximum number of 32-bit registers available to a thread block; @n
 * this number is shared by all thread blocks simultaneously resident on a multiprocessor; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_REGS_PER_BLOCK = "regsPerBlock";
/** is the warp size in threads; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_WARP_SIZE = "warpSize";
/** is the maximum pitch in bytes allowed by the memory copy functions @n
 * that involve memory regions allocated through cudaMallocPitch(); */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MEM_PITCH = "memPitch";
/** is the maximum number of threads per block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_PER_BLOCK = "maxThreadsPerBlock";
/** contains the maximum size of each dimension of a block; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_0 = "maxThreadsDim0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_1 = "maxThreadsDim1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_THREADS_DIM_2 = "maxThreadsDim2";
/** contains the maximum size of each dimension of a grid; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_0 = "maxGridSize0";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_1 = "maxGridSize1";
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MAX_GRID_SIZE_2 = "maxGridSize2";
/** is the clock frequency in kilohertz; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CLOCK_RATE = "clockRate";
/** is the total amount of constant memory available on the device in bytes; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TOTAL_CONST_MEM = "totalConstMem";
/** is the alignment requirement; texture base addresses that are aligned @n
 * to textureAlignment bytes do not need an offset applied to texture fetches; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TEXTURE_ALIGNMENT = "textureAlignment";
/** is 1 if the device can concurrently copy memory between host and device while executing a kernel, or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_DEVICE_OVERLAP = "deviceOverlap";
/** is the number of multiprocessors on the device; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_MULTI_PROCESSOR_COUNT = "multiProcessorCount";
/** is 1 if there is a run time limit for kernels executed on the device, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_KERNEL_EXEC_TIMEOUT_ENABLED = "kernelExecTimeoutEnabled";
/** is 1 if the device is an integrated (motherboard) GPU and 0 if it is a discrete (card) component. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_INTEGRATED = "integrated";
/** is 1 if the device can map host memory into the CUDA address space @n
 * for use with cudaHostAlloc()/cudaHostGetDevicePointer(), or 0 if not; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CAN_MAP_HOST_MEMORY = "canMapHostMemory";

/** is the compute mode that the device is currently in. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE = "computeMode";
/** Default mode - Device is not restricted and multiple threads can use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_DEFAULT = "cudaComputeModeDefault";
/** Compute-exclusive mode - Only one thread will be able to use cudaSetDevice() with this device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_EXCLUSIVE = "cudaComputeModeExclusive";
/** Compute-prohibited mode - No threads can use cudaSetDevice() with this device. @n
 * Any errors from calling cudaSetDevice() with an exclusive (and occupied) or prohibited device @n
 * will only show up after a non-device management runtime function is called. @n
 * At that time, cudaErrorNoDevice will be returned. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_COMPUTE_MODE_PROHIBITED = "cudaComputeModeProhibited";

/** is 1 if the device supports executing multiple kernels within the same context simultaneously, or 0 if not. @n
 * It is not guaranteed that multiple kernels will be resident on the device concurrently @n
 * so this feature should not be relied upon for correctness; */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_CONCURRENT_KERNELS = "concurrentKernels";
/** is 1 if the device has ECC support turned on, or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_ECC_ENABLED = "ECCEnabled";
/** is the PCI bus identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_BUS_ID = "pciBusID";
/** is the PCI device (sometimes called slot) identifier of the device. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_PCI_DEVICE_ID = "pciDeviceID";
/** is 1 if the device is using a TCC driver or 0 if not. */
TBAG_CONSTEXPR char const * const TBAG_GPU_DEVICE_INFO_TCC_DRIVER = "tccDriver";

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int getPlatformCount();
TBAG_API PcPlatformIds getPlatformList();
TBAG_API PcPlatformInfo getPlatformInfo(PcPlatformId platform);

TBAG_API int getDeviceCount(PcPlatformId platform);
TBAG_API PcDeviceIds getDeviceList(PcPlatformId platform);
TBAG_API PcDeviceInfo getDeviceInfo(PcDeviceId device);

TBAG_API PcContextId createContext(PcDeviceId device);
TBAG_API bool deleteContext(PcContextId context);

} // namespace cuda
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CUDA_CUDADEVICE_HPP__

