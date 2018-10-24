/**
 * @file   ModelNetTest.cpp
 * @brief  ModelNet class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/graph/ModelNet.hpp>

using namespace libtbag;
using namespace libtbag::graph;

TEST(ModelNetTest, Constructor)
{
    ModelNet obj1;
    ModelNet obj2(nullptr);

    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelNet obj3 = obj2;
    ASSERT_FALSE(static_cast<bool>(obj3));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelNet obj4;
    ASSERT_TRUE(static_cast<bool>(obj4));
    ModelNet obj5 = std::move(obj4);
    ASSERT_FALSE(static_cast<bool>(obj4));
    ASSERT_TRUE(static_cast<bool>(obj5));
}

TEST(ModelNetTest, Default)
{
    ModelNet net;
    std::cout << net.toString() << std::endl;
}

