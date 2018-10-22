/**
 * @file   BagExTest.cpp
 * @brief  BagEx class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/container/BagEx.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(BagExTest, Default)
{
    BagEx<int> obj1;
    BagEx<int> obj2;

    obj1.resize(10, 10);
    obj2.resize(50);

    BagEx<int> const obj3 = obj2;

    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_TRUE(static_cast<bool>(obj2));
    ASSERT_TRUE(static_cast<bool>(obj3));

    ASSERT_NE(nullptr, obj1.get());
    ASSERT_NE(nullptr, obj2.get());
    ASSERT_NE(nullptr, obj3.get());

    *obj1;
    *obj3;
    ASSERT_NE(0, obj1->elem_size());
    ASSERT_NE(0, obj3->elem_size());

    obj1.bag();
    obj3.bag();
    obj1.user();
    obj3.user();
    obj1.castUser<void>();
    obj3.castUser<void>();

    ASSERT_NE(nullptr, obj1.data());
    ASSERT_NE(nullptr, obj2.data());
    ASSERT_NE(nullptr, obj3.data());

    ASSERT_EQ(sizeof(int), obj1.elem_size());
    ASSERT_EQ(sizeof(int), obj2.elem_size());
    ASSERT_EQ(sizeof(int), obj3.elem_size());

    ASSERT_NE(0, obj1.max_size());
    ASSERT_NE(0, obj2.max_size());
    ASSERT_NE(0, obj3.max_size());

    ASSERT_EQ(10*10, obj1.size());
    ASSERT_EQ(50, obj2.size());
    ASSERT_EQ(50, obj3.size());

    ASSERT_EQ(10, obj1.size(0));
    ASSERT_EQ(50, obj2.size(0));
    ASSERT_EQ(50, obj3.size(0));

    ASSERT_FALSE(obj1.empty());
    ASSERT_FALSE(obj2.empty());
    ASSERT_FALSE(obj3.empty());

    obj1.at(0) = 20;
    obj2.at(0) = 30;

    ASSERT_EQ(20, obj1[0]);
    ASSERT_EQ(30, obj2[0]);
    ASSERT_EQ(30, obj3[0]);

    ASSERT_EQ(20, obj1.index(0));
    ASSERT_EQ(30, obj2.index(0));
    ASSERT_EQ(30, obj3.index(0));

    obj1.clear();
    obj2.clear();

    ASSERT_EQ(0, obj1.size());
    ASSERT_EQ(0, obj2.size());
    ASSERT_EQ(0, obj3.size());

    ASSERT_TRUE(obj1.empty());
    ASSERT_TRUE(obj2.empty());
    ASSERT_TRUE(obj3.empty());
}

