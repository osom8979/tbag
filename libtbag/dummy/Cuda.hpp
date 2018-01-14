/**
 * @file   Cuda.hpp
 * @brief  Cuda class prototype.
 * @author zer0
 * @date   2018-01-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_CUDA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_CUDA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>
#include <cstdint>

#ifndef CUDA_VERSION
#define CUDA_VERSION 0
#endif

#ifndef CUDAAPI
#define CUDAAPI inline
#endif

#ifndef CUDARTAPI
#define CUDARTAPI inline
#endif

#define cudaHostAllocDefault                0x00  // Default page-locked allocation flag
#define cudaHostAllocPortable               0x01  // Pinned memory accessible by all CUDA contexts
#define cudaHostAllocMapped                 0x02  // Map allocation into device space
#define cudaHostAllocWriteCombined          0x04  // Write-combined memory

#define cudaHostRegisterDefault             0x00  // Default host memory registration flag
#define cudaHostRegisterPortable            0x01  // Pinned memory accessible by all CUDA contexts
#define cudaHostRegisterMapped              0x02  // Map registered memory into device space
#define cudaHostRegisterIoMemory            0x04  // Memory-mapped I/O space

#define cudaPeerAccessDefault               0x00  // Default peer addressing enable flag

#define cudaStreamDefault                   0x00  // Default stream flag
#define cudaStreamNonBlocking               0x01  // Stream does not synchronize with stream 0 (the NULL stream)

#define cudaStreamLegacy                    ((cudaStream_t)0x1) // Legacy stream handle
#define cudaStreamPerThread                 ((cudaStream_t)0x2) // Per-thread stream handle

#define cudaEventDefault                    0x00  // Default event flag
#define cudaEventBlockingSync               0x01  // Event uses blocking synchronization
#define cudaEventDisableTiming              0x02  // Event will not record timing data
#define cudaEventInterprocess               0x04  // Event is suitable for interprocess use. cudaEventDisableTiming must be set

#define cudaDeviceScheduleAuto              0x00  // Device flag - Automatic scheduling
#define cudaDeviceScheduleSpin              0x01  // Device flag - Spin default scheduling
#define cudaDeviceScheduleYield             0x02  // Device flag - Yield default scheduling
#define cudaDeviceScheduleBlockingSync      0x04  // Device flag - Use blocking synchronization
#define cudaDeviceBlockingSync              0x04  // Device flag - Use blocking synchronization
                                                  // \deprecated This flag was deprecated as of CUDA 4.0 and
                                                  // replaced with ::cudaDeviceScheduleBlockingSync.
#define cudaDeviceScheduleMask              0x07  // Device schedule flags mask
#define cudaDeviceMapHost                   0x08  // Device flag - Support mapped pinned allocations
#define cudaDeviceLmemResizeToMax           0x10  // Device flag - Keep local memory allocation after launch
#define cudaDeviceMask                      0x1f  // Device flags mask

#define cudaArrayDefault                    0x00  // Default CUDA array allocation flag
#define cudaArrayLayered                    0x01  // Must be set in cudaMalloc3DArray to create a layered CUDA array
#define cudaArraySurfaceLoadStore           0x02  // Must be set in cudaMallocArray or cudaMalloc3DArray in order to bind surfaces to the CUDA array
#define cudaArrayCubemap                    0x04  // Must be set in cudaMalloc3DArray to create a cubemap CUDA array
#define cudaArrayTextureGather              0x08  // Must be set in cudaMallocArray or cudaMalloc3DArray in order to perform texture gather operations on the CUDA array

#define cudaIpcMemLazyEnablePeerAccess      0x01  // Automatically enable peer access between remote devices as needed

#define cudaMemAttachGlobal                 0x01  // Memory can be accessed by any stream on any device
#define cudaMemAttachHost                   0x02  // Memory cannot be accessed by any stream on any device
#define cudaMemAttachSingle                 0x04  // Memory can only be accessed by a single stream on the associated device

#define cudaOccupancyDefault                0x00  // Default behavior
#define cudaOccupancyDisableCachingOverride 0x01  // Assume global caching is enabled and cannot be automatically turned off

#define cudaCpuDeviceId                     ((int)-1) // Device id that represents the CPU
#define cudaInvalidDeviceId                 ((int)-2) // Device id that represents an invalid device

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dummy {
namespace cuda  {

// CUDA error types
enum cudaError
{
    cudaSuccess                           =      0,
    cudaErrorMissingConfiguration         =      1,
    cudaErrorMemoryAllocation             =      2,
    cudaErrorInitializationError          =      3,
    cudaErrorLaunchFailure                =      4,
    cudaErrorPriorLaunchFailure           =      5,
    cudaErrorLaunchTimeout                =      6,
    cudaErrorLaunchOutOfResources         =      7,
    cudaErrorInvalidDeviceFunction        =      8,
    cudaErrorInvalidConfiguration         =      9,
    cudaErrorInvalidDevice                =     10,
    cudaErrorInvalidValue                 =     11,
    cudaErrorInvalidPitchValue            =     12,
    cudaErrorInvalidSymbol                =     13,
    cudaErrorMapBufferObjectFailed        =     14,
    cudaErrorUnmapBufferObjectFailed      =     15,
    cudaErrorInvalidHostPointer           =     16,
    cudaErrorInvalidDevicePointer         =     17,
    cudaErrorInvalidTexture               =     18,
    cudaErrorInvalidTextureBinding        =     19,
    cudaErrorInvalidChannelDescriptor     =     20,
    cudaErrorInvalidMemcpyDirection       =     21,
    cudaErrorAddressOfConstant            =     22,
    cudaErrorTextureFetchFailed           =     23,
    cudaErrorTextureNotBound              =     24,
    cudaErrorSynchronizationError         =     25,
    cudaErrorInvalidFilterSetting         =     26,
    cudaErrorInvalidNormSetting           =     27,
    cudaErrorMixedDeviceExecution         =     28,
    cudaErrorCudartUnloading              =     29,
    cudaErrorUnknown                      =     30,
    cudaErrorNotYetImplemented            =     31,
    cudaErrorMemoryValueTooLarge          =     32,
    cudaErrorInvalidResourceHandle        =     33,
    cudaErrorNotReady                     =     34,
    cudaErrorInsufficientDriver           =     35,
    cudaErrorSetOnActiveProcess           =     36,
    cudaErrorInvalidSurface               =     37,
    cudaErrorNoDevice                     =     38,
    cudaErrorECCUncorrectable             =     39,
    cudaErrorSharedObjectSymbolNotFound   =     40,
    cudaErrorSharedObjectInitFailed       =     41,
    cudaErrorUnsupportedLimit             =     42,
    cudaErrorDuplicateVariableName        =     43,
    cudaErrorDuplicateTextureName         =     44,
    cudaErrorDuplicateSurfaceName         =     45,
    cudaErrorDevicesUnavailable           =     46,
    cudaErrorInvalidKernelImage           =     47,
    cudaErrorNoKernelImageForDevice       =     48,
    cudaErrorIncompatibleDriverContext    =     49,
    cudaErrorPeerAccessAlreadyEnabled     =     50,
    cudaErrorPeerAccessNotEnabled         =     51,
    cudaErrorDeviceAlreadyInUse           =     54,
    cudaErrorProfilerDisabled             =     55,
    cudaErrorProfilerNotInitialized       =     56,
    cudaErrorProfilerAlreadyStarted       =     57,
    cudaErrorProfilerAlreadyStopped       =     58,
    cudaErrorAssert                       =     59,
    cudaErrorTooManyPeers                 =     60,
    cudaErrorHostMemoryAlreadyRegistered  =     61,
    cudaErrorHostMemoryNotRegistered      =     62,
    cudaErrorOperatingSystem              =     63,
    cudaErrorPeerAccessUnsupported        =     64,
    cudaErrorLaunchMaxDepthExceeded       =     65,
    cudaErrorLaunchFileScopedTex          =     66,
    cudaErrorLaunchFileScopedSurf         =     67,
    cudaErrorSyncDepthExceeded            =     68,
    cudaErrorLaunchPendingCountExceeded   =     69,
    cudaErrorNotPermitted                 =     70,
    cudaErrorNotSupported                 =     71,
    cudaErrorHardwareStackError           =     72,
    cudaErrorIllegalInstruction           =     73,
    cudaErrorMisalignedAddress            =     74,
    cudaErrorInvalidAddressSpace          =     75,
    cudaErrorInvalidPc                    =     76,
    cudaErrorIllegalAddress               =     77,
    cudaErrorInvalidPtx                   =     78,
    cudaErrorInvalidGraphicsContext       =     79,
    cudaErrorNvlinkUncorrectable          =     80,
    cudaErrorStartupFailure               =   0x7f,
    cudaErrorApiFailureBase               =  10000,
};

// Channel format kind
enum cudaChannelFormatKind
{
    cudaChannelFormatKindSigned           =   0,      // Signed channel format
    cudaChannelFormatKindUnsigned         =   1,      // Unsigned channel format
    cudaChannelFormatKindFloat            =   2,      // Float channel format
    cudaChannelFormatKindNone             =   3,      // No channel format
};

// CUDA device compute modes
enum cudaComputeMode
{
    cudaComputeModeDefault          = 0, // Default compute mode (Multiple threads can use ::cudaSetDevice() with this device)
    cudaComputeModeExclusive        = 1, // Compute-exclusive-thread mode (Only one thread in one process will be able to use ::cudaSetDevice() with this device)
    cudaComputeModeProhibited       = 2, // Compute-prohibited mode (No threads can use ::cudaSetDevice() with this device)
    cudaComputeModeExclusiveProcess = 3, // Compute-exclusive-process mode (Many threads in one process will be able to use ::cudaSetDevice() with this device)
};

// CUDA memory types
enum cudaMemoryType
{
    cudaMemoryTypeHost   = 1, // Host memory
    cudaMemoryTypeDevice = 2, // Device memory
};

// CUDA memory copy types
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost          =   0,      // Host   -> Host
    cudaMemcpyHostToDevice        =   1,      // Host   -> Device
    cudaMemcpyDeviceToHost        =   2,      // Device -> Host
    cudaMemcpyDeviceToDevice      =   3,      // Device -> Device
    cudaMemcpyDefault             =   4,      // Direction of the transfer is inferred from the pointer values.
                                              // Requires unified virtual addressing
};

// CUDA resource types
enum cudaResourceType
{
    cudaResourceTypeArray          = 0x00, // Array resource
    cudaResourceTypeMipmappedArray = 0x01, // Mipmapped array resource
    cudaResourceTypeLinear         = 0x02, // Linear resource
    cudaResourceTypePitch2D        = 0x03, // Pitch 2D resource
};

// CUDA texture resource view formats
enum cudaResourceViewFormat
{
    cudaResViewFormatNone                      = 0x00, // No resource view format (use underlying resource format)
    cudaResViewFormatUnsignedChar1             = 0x01, // 1 channel unsigned 8-bit integers
    cudaResViewFormatUnsignedChar2             = 0x02, // 2 channel unsigned 8-bit integers
    cudaResViewFormatUnsignedChar4             = 0x03, // 4 channel unsigned 8-bit integers
    cudaResViewFormatSignedChar1               = 0x04, // 1 channel signed 8-bit integers
    cudaResViewFormatSignedChar2               = 0x05, // 2 channel signed 8-bit integers
    cudaResViewFormatSignedChar4               = 0x06, // 4 channel signed 8-bit integers
    cudaResViewFormatUnsignedShort1            = 0x07, // 1 channel unsigned 16-bit integers
    cudaResViewFormatUnsignedShort2            = 0x08, // 2 channel unsigned 16-bit integers
    cudaResViewFormatUnsignedShort4            = 0x09, // 4 channel unsigned 16-bit integers
    cudaResViewFormatSignedShort1              = 0x0a, // 1 channel signed 16-bit integers
    cudaResViewFormatSignedShort2              = 0x0b, // 2 channel signed 16-bit integers
    cudaResViewFormatSignedShort4              = 0x0c, // 4 channel signed 16-bit integers
    cudaResViewFormatUnsignedInt1              = 0x0d, // 1 channel unsigned 32-bit integers
    cudaResViewFormatUnsignedInt2              = 0x0e, // 2 channel unsigned 32-bit integers
    cudaResViewFormatUnsignedInt4              = 0x0f, // 4 channel unsigned 32-bit integers
    cudaResViewFormatSignedInt1                = 0x10, // 1 channel signed 32-bit integers
    cudaResViewFormatSignedInt2                = 0x11, // 2 channel signed 32-bit integers
    cudaResViewFormatSignedInt4                = 0x12, // 4 channel signed 32-bit integers
    cudaResViewFormatHalf1                     = 0x13, // 1 channel 16-bit floating point
    cudaResViewFormatHalf2                     = 0x14, // 2 channel 16-bit floating point
    cudaResViewFormatHalf4                     = 0x15, // 4 channel 16-bit floating point
    cudaResViewFormatFloat1                    = 0x16, // 1 channel 32-bit floating point
    cudaResViewFormatFloat2                    = 0x17, // 2 channel 32-bit floating point
    cudaResViewFormatFloat4                    = 0x18, // 4 channel 32-bit floating point
    cudaResViewFormatUnsignedBlockCompressed1  = 0x19, // Block compressed 1
    cudaResViewFormatUnsignedBlockCompressed2  = 0x1a, // Block compressed 2
    cudaResViewFormatUnsignedBlockCompressed3  = 0x1b, // Block compressed 3
    cudaResViewFormatUnsignedBlockCompressed4  = 0x1c, // Block compressed 4 unsigned
    cudaResViewFormatSignedBlockCompressed4    = 0x1d, // Block compressed 4 signed
    cudaResViewFormatUnsignedBlockCompressed5  = 0x1e, // Block compressed 5 unsigned
    cudaResViewFormatSignedBlockCompressed5    = 0x1f, // Block compressed 5 signed
    cudaResViewFormatUnsignedBlockCompressed6H = 0x20, // Block compressed 6 unsigned half-float
    cudaResViewFormatSignedBlockCompressed6H   = 0x21, // Block compressed 6 signed half-float
    cudaResViewFormatUnsignedBlockCompressed7  = 0x22, // Block compressed 7
};

// CUDA device attributes
enum cudaDeviceAttr
{
    cudaDevAttrMaxThreadsPerBlock                =  1, // Maximum number of threads per block
    cudaDevAttrMaxBlockDimX                      =  2, // Maximum block dimension X
    cudaDevAttrMaxBlockDimY                      =  3, // Maximum block dimension Y
    cudaDevAttrMaxBlockDimZ                      =  4, // Maximum block dimension Z
    cudaDevAttrMaxGridDimX                       =  5, // Maximum grid dimension X
    cudaDevAttrMaxGridDimY                       =  6, // Maximum grid dimension Y
    cudaDevAttrMaxGridDimZ                       =  7, // Maximum grid dimension Z
    cudaDevAttrMaxSharedMemoryPerBlock           =  8, // Maximum shared memory available per block in bytes
    cudaDevAttrTotalConstantMemory               =  9, // Memory available on device for __constant__ variables in a CUDA C kernel in bytes
    cudaDevAttrWarpSize                          = 10, // Warp size in threads
    cudaDevAttrMaxPitch                          = 11, // Maximum pitch in bytes allowed by memory copies
    cudaDevAttrMaxRegistersPerBlock              = 12, // Maximum number of 32-bit registers available per block
    cudaDevAttrClockRate                         = 13, // Peak clock frequency in kilohertz
    cudaDevAttrTextureAlignment                  = 14, // Alignment requirement for textures
    cudaDevAttrGpuOverlap                        = 15, // Device can possibly copy memory and execute a kernel concurrently
    cudaDevAttrMultiProcessorCount               = 16, // Number of multiprocessors on device
    cudaDevAttrKernelExecTimeout                 = 17, // Specifies whether there is a run time limit on kernels
    cudaDevAttrIntegrated                        = 18, // Device is integrated with host memory
    cudaDevAttrCanMapHostMemory                  = 19, // Device can map host memory into CUDA address space
    cudaDevAttrComputeMode                       = 20, // Compute mode (See ::cudaComputeMode for details)
    cudaDevAttrMaxTexture1DWidth                 = 21, // Maximum 1D texture width
    cudaDevAttrMaxTexture2DWidth                 = 22, // Maximum 2D texture width
    cudaDevAttrMaxTexture2DHeight                = 23, // Maximum 2D texture height
    cudaDevAttrMaxTexture3DWidth                 = 24, // Maximum 3D texture width
    cudaDevAttrMaxTexture3DHeight                = 25, // Maximum 3D texture height
    cudaDevAttrMaxTexture3DDepth                 = 26, // Maximum 3D texture depth
    cudaDevAttrMaxTexture2DLayeredWidth          = 27, // Maximum 2D layered texture width
    cudaDevAttrMaxTexture2DLayeredHeight         = 28, // Maximum 2D layered texture height
    cudaDevAttrMaxTexture2DLayeredLayers         = 29, // Maximum layers in a 2D layered texture
    cudaDevAttrSurfaceAlignment                  = 30, // Alignment requirement for surfaces
    cudaDevAttrConcurrentKernels                 = 31, // Device can possibly execute multiple kernels concurrently
    cudaDevAttrEccEnabled                        = 32, // Device has ECC support enabled
    cudaDevAttrPciBusId                          = 33, // PCI bus ID of the device
    cudaDevAttrPciDeviceId                       = 34, // PCI device ID of the device
    cudaDevAttrTccDriver                         = 35, // Device is using TCC driver model
    cudaDevAttrMemoryClockRate                   = 36, // Peak memory clock frequency in kilohertz
    cudaDevAttrGlobalMemoryBusWidth              = 37, // Global memory bus width in bits
    cudaDevAttrL2CacheSize                       = 38, // Size of L2 cache in bytes
    cudaDevAttrMaxThreadsPerMultiProcessor       = 39, // Maximum resident threads per multiprocessor
    cudaDevAttrAsyncEngineCount                  = 40, // Number of asynchronous engines
    cudaDevAttrUnifiedAddressing                 = 41, // Device shares a unified address space with the host
    cudaDevAttrMaxTexture1DLayeredWidth          = 42, // Maximum 1D layered texture width
    cudaDevAttrMaxTexture1DLayeredLayers         = 43, // Maximum layers in a 1D layered texture
    cudaDevAttrMaxTexture2DGatherWidth           = 45, // Maximum 2D texture width if cudaArrayTextureGather is set
    cudaDevAttrMaxTexture2DGatherHeight          = 46, // Maximum 2D texture height if cudaArrayTextureGather is set
    cudaDevAttrMaxTexture3DWidthAlt              = 47, // Alternate maximum 3D texture width
    cudaDevAttrMaxTexture3DHeightAlt             = 48, // Alternate maximum 3D texture height
    cudaDevAttrMaxTexture3DDepthAlt              = 49, // Alternate maximum 3D texture depth
    cudaDevAttrPciDomainId                       = 50, // PCI domain ID of the device
    cudaDevAttrTexturePitchAlignment             = 51, // Pitch alignment requirement for textures
    cudaDevAttrMaxTextureCubemapWidth            = 52, // Maximum cubemap texture width/height
    cudaDevAttrMaxTextureCubemapLayeredWidth     = 53, // Maximum cubemap layered texture width/height
    cudaDevAttrMaxTextureCubemapLayeredLayers    = 54, // Maximum layers in a cubemap layered texture
    cudaDevAttrMaxSurface1DWidth                 = 55, // Maximum 1D surface width
    cudaDevAttrMaxSurface2DWidth                 = 56, // Maximum 2D surface width
    cudaDevAttrMaxSurface2DHeight                = 57, // Maximum 2D surface height
    cudaDevAttrMaxSurface3DWidth                 = 58, // Maximum 3D surface width
    cudaDevAttrMaxSurface3DHeight                = 59, // Maximum 3D surface height
    cudaDevAttrMaxSurface3DDepth                 = 60, // Maximum 3D surface depth
    cudaDevAttrMaxSurface1DLayeredWidth          = 61, // Maximum 1D layered surface width
    cudaDevAttrMaxSurface1DLayeredLayers         = 62, // Maximum layers in a 1D layered surface
    cudaDevAttrMaxSurface2DLayeredWidth          = 63, // Maximum 2D layered surface width
    cudaDevAttrMaxSurface2DLayeredHeight         = 64, // Maximum 2D layered surface height
    cudaDevAttrMaxSurface2DLayeredLayers         = 65, // Maximum layers in a 2D layered surface
    cudaDevAttrMaxSurfaceCubemapWidth            = 66, // Maximum cubemap surface width
    cudaDevAttrMaxSurfaceCubemapLayeredWidth     = 67, // Maximum cubemap layered surface width
    cudaDevAttrMaxSurfaceCubemapLayeredLayers    = 68, // Maximum layers in a cubemap layered surface
    cudaDevAttrMaxTexture1DLinearWidth           = 69, // Maximum 1D linear texture width
    cudaDevAttrMaxTexture2DLinearWidth           = 70, // Maximum 2D linear texture width
    cudaDevAttrMaxTexture2DLinearHeight          = 71, // Maximum 2D linear texture height
    cudaDevAttrMaxTexture2DLinearPitch           = 72, // Maximum 2D linear texture pitch in bytes
    cudaDevAttrMaxTexture2DMipmappedWidth        = 73, // Maximum mipmapped 2D texture width
    cudaDevAttrMaxTexture2DMipmappedHeight       = 74, // Maximum mipmapped 2D texture height
    cudaDevAttrComputeCapabilityMajor            = 75, // Major compute capability version number
    cudaDevAttrComputeCapabilityMinor            = 76, // Minor compute capability version number
    cudaDevAttrMaxTexture1DMipmappedWidth        = 77, // Maximum mipmapped 1D texture width
    cudaDevAttrStreamPrioritiesSupported         = 78, // Device supports stream priorities
    cudaDevAttrGlobalL1CacheSupported            = 79, // Device supports caching globals in L1
    cudaDevAttrLocalL1CacheSupported             = 80, // Device supports caching locals in L1
    cudaDevAttrMaxSharedMemoryPerMultiprocessor  = 81, // Maximum shared memory available per multiprocessor in bytes
    cudaDevAttrMaxRegistersPerMultiprocessor     = 82, // Maximum number of 32-bit registers available per multiprocessor
    cudaDevAttrManagedMemory                     = 83, // Device can allocate managed memory on this system
    cudaDevAttrIsMultiGpuBoard                   = 84, // Device is on a multi-GPU board
    cudaDevAttrMultiGpuBoardGroupID              = 85, // Unique identifier for a group of devices on the same multi-GPU board
    cudaDevAttrHostNativeAtomicSupported         = 86, // Link between the device and the host supports native atomic operations
    cudaDevAttrSingleToDoublePrecisionPerfRatio  = 87, // Ratio of single precision performance (in floating-point operations per second) to double precision performance
    cudaDevAttrPageableMemoryAccess              = 88, // Device supports coherently accessing pageable memory without calling cudaHostRegister on it
    cudaDevAttrConcurrentManagedAccess           = 89, // Device can coherently access managed memory concurrently with the CPU
    cudaDevAttrComputePreemptionSupported        = 90, // Device supports Compute Preemption
    cudaDevAttrCanUseHostPointerForRegisteredMem = 91, // Device can access host registered memory at the same virtual address as the CPU
};

// CUDA function cache configurations
enum cudaFuncCache
{
    cudaFuncCachePreferNone   = 0, // Default function cache configuration, no preference
    cudaFuncCachePreferShared = 1, // Prefer larger shared memory and smaller L1 cache
    cudaFuncCachePreferL1     = 2, // Prefer larger L1 cache and smaller shared memory
    cudaFuncCachePreferEqual  = 3, // Prefer equal size L1 cache and shared memory
};

// CUDA shared memory configuration
enum cudaSharedMemConfig
{
    cudaSharedMemBankSizeDefault   = 0,
    cudaSharedMemBankSizeFourByte  = 1,
    cudaSharedMemBankSizeEightByte = 2,
};

// CUDA Limits
enum cudaLimit
{
    cudaLimitStackSize                    = 0x00, // GPU thread stack size
    cudaLimitPrintfFifoSize               = 0x01, // GPU printf/fprintf FIFO size
    cudaLimitMallocHeapSize               = 0x02, // GPU malloc heap size
    cudaLimitDevRuntimeSyncDepth          = 0x03, // GPU device runtime synchronize depth
    cudaLimitDevRuntimePendingLaunchCount = 0x04, // GPU device runtime pending launch count
};

// CUDA Memory Advise values
enum cudaMemoryAdvise
{
    cudaMemAdviseSetReadMostly          = 1, // Data will mostly be read and only occassionally be written to
    cudaMemAdviseUnsetReadMostly        = 2, // Undo the effect of ::cudaMemAdviseSetReadMostly
    cudaMemAdviseSetPreferredLocation   = 3, // Set the preferred location for the data as the specified device
    cudaMemAdviseUnsetPreferredLocation = 4, // Clear the preferred location for the data
    cudaMemAdviseSetAccessedBy          = 5, // Data will be accessed by the specified device, so prevent page faults as much as possible
    cudaMemAdviseUnsetAccessedBy        = 6, // Let the Unified Memory subsystem decide on the page faulting policy for the specified device
};

// CUDA range attributes
enum cudaMemRangeAttribute
{
    cudaMemRangeAttributeReadMostly           = 1, // Whether the range will mostly be read and only occassionally be written to
    cudaMemRangeAttributePreferredLocation    = 2, // The preferred location of the range
    cudaMemRangeAttributeAccessedBy           = 3, // Memory range has ::cudaMemAdviseSetAccessedBy set for specified device
    cudaMemRangeAttributeLastPrefetchLocation = 4, // The last location to which the range was prefetched
};

// CUDA Profiler Output modes
enum cudaOutputMode
{
    cudaKeyValuePair    = 0x00, // Output mode Key-Value pair format.
    cudaCSV             = 0x01, // Output mode Comma separated values format.
};

struct cudaArray          { /* EMPTY. */ };
struct cudaMipmappedArray { /* EMPTY. */ };
struct cudaEvent          { /* EMPTY. */ };
struct cudaStream         { /* EMPTY. */ };

