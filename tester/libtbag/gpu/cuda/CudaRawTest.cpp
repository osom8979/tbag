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
    std::vector<int> a = {  1,   2,   3,   4};
    std::vector<int> b = {100, 200, 300, 400};
    std::vector<int> c(a.size());

    ASSERT_EQ(TB_CUDA_TRUE, tbCudaAddByCpu(&a[0], &b[0], &c[0], c.size()));
    ASSERT_EQ(4, c.size());
    ASSERT_EQ(101, c[0]);
    ASSERT_EQ(202, c[1]);
    ASSERT_EQ(303, c[2]);
    ASSERT_EQ(404, c[3]);
}

TEST(CudaRawTest, AddByGpu)
{
    if (isCudaSupport()) {
        std::vector<int> a = {  1,   2,   3,   4,   5,   6,   7,   8,   9};
        std::vector<int> b = {100, 200, 300, 400, 500, 600, 700, 800, 900};
        std::vector<int> c(a.size());

        ASSERT_EQ(TB_CUDA_TRUE, tbCudaAddByGpu(&a[0], &b[0], &c[0], c.size()));
        ASSERT_EQ(9, c.size());
        ASSERT_EQ(101, c[0]);
        ASSERT_EQ(202, c[1]);
        ASSERT_EQ(303, c[2]);
        ASSERT_EQ(404, c[3]);
        ASSERT_EQ(505, c[4]);
        ASSERT_EQ(606, c[5]);
        ASSERT_EQ(707, c[6]);
        ASSERT_EQ(808, c[7]);
        ASSERT_EQ(909, c[8]);
    }
}

