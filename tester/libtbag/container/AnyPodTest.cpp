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
    ASSERT_EQ(100, pod2.data().i);
    ASSERT_EQ(100, pod2.cast<int>());

    AnyPod pod4(AnyPod(100));
    AnyPod pod5 = 200;
    AnyPod pod6 = pod5;

    ASSERT_FALSE(pod4 == pod5);
    ASSERT_TRUE (pod4 != pod5);
    ASSERT_TRUE (pod5 == pod6);

    ASSERT_LT(0, AnyPod::size());
}

