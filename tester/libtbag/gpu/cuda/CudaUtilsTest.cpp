/**
 * @file   CudaUtilsTest.cpp
 * @brief  CudaUtils class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/cuda/CudaUtils.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::cuda;

TEST(CudaUtilsTest, AddByCpu)
{
    std::vector<int> a = {  1,   2,   3,   4};
    std::vector<int> b = {100, 200, 300, 400};
    auto result = libtbag::gpu::cuda::add(a, b, BrewType::BT_CPU);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ(101, result[0]);
    ASSERT_EQ(202, result[1]);
    ASSERT_EQ(303, result[2]);
    ASSERT_EQ(404, result[3]);
}

TEST(CudaUtilsTest, AddByGpu)
{
    if (isCudaSupport()) {
        std::vector<int> a = {  1,   2,   3,   4,   5,   6,   7,   8,   9};
        std::vector<int> b = {100, 200, 300, 400, 500, 600, 700, 800, 900};
        auto result = libtbag::gpu::cuda::add(a, b, BrewType::BT_GPU);
        ASSERT_EQ(9, result.size());
        ASSERT_EQ(101, result[0]);
        ASSERT_EQ(202, result[1]);
        ASSERT_EQ(303, result[2]);
        ASSERT_EQ(404, result[3]);
        ASSERT_EQ(505, result[4]);
        ASSERT_EQ(606, result[5]);
        ASSERT_EQ(707, result[6]);
        ASSERT_EQ(808, result[7]);
        ASSERT_EQ(909, result[8]);
    }
}