typedef         enum cudaError              cudaError_t;
typedef       struct cudaArray *            cudaArray_t;
typedef const struct cudaArray *            cudaArray_const_t;
typedef       struct cudaEvent *            cudaEvent_t;
typedef       struct cudaStream *           cudaStream_t;
typedef       struct cudaMipmappedArray *   cudaMipmappedArray_t;

struct cudaDeviceProp
{
    char   name[256];                        // ASCII string identifying device
    size_t totalGlobalMem;                   // Global memory available on device in bytes
    size_t sharedMemPerBlock;                // Shared memory available per block in bytes
    int    regsPerBlock;                     // 32-bit registers available per block
    int    warpSize;                         // Warp size in threads
    size_t memPitch;                         // Maximum pitch in bytes allowed by memory copies
    int    maxThreadsPerBlock;               // Maximum number of threads per block
    int    maxThreadsDim[3];                 // Maximum size of each dimension of a block
    int    maxGridSize[3];                   // Maximum size of each dimension of a grid
    int    clockRate;                        // Clock frequency in kilohertz
    size_t totalConstMem;                    // Constant memory available on device in bytes
    int    major;                            // Major compute capability
    int    minor;                            // Minor compute capability
    size_t textureAlignment;                 // Alignment requirement for textures
    size_t texturePitchAlignment;            // Pitch alignment requirement for texture references bound to pitched memory
    int    deviceOverlap;                    // Device can concurrently copy memory and execute a kernel. Deprecated. Use instead asyncEngineCount.
    int    multiProcessorCount;              // Number of multiprocessors on device
    int    kernelExecTimeoutEnabled;         // Specified whether there is a run time limit on kernels
    int    integrated;                       // Device is integrated as opposed to discrete
    int    canMapHostMemory;                 // Device can map host memory with cudaHostAlloc/cudaHostGetDevicePointer
    int    computeMode;                      // Compute mode (See ::cudaComputeMode)
    int    maxTexture1D;                     // Maximum 1D texture size
    int    maxTexture1DMipmap;               // Maximum 1D mipmapped texture size
    int    maxTexture1DLinear;               // Maximum size for 1D textures bound to linear memory
    int    maxTexture2D[2];                  // Maximum 2D texture dimensions
    int    maxTexture2DMipmap[2];            // Maximum 2D mipmapped texture dimensions
    int    maxTexture2DLinear[3];            // Maximum dimensions (width, height, pitch) for 2D textures bound to pitched memory
    int    maxTexture2DGather[2];            // Maximum 2D texture dimensions if texture gather operations have to be performed
    int    maxTexture3D[3];                  // Maximum 3D texture dimensions
    int    maxTexture3DAlt[3];               // Maximum alternate 3D texture dimensions
    int    maxTextureCubemap;                // Maximum Cubemap texture dimensions
    int    maxTexture1DLayered[2];           // Maximum 1D layered texture dimensions
    int    maxTexture2DLayered[3];           // Maximum 2D layered texture dimensions
    int    maxTextureCubemapLayered[2];      // Maximum Cubemap layered texture dimensions
    int    maxSurface1D;                     // Maximum 1D surface size
    int    maxSurface2D[2];                  // Maximum 2D surface dimensions
    int    maxSurface3D[3];                  // Maximum 3D surface dimensions
    int    maxSurface1DLayered[2];           // Maximum 1D layered surface dimensions
    int    maxSurface2DLayered[3];           // Maximum 2D layered surface dimensions
    int    maxSurfaceCubemap;                // Maximum Cubemap surface dimensions
    int    maxSurfaceCubemapLayered[2];      // Maximum Cubemap layered surface dimensions
    size_t surfaceAlignment;                 // Alignment requirements for surfaces
    int    concurrentKernels;                // Device can possibly execute multiple kernels concurrently
    int    ECCEnabled;                       // Device has ECC support enabled
    int    pciBusID;                         // PCI bus ID of the device
    int    pciDeviceID;                      // PCI device ID of the device
    int    pciDomainID;                      // PCI domain ID of the device
    int    tccDriver;                        // 1 if device is a Tesla device using TCC driver, 0 otherwise
    int    asyncEngineCount;                 // Number of asynchronous engines
    int    unifiedAddressing;                // Device shares a unified address space with the host
    int    memoryClockRate;                  // Peak memory clock frequency in kilohertz
    int    memoryBusWidth;                   // Global memory bus width in bits
    int    l2CacheSize;                      // Size of L2 cache in bytes
    int    maxThreadsPerMultiProcessor;      // Maximum resident threads per multiprocessor
    int    streamPrioritiesSupported;        // Device supports stream priorities
    int    globalL1CacheSupported;           // Device supports caching globals in L1
    int    localL1CacheSupported;            // Device supports caching locals in L1
    size_t sharedMemPerMultiprocessor;       // Shared memory available per multiprocessor in bytes
    int    regsPerMultiprocessor;            // 32-bit registers available per multiprocessor
    int    managedMemory;                    // Device supports allocating managed memory on this system
    int    isMultiGpuBoard;                  // Device is on a multi-GPU board
    int    multiGpuBoardGroupID;             // Unique identifier for a group of devices on the same multi-GPU board
    int    hostNativeAtomicSupported;        // Link between the device and the host supports native atomic operations
    int    singleToDoublePrecisionPerfRatio; // Ratio of single precision performance (in floating-point operations per second) to double precision performance
    int    pageableMemoryAccess;             // Device supports coherently accessing pageable memory without calling cudaHostRegister on it
    int    concurrentManagedAccess;          // Device can coherently access managed memory concurrently with the CPU
};

