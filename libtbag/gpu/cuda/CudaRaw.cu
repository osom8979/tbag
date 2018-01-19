/**
 * @file   CudaRaw.cu
 * @brief  CUDA raw implementation.
 * @author zer0
 * @date   2017-08-01
 *
 * @warning
 *  Don't use the modern C++(0x, 11, ...);
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <cstdio>
#include <cassert>

#include <cuda.h>
#include <cuda_runtime.h>

#ifndef TB_CUDA_DEVICE
#define TB_CUDA_DEVICE  __device__
#endif

#ifndef TB_CUDA_HOST
#define TB_CUDA_HOST  __host__
#endif

#ifndef TB_CUDA_GLOBAL
#define TB_CUDA_GLOBAL  __global__
#endif

#ifndef TB_CUDA_INLINE
#define TB_CUDA_INLINE  __forceinline__
#endif

static bool isTbagCudaRawVerbose()
{
#if defined(TBAG_CUDA_RAW_VERBOSE)
    return true;
#else
    return false;
#endif
}

#ifndef tbCudaRawVerbose
#define tbCudaRawVerbose(condition, ...) \
    if (condition) { printf(__VA_ARGS__); }
#endif

// ---------------
namespace __impl {
// ---------------

template <typename T>
bool tbCudaGetMaxPotentialBlockSize(int * result_grid_size, int * result_block_size, T func, std::size_t array_count,
                                    std::size_t dynamic_shared_mem_size = 0, int block_size_limit = 0)
{
    int block_size    = 0; // The launch configurator returned block size.
    int min_grid_size = 0; // The minimum grid size needed to achieve the maximum occupancy for a full device launch.
    int round_up_grid = 0; // The actual grid size needed, based on input size.

    cudaError_t code = ::cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, func,
                                                            dynamic_shared_mem_size, block_size_limit);
    if (code != cudaSuccess) {
        return false;
    }
    round_up_grid = (array_count + block_size - 1) / block_size; // Round up according to array size.
    round_up_grid = (round_up_grid > min_grid_size ? round_up_grid : min_grid_size);
    tbCudaRawVerbose(isTbagCudaRawVerbose(), "tbCudaGetMaxPotentialBlockSize() GRID: %d, BLOCK: %d", round_up_grid, block_size);
    if (result_grid_size != TB_NULL) {
        *result_grid_size = round_up_grid;
    }
    if (result_block_size != TB_NULL) {
        *result_block_size = block_size;
    }
    return true;
}

static TB_CUDA_INLINE TB_CUDA_DEVICE int __global_index_1g_1b__()
{
    return (blockIdx.x * blockDim.x) /*GLOBAL_BLOCK_OFFSET*/
           + threadIdx.x /*LOCAL_THREAD_OFFSET*/;
}

//static TB_CUDA_INLINE TB_CUDA_DEVICE int __global_index_1g_2b__()
//{
//    return (blockIdx.x * (blockDim.x * blockDim.y)) /*GLOBAL_BLOCK_OFFSET*/
//           + (threadIdx.y * blockDim.x) /*LOCAL_THREAD_Y_OFFSET*/
//           + threadIdx.x /*LOCAL_THREAD_X_OFFSET*/;
//}
//
//static TB_CUDA_INLINE TB_CUDA_DEVICE int __global_index_1g_3b__()
//{
//    return (blockIdx.x * (blockDim.x * blockDim.y * blockDim.z)) /*GLOBAL_BLOCK_OFFSET*/
//           + (threadIdx.z * (blockDim.y * blockDim.x)) /*LOCAL_THREAD_X_OFFSET*/
//           + (threadIdx.y * blockDim.x) /*LOCAL_THREAD_Y_OFFSET*/
//           + threadIdx.x /*LOCAL_THREAD_X_OFFSET*/;
//}

template <typename T>
TB_CUDA_GLOBAL void tbCudaAddKernel(T const * v1, T const * v2, T * r, int size)
{
    int const index = __global_index_1g_1b__();
    if (index < size) {
        r[index] = v1[index] + v2[index];
    }
}

template <typename T, typename StreamType>
bool tbCudaAdd(T const * v1, T const * v2, T * r, int count, StreamType stream)
{
    int grid_size(0), block_size(0);
    if (tbCudaGetMaxPotentialBlockSize(&grid_size, &block_size, tbCudaAddKernel<T>, count) == false) {
        return TB_FALSE;
    }
    assert(grid_size > 0);
    assert(block_size > 0);

    tbCudaAddKernel<T><<<grid_size, block_size, 0, stream>>>(v1, v2, r, count);
    return TB_TRUE;
}

// ------------------
} // namespace __impl
// ------------------

tbBOOL tbCudaAdd1i(int const * v1, int const * v2, int * r, int count, void * stream)
{ return __impl::tbCudaAdd(gpu_v1, v2, r, count, (cudaStream_t)stream) ? TB_TRUE : TB_FALSE; }
tbBOOL tbCudaAdd1u(unsigned const * v1, unsigned const * v2, unsigned * r, int count, void * stream)
{ return __impl::tbCudaAdd(gpu_v1, v2, r, count, (cudaStream_t)stream) ? TB_TRUE : TB_FALSE; }
tbBOOL tbCudaAdd1f(float const * v1, float const * v2, float * r, int count, void * stream)
{ return __impl::tbCudaAdd(gpu_v1, v2, r, count, (cudaStream_t)stream) ? TB_TRUE : TB_FALSE; }
tbBOOL tbCudaAdd1d(double const * v1, double const * v2, double * r, int count, void * stream)
{ return __impl::tbCudaAdd(gpu_v1, v2, r, count, (cudaStream_t)stream) ? TB_TRUE : TB_FALSE; }

