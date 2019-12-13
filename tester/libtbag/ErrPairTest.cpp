/**
 * @file   ErrPairTest.cpp
 * @brief  ErrPair class tester.
 * @author zer0
 * @date   2019-09-11
 */

#include <gtest/gtest.h>
#include <libtbag/ErrPair.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <string>
#include <sstream>

using namespace libtbag;

TEST(ErrPairTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(ErrPair<int>, obj1);
    TEST_DEFAULT_ASSIGNMENT(ErrPair<int>, obj2);
}

TEST(ErrPairTest, StringCast)
{
    ErrPair<int> obj(E_ILLARGS);
    ASSERT_STREQ("E_ILLARGS", obj.name());
}

TEST(ErrPairTest, StreamCast)
{
    ErrPair<int> obj(E_ILLARGS);
    std::stringstream ss;
    ss << obj;
    ASSERT_STREQ("E_ILLARGS", ss.str().c_str());
}

TEST(ErrPairTest, BooleanCast)
{
    ErrPair<int> obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    ErrPair<int> obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

TEST(ErrPairTest, ErrMsg)
{
    ErrMsg err;
    ASSERT_EQ(nullptr, err.value);
}

TEST(ErrPairTest, MakeErrMsg)
{
    ErrMsgPair<int> err1 = TBAG_MAKE_ERR_MSG("test1", E_UNKNOWN, 100);
    std::cout << "err1 msg: " << err1.msg << std::endl;
    ASSERT_FALSE(err1.msg.empty());
    ASSERT_EQ(E_UNKNOWN, err1.code);
    ASSERT_EQ(100, err1.value);

    ErrMsgPair<std::string> err2 = TBAG_MAKE_ERR_MSG("test2", E_INVALID_ID, "value");
    std::cout << "err2 msg: " << err2.msg << std::endl;
    ASSERT_FALSE(err2.msg.empty());
    ASSERT_EQ(E_INVALID_ID, err2.code);
    ASSERT_STREQ("value", err2.value.c_str());

    ErrMsgPair<double> err3 = TBAG_MAKE_ERR_MSG("test3", E_SUCCESS);
    std::cout << "err3 msg: " << err3.msg << std::endl;
    ASSERT_FALSE(err3.msg.empty());
    ASSERT_EQ(E_SUCCESS, err3.code);

    ErrMsg err4 = TBAG_MAKE_ERR_MSG(E_ALREADY);
    std::cout << "err4 msg: " << err4.msg << std::endl;
    ASSERT_FALSE(err4.msg.empty());
    ASSERT_EQ(E_ALREADY, err4.code);
}

