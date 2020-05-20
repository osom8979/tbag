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
    ErrPair<int> obj1(E_ILLARGS, 10);
    std::stringstream ss1;
    ss1 << obj1;
    ASSERT_STREQ("E_ILLARGS", ss1.str().c_str());
}

TEST(ErrPairTest, ErrMsg)
{
    ErrMsg obj = TBAG_ERR_FMT(E_ILLARGS, "{}.{}", 10, "msg");
    std::stringstream ss;
    ss << obj;
    ASSERT_STREQ("E_ILLARGS('10.msg')", ss.str().c_str());
}

TEST(ErrPairTest, BooleanCast)
{
    ErrPair<int> obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    ErrPair<int> obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