// CUDA Channel format descriptor
struct cudaChannelFormatDesc
{
    int                        x; // x
    int                        y; // y
    int                        z; // z
    int                        w; // w
    enum cudaChannelFormatKind f; // Channel format kind
};

// CUDA resource descriptor
struct cudaResourceDesc
{
    enum cudaResourceType resType;             // Resource type

    union {
        struct {
            cudaArray_t array;                 // CUDA array
        } array;
        struct {
            cudaMipmappedArray_t mipmap;       // CUDA mipmapped array
        } mipmap;
        struct {
            void *devPtr;                      // Device pointer
            struct cudaChannelFormatDesc desc; // Channel descriptor
            size_t sizeInBytes;                // Size in bytes
        } linear;
        struct {
            void *devPtr;                      // Device pointer
            struct cudaChannelFormatDesc desc; // Channel descriptor
            size_t width;                      // Width of the array in elements
            size_t height;                     // Height of the array in elements
            size_t pitchInBytes;               // Pitch between two rows in bytes
        } pitch2D;
    } res;
};

// CUDA resource view descriptor
struct cudaResourceViewDesc
{
    enum cudaResourceViewFormat format;           // Resource view format
    size_t                      width;            // Width of the resource view
    size_t                      height;           // Height of the resource view
    size_t                      depth;            // Depth of the resource view
    unsigned int                firstMipmapLevel; // First defined mipmap level
    unsigned int                lastMipmapLevel;  // Last defined mipmap level
    unsigned int                firstLayer;       // First layer index
    unsigned int                lastLayer;        // Last layer index
};

