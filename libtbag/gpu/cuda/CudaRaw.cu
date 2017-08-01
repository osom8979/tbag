/**
 * @file   CudaRaw.cu
 * @brief  CUDA raw implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <cuda.h>

__global__ void add(int * a, int * b, int * c, int N)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < N) {
        c[idx] = a[idx] + b[idx];
    }
}

int tbCudaAddByGpu(int const * lh, int const * rh, int * result, int size)
{
    int * device_lh;
    int * device_rh;
    int * device_result;

    ::cudaMalloc((void**)&device_lh, size);
    ::cudaMalloc((void**)&device_rh, size);
    ::cudaMalloc((void**)&device_result, size);

    ::cudaMemcpy(device_lh, lh, size, ::cudaMemcpyHostToDevice);
    ::cudaMemcpy(device_rh, rh, size, ::cudaMemcpyHostToDevice);

    add<<<size, 1>>>(device_lh, device_rh, device_result);

    ::cudaMemcpy(result, device_rh, size, ::cudaMemcpyDeviceToHost);

    ::cudaFree(device_lh);
    ::cudaFree(device_rh);
    ::cudaFree(device_result);

    return TB_CUDA_TRUE;
}

