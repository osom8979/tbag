/**
 * @file   DbgPairTest.cpp
 * @brief  DbgPair class tester.
 * @author zer0
 * @date   2020-05-20
 */

#include <gtest/gtest.h>
#include <libtbag/DbgPair.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;

TEST(DbgPairTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(DbgPair<int>, obj1);
    TEST_DEFAULT_ASSIGNMENT(DbgPair<int>, obj2);

    TEST_DEFAULT_CONSTRUCTOR(DbgNull, obj3);
    TEST_DEFAULT_ASSIGNMENT(DbgNull, obj4);

    TEST_DEFAULT_CONSTRUCTOR(DbgString, obj5);
    TEST_DEFAULT_ASSIGNMENT(DbgString, obj6);
}

TEST(DbgPairTest, StringCast)
{
    DbgPair<int> obj(E_ILLARGS);
    ASSERT_STREQ("E_ILLARGS", obj.name());
}

TEST(DbgPairTest, StreamCast)
{
    DbgPair<int> obj(E_ILLARGS);
    std::stringstream ss;
    ss << obj;
    ASSERT_STREQ("E_ILLARGS", ss.str().c_str());
}

TEST(DbgPairTest, BooleanCast)
{
    DbgPair<int> obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    DbgPair<int> obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

TEST(DbgPairTest, Macro)
{
    DbgPair<int> obj1 = TBAG_DBG_FMT(E_WARNING, "{}.{}", 10, "msg");
    ASSERT_EQ(E_WARNING, obj1);
    ASSERT_FALSE(obj1.file.empty());
    ASSERT_NE(0, obj1.line);
    ASSERT_STREQ("10.msg", obj1.msg.c_str());

    DbgPair<int> obj2 = TBAG_DBG_OK(100);
    ASSERT_EQ(E_SUCCESS, obj2);
    ASSERT_FALSE(obj2.file.empty());
    ASSERT_NE(0, obj2.line);
    ASSERT_TRUE(obj2.msg.empty());
    ASSERT_EQ(100, obj2.value);
}

