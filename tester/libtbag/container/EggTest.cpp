/**
 * @file   EggTest.cpp
 * @brief  Egg class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/container/Egg.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(EggTest, Default)
{
    Egg obj1;
    Egg obj2;

    ASSERT_FALSE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));
    ASSERT_EQ(0, obj1.size());
    ASSERT_EQ(0, obj2.size());
    ASSERT_TRUE(obj1.empty());
    ASSERT_TRUE(obj2.empty());

    ASSERT_EQ(Err::E_NREADY, obj1.resize(10));
    ASSERT_EQ(Err::E_NREADY, obj2.resize(10, 20));

    ASSERT_EQ(Err::E_SUCCESS, obj1.create(EggTypeTable::ETT_INT32));
    ASSERT_EQ(Err::E_SUCCESS, obj2.create<int32_t>());
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

    auto obj1_bag = obj1.bag<int32_t>();
    auto obj2_bag = obj2.bag<int32_t>();

    ASSERT_TRUE(static_cast<bool>(obj1_bag));
    ASSERT_TRUE(static_cast<bool>(obj2_bag));

    ASSERT_EQ(10   , obj1_bag->size());
    ASSERT_EQ(10   , obj1_bag->size(0));
    ASSERT_EQ(10*20, obj2_bag->size());
    ASSERT_EQ(10   , obj2_bag->size(0));
    ASSERT_EQ(20   , obj2_bag->size(1));

    Egg obj3 = std::move(obj2);
    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));
    ASSERT_TRUE(static_cast<bool>(obj3));

    std::cout << "Obj1: " << obj1 << std::endl;
    std::cout << "Obj2: " << obj2 << std::endl;
    std::cout << "Obj3: " << obj3 << std::endl;

    Egg obj4("EggTest.Default");
    Egg obj5(std::vector<int32_t>{0, 1, 2, 3, 4});
    std::cout << "Obj4: " << obj4 << std::endl;
    std::cout << "Obj5: " << obj5 << std::endl;

    obj4 = "Update Text";
    obj5 = std::vector<float>{5, 6, 7, 8, 9};
    std::cout << "Update Obj4: " << obj4 << std::endl;
    std::cout << "Update Obj5: " << obj5 << std::endl;

    Egg obj6 = obj5;
    ASSERT_EQ(obj6, obj5);
}

TEST(EggTest, InitializerList)
{
    Egg obj = { 0, 1, 2, 3, 4 };
    ASSERT_EQ(EggTypeTable::ETT_INT32, obj.getType());
    ASSERT_EQ(5, obj.size());
    ASSERT_EQ(0, obj.at<int32_t>(0));
    ASSERT_EQ(1, obj.at<int32_t>(1));
    ASSERT_EQ(2, obj.at<int32_t>(2));
    ASSERT_EQ(3, obj.at<int32_t>(3));
    ASSERT_EQ(4, obj.at<int32_t>(4));
}

TEST(EggTest, Offset)
{
    Egg bag;
    bag.resize<int32_t>(2, 3, 4);
    ASSERT_EQ(EggTypeTable::ETT_INT32, bag.getType());
    ASSERT_EQ(2*3*4, bag.size());
    ASSERT_EQ(2, bag.size(0));
    ASSERT_EQ(3, bag.size(1));
    ASSERT_EQ(4, bag.size(2));

    for (int i = 0; i < bag.size(); ++i) {
        *(bag.cast<int32_t>() + i) = i;
    }

    ASSERT_EQ( 0, bag.at<int32_t>(0, 0, 0));
    ASSERT_EQ( 1, bag.at<int32_t>(0, 0, 1));
    ASSERT_EQ( 2, bag.at<int32_t>(0, 0, 2));
    ASSERT_EQ( 3, bag.at<int32_t>(0, 0, 3));

    ASSERT_EQ( 4, bag.at<int32_t>(0, 1, 0));
    ASSERT_EQ( 5, bag.at<int32_t>(0, 1, 1));
    ASSERT_EQ( 6, bag.at<int32_t>(0, 1, 2));
    ASSERT_EQ( 7, bag.at<int32_t>(0, 1, 3));

    ASSERT_EQ( 8, bag.at<int32_t>(0, 2, 0));
    ASSERT_EQ( 9, bag.at<int32_t>(0, 2, 1));
    ASSERT_EQ(10, bag.at<int32_t>(0, 2, 2));
    ASSERT_EQ(11, bag.at<int32_t>(0, 2, 3));

    ASSERT_EQ(12, bag.at<int32_t>(1, 0, 0));
    ASSERT_EQ(13, bag.at<int32_t>(1, 0, 1));
    ASSERT_EQ(14, bag.at<int32_t>(1, 0, 2));
    ASSERT_EQ(15, bag.at<int32_t>(1, 0, 3));

    ASSERT_EQ(16, bag.at<int32_t>(1, 1, 0));
    ASSERT_EQ(17, bag.at<int32_t>(1, 1, 1));
    ASSERT_EQ(18, bag.at<int32_t>(1, 1, 2));
    ASSERT_EQ(19, bag.at<int32_t>(1, 1, 3));

    ASSERT_EQ(20, bag.at<int32_t>(1, 2, 0));
    ASSERT_EQ(21, bag.at<int32_t>(1, 2, 1));
    ASSERT_EQ(22, bag.at<int32_t>(1, 2, 2));
    ASSERT_EQ(23, bag.at<int32_t>(1, 2, 3));
}

struct TestBagExUser : public Egg::User
{
    bool & call_destructor;
    int value;

    TestBagExUser(int v, bool & cd) : value(v), call_destructor(cd)
    { /* EMPTY. */ }

    virtual ~TestBagExUser()
    {
        call_destructor = true;
    }
};

