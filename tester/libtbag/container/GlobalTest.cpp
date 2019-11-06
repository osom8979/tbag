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

struct TestObject
{
    int data;

    TestObject(int value) : data(value)
    { /* EMPTY. */ }
};

TEST(GlobalTest, Default)
{
    Global::createInstance();
    Global * global = Global::getInstance();

    ASSERT_NE(nullptr, global);
    ASSERT_TRUE(global->empty());
    ASSERT_EQ(0, global->size());

    char const * KEY1 = "1";
    char const * KEY2 = "2";
    auto shared1 = global->insertNewObject<TestObject>(KEY1, 10);
    auto shared2 = global->insertNewObject<TestObject>(KEY2, 20);

    ASSERT_EQ(2, global->size());
    ASSERT_FALSE(global->empty());
    ASSERT_GT(global->max_size(), global->size());

    auto shared1_1 = global->find<TestObject>(KEY1).lock();
    auto shared2_1 = global->find<TestObject>(KEY2).lock();

    ASSERT_EQ(10, shared1_1->data);
    ASSERT_EQ(20, shared2_1->data);

    ASSERT_TRUE(global->erase(KEY1));
    ASSERT_EQ(1, global->size());

    global->clear();
    ASSERT_TRUE(global->empty());
    ASSERT_EQ(0, global->size());

    Global::releaseInstance();
}

