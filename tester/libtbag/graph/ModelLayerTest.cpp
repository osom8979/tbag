/**
 * @file   ModelLayerTest.cpp
 * @brief  ModelLayer class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/graph/ModelLayer.hpp>

using namespace libtbag;
using namespace libtbag::graph;

TEST(ModelLayerTest, Constructor)
{
    ModelLayer obj1;
    ModelLayer obj2(nullptr);

    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelLayer obj3 = obj2;
    ASSERT_FALSE(static_cast<bool>(obj3));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelLayer obj4;
    ASSERT_TRUE(static_cast<bool>(obj4));
    ModelLayer obj5 = std::move(obj4);
    ASSERT_FALSE(static_cast<bool>(obj4));
    ASSERT_TRUE(static_cast<bool>(obj5));
}