// CUDA pointer attributes
struct cudaPointerAttributes
{
    enum cudaMemoryType memoryType;
    int device;
    void * devicePointer;
    void * hostPointer;
    int isManaged;
};

// CUDA function attributes
struct cudaFuncAttributes
{
    size_t sharedSizeBytes;
    size_t constSizeBytes;
    size_t localSizeBytes;
    int maxThreadsPerBlock;
    int numRegs;
    int ptxVersion;
    int binaryVersion;
    int cacheModeCA;
};

CUDARTAPI char const * cudaGetErrorString(cudaError_t err)
{
    if (err == cudaSuccess) {
        return "cudaSuccess";
    } else if (err == cudaErrorNotSupported) {
        return "cudaErrorNotSupported";
    }
    return "UNKNOWN";
}

CUDARTAPI cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaGetDeviceCount(int * count)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaGetDevice(int * device)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaSetDevice(int device)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaDriverGetVersion(int * driverVersion)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaRuntimeGetVersion(int * runtimeVersion)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp * prop, int device)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaStreamCreate(cudaStream_t * stream)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaStreamDestroy(cudaStream_t stream)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaEventCreate(cudaEvent_t * event)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaEventDestroy(cudaEvent_t event)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaEventSynchronize(cudaEvent_t event)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaEventElapsedTime(float * ms, cudaEvent_t start, cudaEvent_t end)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaMalloc(void ** devPtr, size_t size)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaMallocHost(void ** ptr, size_t size)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaFree(void * devPtr)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaFreeHost(void * ptr)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaMemcpy(void * dst, const void * src, size_t count, enum cudaMemcpyKind kind)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaMemcpyAsync(void * dst, const void * src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream = 0)
{ return cudaErrorNotSupported; }

CUDARTAPI cudaError_t cudaStreamSynchronize(cudaStream_t stream)
{ return cudaErrorNotSupported; }

// Returns grid and block size that achieves maximum potential occupancy for a device function.
template <typename T>
CUDARTAPI cudaError_t cudaOccupancyMaxPotentialBlockSize(
        int * minGridSize, int * blockSize, T func, size_t dynamicSMemSize = 0, int blockSizeLimit = 0)
{ return cudaErrorNotSupported; }

// Returns grid and block size that achieves maximum potential occupancy for a device function.
template <typename UnaryFunction, typename T>
CUDARTAPI cudaError_t cudaOccupancyMaxPotentialBlockSizeVariableSMem(
        int * minGridSize, int * blockSize, T func, UnaryFunction blockSizeToDynamicSMemSize, int blockSizeLimit = 0)
{ return cudaErrorNotSupported; }

} // namespace cuda
} // namespace dummy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DUMMY_CUDA_HPP__

