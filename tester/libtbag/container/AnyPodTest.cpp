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
    ASSERT_FALSE(pod1.exists());

    AnyPod pod2(100);
    AnyPod pod3(pod2);
    ASSERT_EQ(pod2, pod3);
    ASSERT_EQ(100, pod2.data().i);
    ASSERT_EQ(100, pod2.cast<int>());
    ASSERT_LT(0, AnyPod::size());

    AnyPod pod4 = std::move(AnyPod(200));
    pod1 = pod4;
    ASSERT_EQ(200, pod1);
    ASSERT_EQ(200, pod4);

    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod4.type());
    ASSERT_FALSE(pod4.toString().empty());

    AnyPod pod5 = 1.5;
    ASSERT_EQ(AnyPod::TypeTable::TT_DOUBLE, pod5.type());
    ASSERT_FALSE(pod5.toString().empty());
}

TEST(AnyPodTest, Cast)
{
    AnyPod pod1 = 101.08;
    ASSERT_EQ(AnyPod::TypeTable::TT_DOUBLE, pod1.type());
    ASSERT_FLOAT_EQ(101.08, pod1.cast<float>());
    ASSERT_EQ(101, pod1.cast<int>());

    AnyPod pod2 = 102;
    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod2.type());
    ASSERT_FLOAT_EQ(102.0, pod2.cast<float>());
    ASSERT_EQ(102, pod2.cast<int>());
}

TEST(AnyPodTest, Pointer)
{
    int i = 0;
    AnyPod pod1 = (void*)&i;
    ASSERT_EQ(&i, pod1.pointer());
}

TEST(AnyPodTest, Compare)
{
    AnyPod pod1(100);
    AnyPod pod2(100);

    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod1.type());
    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod2.type());

    ASSERT_TRUE(pod1 <= pod2);
    ASSERT_TRUE(pod1 >= pod2);
    ASSERT_TRUE(pod1 == pod2);

    AnyPod pod3 = 200;
    AnyPod pod4 = 100.0f;

    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod3.type());
    ASSERT_EQ(AnyPod::TypeTable::TT_FLOAT, pod4.type());

    ASSERT_FALSE(pod3 == pod4);
    ASSERT_TRUE (pod3 != pod4);

    AnyPod pod5 = 500;
    AnyPod pod6 = 600u;

    ASSERT_EQ(AnyPod::TypeTable::TT_INT, pod5.type());
    ASSERT_EQ(AnyPod::TypeTable::TT_UINT, pod6.type());

    ASSERT_FALSE(pod5 == pod6);
    ASSERT_TRUE (pod5 != pod6);
    ASSERT_FALSE(pod5  > pod6);
    ASSERT_TRUE (pod5  < pod6);
    ASSERT_FALSE(pod5 >= pod6);
    ASSERT_TRUE (pod5 <= pod6);

    ASSERT_FALSE(pod5 == 600u);
    ASSERT_TRUE (pod5 != 600u);
    ASSERT_FALSE(pod5  > 600u);
    ASSERT_TRUE (pod5  < 600u);
    ASSERT_FALSE(pod5 >= 600u);
    ASSERT_TRUE (pod5 <= 600u);

    ASSERT_FALSE(pod5 == 600.0f);
    ASSERT_TRUE (pod5 != 600.0f);
    ASSERT_FALSE(pod5  > 600.0f);
    ASSERT_TRUE (pod5  < 600.0f);
    ASSERT_FALSE(pod5 >= 600.0f);
    ASSERT_TRUE (pod5 <= 600.0f);

    ASSERT_FALSE(-10 == pod6);
    ASSERT_TRUE (-10 != pod6);
    ASSERT_FALSE(-10  > pod6);
    ASSERT_TRUE (-10  < pod6);
    ASSERT_FALSE(-10 >= pod6);
    ASSERT_TRUE (-10 <= pod6);

    ASSERT_FALSE(500u == pod6);
    ASSERT_TRUE (500u != pod6);
    ASSERT_FALSE(500u  > pod6);
    ASSERT_TRUE (500u  < pod6);
    ASSERT_FALSE(500u >= pod6);
    ASSERT_TRUE (500u <= pod6);

    ASSERT_FALSE(500.0f == pod6);
    ASSERT_TRUE (500.0f != pod6);
    ASSERT_FALSE(500.0f  > pod6);
    ASSERT_TRUE (500.0f  < pod6);
    ASSERT_FALSE(500.0f >= pod6);
    ASSERT_TRUE (500.0f <= pod6);
}

