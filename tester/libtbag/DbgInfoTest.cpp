/**
 * @file   DbgInfoTest.cpp
 * @brief  DbgInfo class tester.
 * @author zer0
 * @date   2020-05-20
 */

#include <gtest/gtest.h>
#include <libtbag/DbgInfo.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;

TEST(DbgInfoTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(DbgInfo, obj1);
    TEST_DEFAULT_ASSIGNMENT(DbgInfo, obj2);
}

TEST(DbgInfoTest, StringCast)
{
    DbgInfo obj(E_ILLARGS);
    ASSERT_STREQ("E_ILLARGS", obj.name());
}

TEST(DbgInfoTest, StreamCast)
{
    DbgInfo obj(E_ILLARGS);
    std::stringstream ss;
    ss << obj;
    ASSERT_STREQ("E_ILLARGS", ss.str().c_str());
}

TEST(DbgInfoTest, BooleanCast)
{
    DbgInfo obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    DbgInfo obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

TEST(DbgInfoTest, Macro)
{
    auto const result = TBAG_DBG_FMT(E_SUCCESS, "{}.{}", 10, "msg");
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_FALSE(result.file.empty());
    ASSERT_NE(0, result.line);
    ASSERT_STREQ("10.msg", result.msg.c_str());
}

