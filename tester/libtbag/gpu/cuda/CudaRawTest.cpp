/**
 * @file   CudaRawTest.cpp
 * @brief  CudaRaw class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/cuda/CudaRaw.h>
#include <libtbag/gpu/GpuBackend.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(CudaRawTest, AddByCpu)
{
    int const SIZE = 10;
    int a[SIZE] = {0,   1,   2,   3,   4,   5,   6,   7,   8,   9};
    int b[SIZE] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900};
    int c[SIZE] = {0,};

    ASSERT_EQ(TB_CUDA_TRUE, tbCudaAddByCpu(a, b, c, SIZE));
    ASSERT_EQ(  0, c[0]);
    ASSERT_EQ(101, c[1]);
    ASSERT_EQ(202, c[2]);
    ASSERT_EQ(303, c[3]);
    ASSERT_EQ(404, c[4]);
    ASSERT_EQ(505, c[5]);
    ASSERT_EQ(606, c[6]);
    ASSERT_EQ(707, c[7]);
    ASSERT_EQ(808, c[8]);
    ASSERT_EQ(909, c[9]);
}

TEST(CudaRawTest, AddByGpu)
{
    if (isCudaSupport() == false) {
        return;
    }

    int const SIZE = 10;
    int a[SIZE] = {0,   1,   2,   3,   4,   5,   6,   7,   8,   9};
    int b[SIZE] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900};
    int c[SIZE] = {0,};

    ASSERT_EQ(TB_CUDA_TRUE, tbCudaAddByGpu(a, b, c, SIZE));
    ASSERT_EQ(  0, c[0]);
    ASSERT_EQ(101, c[1]);
    ASSERT_EQ(202, c[2]);
    ASSERT_EQ(303, c[3]);
    ASSERT_EQ(404, c[4]);
    ASSERT_EQ(505, c[5]);
    ASSERT_EQ(606, c[6]);
    ASSERT_EQ(707, c[7]);
    ASSERT_EQ(808, c[8]);
    ASSERT_EQ(909, c[9]);
}

