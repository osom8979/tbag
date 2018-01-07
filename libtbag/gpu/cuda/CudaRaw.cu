/**
 * @file   CudaRaw.cu
 * @brief  CUDA raw implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.h>
#include <cuda.h>

template <typename T>
__global__ void tbCudaAddKernel(T const * v1, T const * v2, T * result, int size)
{
    int const index = (blockIdx.x * blockDim.x) + threadIdx.x;
    if (index < size) {
        result[index] = v1[index] + v2[index];
    }
}

int tbCudaAdd1f(float const * gpu_v1, float const * gpu_v2, float * gpu_result, int size)
{
    int block_size    = 0;
    int min_grid_size = 0;
    int grid_size     = 0;

    ::cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, kernel, 0, size);
    grid_size = (size + block_size - 1) / block_size;
    grid_size = (min_grid_size > grid_size ? min_grid_size : grid_size);
    tbCudaAddKernel<float><<<grid_size, block_size>>>(gpu_v1, gpu_v2, gpu_result, size);

    return TB_CUDA_TRUE;
}

int tbCudaAdd1d(double const * gpu_v1, double const * gpu_v2, double * gpu_result, int size)
{
    int block_size    = 0;
    int min_grid_size = 0;
    int grid_size     = 0;

    ::cudaOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, kernel, 0, size);
    grid_size = (size + block_size - 1) / block_size;
    grid_size = (min_grid_size > grid_size ? min_grid_size : grid_size);
    tbCudaAddKernel<double><<<grid_size, block_size>>>(gpu_v1, gpu_v2, gpu_result, size);

    return TB_CUDA_TRUE;
}