TEST(EggTest, User)
{
    int const TEST_VALUE = 10;
    bool call_destructor = false;

    Egg test;
    test.createUser<TestBagExUser>(TEST_VALUE, call_destructor);
    auto shared_user = test.user<TestBagExUser>();
    ASSERT_EQ(TEST_VALUE, shared_user->value);
    shared_user.reset();
    test.clear();
    ASSERT_TRUE(call_destructor);
}

TEST(EggTest, StringConvert)
{
    std::string const TEST_STRING = "EggTest.StringConvert";
    Egg const TEST_BAG(TEST_STRING);
    ASSERT_TRUE(TEST_BAG.exists());
    ASSERT_FALSE(TEST_BAG.empty());
    ASSERT_EQ(TEST_STRING.size(), TEST_BAG.size());
    ASSERT_EQ(1, TEST_BAG.dims());

    ASSERT_EQ(TEST_STRING, TEST_BAG.toString());
}

TEST(EggTest, VectorConvert)
{
    std::vector<int> const TEST_VECTOR = {10, 20, 30};
    Egg const TEST_BAG(TEST_VECTOR);
    ASSERT_TRUE(TEST_BAG.exists());
    ASSERT_FALSE(TEST_BAG.empty());
    ASSERT_EQ(TEST_VECTOR.size(), TEST_BAG.size());
    ASSERT_EQ(1, TEST_BAG.dims());

    auto const RESULT_VECTOR = TEST_BAG.toVector<int>();
    ASSERT_EQ(TEST_VECTOR.size(), RESULT_VECTOR.size());
    ASSERT_EQ(TEST_VECTOR[0], RESULT_VECTOR[0]);
    ASSERT_EQ(TEST_VECTOR[1], RESULT_VECTOR[1]);
    ASSERT_EQ(TEST_VECTOR[2], RESULT_VECTOR[2]);
}

TEST(EggTest, HexConvert)
{
    std::string const TEST_STRING = "0123456789";
    Egg const TEST_BAG(TEST_STRING);
    ASSERT_TRUE(TEST_BAG.exists());

    auto const HEX = TEST_BAG.toHexString();
    auto const HEX_BOX = TEST_BAG.toHexBoxString();
    ASSERT_FALSE(HEX.empty());
    ASSERT_FALSE(HEX_BOX.empty());

    std::cout << "Hex: " << HEX << std::endl;
    std::cout << "HexBox: " << HEX_BOX << std::endl;
}

