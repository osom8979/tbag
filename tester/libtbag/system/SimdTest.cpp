/**
 * @file   SimdTest.cpp
 * @brief  Simd class tester.
 * @author zer0
 * @date   2018-01-07
 */

#include <gtest/gtest.h>
#include <libtbag/system/Simd.hpp>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::system;

TEST(SimdTest, Default)
{
    SimdArch arch;
    std::cout << "SIMD INFO: " << arch.toString() << "(" << arch.toBinaryString() << ")" << std::endl;
}

TEST(SimdTest, ToBinaryString)
{
    SimdArch full(SimdArch::FULL_ARCHS);
    auto const full_str = full.toBinaryString();

    SimdArch arch;
    auto const arch_str = arch.toBinaryString();

    ASSERT_EQ(full_str.size(), arch_str.size());
    ASSERT_EQ(full_str.size(), std::count(full_str.begin(), full_str.end(), '1'));
}

