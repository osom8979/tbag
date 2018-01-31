/**
 * @file   CudaContext.hpp
 * @brief  CudaContext class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDACONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDACONTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuDetails.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu  {
namespace cuda {

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

TBAG_API int             getPlatformCount();
TBAG_API GpuPlatforms    getPlatformList ();
TBAG_API GpuPlatformInfo getPlatformInfo (GpuPlatform const & platform);

TBAG_API int             getDeviceCount(GpuPlatform const & platform);
TBAG_API GpuDevices      getDeviceList (GpuPlatform const & platform);
TBAG_API GpuDeviceInfo   getDeviceInfo (GpuDevice   const & device);

TBAG_API SharedGpuContext createContext(GpuDevice const & device);

/**
 * CudaContext class prototype.
 *
 * @author zer0
 * @date   2018-01-13
 */
class TBAG_API CudaContext : public GpuContext
{
public:
    CudaContext(GpuDevice const & d, GpuId c);
    virtual ~CudaContext();

public:
    /**
     * Maximum Thread Blocks Per Multiprocessor.
     */
    struct OccupancyInfo
    {
        std::size_t max_group;     ///< Limited by Max Warps or Max Blocks per Multiprocessor.
        std::size_t registers;     ///< Limited by Registers per Multiprocessor.
        std::size_t shared_memory; ///< Limited by Shared Memory per Multiprocessor.
    };

public:
    Err calcOccupancy(int thread_per_block,
                      int registers_per_thread,
                      int shared_memory_per_block,
                      OccupancyInfo & result);

public:
    /**
     * Theoretical bandwidth can be calculated using hardware specifications available in the product literature.
     *
     * @return
     *  Theoretical bandwidth (GByte/s).
     *
     * @remarks
     *  - Note: Some calculations use 10243 instead of 109 for the final calculation. @n
     *          In such a case, the bandwidth would be 165.4GB/s. It is important to  @n
     *          use the same divisor when calculating theoretical and effective       @n
     *          bandwidth so that the comparison is valid. @n
     *  - Note: When ECC is enabled, the effective maximum bandwidth is reduced by    @n
     *          approximately 20% due to the additional traffic for the memory        @n
     *          checksums, though the exact impact of ECC on bandwidth depends on the @n
     *          memory access pattern.
     *
     * @see <http://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#theoretical-bandwidth-calculation>
     */
    static double calcTheoreticalBandwidth(double memory_clock_rate_ghz,
                                           int    memory_interface_bit,
                                           int    memory_interface_lane,
                                           bool   enable_ecc = false);

    /**
     * Effective bandwidth is calculated by timing specific program activities
     * and by knowing how data is accessed by the program.
     *
     * @return
     *  Effective bandwidth (GByte/s).
     *
     * @remarks
     *  - Note: Some calculations use 10243 instead of 109 for the final calculation. @n
     *          In such a case, the bandwidth would be 165.4GB/s. It is important to  @n
     *          use the same divisor when calculating theoretical and effective       @n
     *          bandwidth so that the comparison is valid. @n
     *
     * @see <http://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html#theoretical-bandwidth-calculation>
     */
    static double calcEffectiveBandwidth(int read_byte_by_kernel, int write_byte_by_kernel, int seconds);

public:
    virtual bool isSupport() const TBAG_NOEXCEPT override;
    virtual bool    isHost() const TBAG_NOEXCEPT override;
    virtual bool  isDevice() const TBAG_NOEXCEPT override;
    virtual bool  isStream() const TBAG_NOEXCEPT override;

    virtual Err  createStream(GpuStream & stream) const override;
    virtual Err releaseStream(GpuStream & stream) const override;

    virtual Err  createEvent(GpuStream const & stream, GpuEvent & event) const override;
    virtual Err    syncEvent(GpuEvent  const &  event) const override;
    virtual Err elapsedEvent(GpuEvent        &  event, float * millisec = nullptr) const override;
    virtual Err releaseEvent(GpuEvent        &  event) const override;

    virtual Err malloc(GpuMemory & memory, std::size_t size) const override;
    virtual Err   free(GpuMemory & memory) const override;

    virtual Err mallocHost(HostMemory & memory, std::size_t size, HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) const override;
    virtual Err   freeHost(HostMemory & memory) const override;

    virtual Err write(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err  read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err writeAsync(GpuStream const & stream, GpuMemory       & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err  readAsync(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory       & host_mem, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err copy(GpuStream const & stream,  GpuMemory const & src,  GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err copy(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err copyAsync(GpuStream const & stream,  GpuMemory const & src,  GpuMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;
    virtual Err copyAsync(GpuStream const & stream, HostMemory const & src, HostMemory & dest, std::size_t size, GpuEvent * event = nullptr) const override;

    virtual Err  flush(GpuStream const & stream) const override;
    virtual Err finish(GpuStream const & stream) const override;

public:
    virtual Err fill(GpuStream const & stream, int      * out, int      data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, unsigned * out, unsigned data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, float    * out, float    data, int count, GpuEvent * event) const override;
    virtual Err fill(GpuStream const & stream, double   * out, double   data, int count, GpuEvent * event) const override;

    virtual Err add(GpuStream const & stream, int      const * in1, int      const * in2, int      * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, float    const * in1, float    const * in2, float    * out, int count, GpuEvent * event) const override;
    virtual Err add(GpuStream const & stream, double   const * in1, double   const * in2, double   * out, int count, GpuEvent * event) const override;
};

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDA_CUDACONTEXT_HPP__

