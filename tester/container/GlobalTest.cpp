/**
 * @file   GlobalTest.cpp
 * @brief  Global class tester.
 * @author zer0
 * @date   2017-02-16
 */

#include <gtest/gtest.h>
#include <libtbag/container/Global.hpp>

using namespace libtbag;
using namespace libtbag::container;

struct TestObject1 : public Global::Object
{
    int data;

    TestObject1(int value) : data(value)
    { /* EMPTY. */ }

    virtual int type() override
    { return 1; }
};

struct TestObject2 : public Global::Object
{
    int data;

    TestObject2(int value) : data(value)
    { /* EMPTY. */ }

    virtual int type() override
    { return 2; }
};

TEST(GlobalTest, Default)
{
    Global::createInstance();

    Global * instance = Global::getInstance();
    ASSERT_NE(nullptr, instance);
    ASSERT_TRUE(instance->empty());
    ASSERT_EQ(0, instance->size());

    auto shared1 = instance->newObject<TestObject1>(10);
    auto shared2 = instance->newObject<TestObject2>(20);

    Global::Object & obj1 = *shared1;
    Global::Object & obj2 = *shared2;

    ASSERT_EQ(2, instance->size());
    ASSERT_FALSE(instance->empty());
    ASSERT_GT(instance->max_size(), instance->size());

    auto itr = instance->begin();
    auto end = instance->end();
    ASSERT_NE(itr, end);
    ++itr;
    ++itr;
    ASSERT_EQ(itr, end);

    auto shared1_1 = instance->find(obj1).lock();
    auto shared2_1 = instance->find(obj2).lock();

    ASSERT_EQ(1, shared1_1->type());
    ASSERT_EQ(2, shared2_1->type());

    auto shared1_2 = instance->find<TestObject1>(obj1).lock();
    auto shared2_2 = instance->find<TestObject2>(obj2).lock();

    ASSERT_EQ(10, shared1_2->data);
    ASSERT_EQ(20, shared2_2->data);

    ASSERT_TRUE(instance->erase(obj2));
    ASSERT_EQ(1, instance->size());

    instance->clear();
    ASSERT_TRUE(instance->empty());
    ASSERT_EQ(0, instance->size());

    Global::releaseInstance();
}

