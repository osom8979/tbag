/**
 * @file   CudaRaw.cu
 * @brief  CUDA raw implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <cuda.h>

__global__ void addByOnlyBlocks(int * lh, int * rh, int * result)
{
    result[blockIdx.x] = lh[blockIdx.x] + rh[blockIdx.x];
}

int tbCudaAddByGpu(int const * lh, int const * rh, int * result, int size)
{
    int const MEMORY_SIZE = size * sizeof(int);
    int * device_lh;
    int * device_rh;
    int * device_result;

    ::cudaMalloc((void**)&device_lh    , MEMORY_SIZE);
    ::cudaMalloc((void**)&device_rh    , MEMORY_SIZE);
    ::cudaMalloc((void**)&device_result, MEMORY_SIZE);

    ::cudaMemcpy(device_lh, lh, MEMORY_SIZE, ::cudaMemcpyHostToDevice);
    ::cudaMemcpy(device_rh, rh, MEMORY_SIZE, ::cudaMemcpyHostToDevice);

    addByOnlyBlocks<<<size, 1>>>(device_lh, device_rh, device_result);
    ::cudaDeviceSynchronize();

    ::cudaMemcpy(result, device_result, MEMORY_SIZE, ::cudaMemcpyDeviceToHost);

    ::cudaFree(device_lh);
    ::cudaFree(device_rh);
    ::cudaFree(device_result);

    return TB_CUDA_TRUE;
}

