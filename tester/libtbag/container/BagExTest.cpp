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
    BagEx obj1;
    BagEx obj2;

    ASSERT_FALSE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));
    ASSERT_EQ(0, obj1.size());
    ASSERT_EQ(0, obj2.size());
    ASSERT_TRUE(obj1.empty());
    ASSERT_TRUE(obj2.empty());

    ASSERT_EQ(Err::E_NREADY, obj1.resize(10));
    ASSERT_EQ(Err::E_NREADY, obj2.resize(10, 20));

    ASSERT_EQ(Err::E_SUCCESS, obj1.create(libtbag::type::TypeTable::TT_INT));
    ASSERT_EQ(Err::E_SUCCESS, obj2.create<int>());
    ASSERT_EQ(obj1.getType(), obj2.getType());

    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_TRUE(static_cast<bool>(obj2));
    ASSERT_EQ(0, obj1.size());
    ASSERT_EQ(0, obj2.size());
    ASSERT_TRUE(obj1.empty());
    ASSERT_TRUE(obj2.empty());

    ASSERT_EQ(Err::E_SUCCESS, obj1.resize(10));
    ASSERT_EQ(Err::E_SUCCESS, obj2.resize(10, 20));
    ASSERT_EQ(10, obj1.size());
    ASSERT_EQ(10*20, obj2.size());
    ASSERT_FALSE(obj1.empty());
    ASSERT_FALSE(obj2.empty());

    auto obj1_bag = obj1.bag<int>();
    auto obj2_bag = obj2.bag<int>();

    ASSERT_TRUE(static_cast<bool>(obj1_bag));
    ASSERT_TRUE(static_cast<bool>(obj2_bag));

    ASSERT_EQ(10   , obj1_bag->size());
    ASSERT_EQ(10   , obj1_bag->size(0));
    ASSERT_EQ(10*20, obj2_bag->size());
    ASSERT_EQ(10   , obj2_bag->size(0));
    ASSERT_EQ(20   , obj2_bag->size(1));

    BagEx obj3 = std::move(obj2);
    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));
    ASSERT_TRUE(static_cast<bool>(obj3));

    std::cout << "Obj1: " << obj1.toString() << std::endl;
    std::cout << "Obj2: " << obj2.toString() << std::endl;
    std::cout << "Obj3: " << obj3.toString() << std::endl;
}

struct TestBagExUser : public BagEx::User
{
    int value;

    TestBagExUser(int v = 0) : value(v)
    { /* EMPTY. */ }

    virtual ~TestBagExUser()
    { /* EMPTY. */ }
};

TEST(BagExTest, User)
{
    int const TEST_VALUE = 10;

    BagEx test;
    test.createUser(new TestBagExUser(TEST_VALUE));
    auto shared_user = test.user<TestBagExUser>();
    ASSERT_EQ(TEST_VALUE, shared_user->value);
}

