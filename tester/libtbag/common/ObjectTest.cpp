/**
 * @file   ObjectTest.cpp
 * @brief  Object class tester.
 * @author zer0
 * @date   2016-07-06
 */

#include <gtest/gtest.h>
#include <libtbag/common/Object.hpp>

using namespace libtbag;
using namespace libtbag::common;

TEST(ObjectTest, Default)
{
    Object o1;
    Object o2;
    Object o3 = o1;
    Object o4(o1);
    Object o5(std::move(Object()));
    Object o6;
    Object o7;

    o6 = o1;
    o6 = Object();

    ASSERT_TRUE(o1 != o2);
    ASSERT_FALSE(o1 == o6);
    ASSERT_STREQ("Object", Object::getClassName());
}

