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

    ErrMsg obj2(E_ILLARGS, "msg");
    std::stringstream ss2;
    ss2 << obj2;
    ASSERT_STREQ("E_ILLARGS('msg')", ss2.str().c_str());
}

TEST(ErrPairTest, BooleanCast)
{
    ErrPair<int> obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    ErrPair<int> obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

TEST(ErrPairTest, Macro)
{
    auto const result = TBAG_ERR_FMT(E_SUCCESS, "{}.{}", 10, "msg");
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_STREQ("10.msg", result.value.c_str());
}

