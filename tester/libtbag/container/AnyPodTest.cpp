/**
 * @file   AnyPodTest.cpp
 * @brief  AnyPod class tester.
 * @author zer0
 * @date   2018-01-16
 */

#include <gtest/gtest.h>
#include <libtbag/container/AnyPod.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(AnyPodTest, Default)
{
    AnyPod pod1;
    AnyPod pod2(100);
    AnyPod pod3(pod2);
    ASSERT_EQ(pod2, pod3);
    ASSERT_EQ(100, pod2.data.i);
    std::cout << "AnyPod.UnionData size: " << pod2.size() << std::endl;
}

