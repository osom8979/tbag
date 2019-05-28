/**
 * @file   SceneTest.cpp
 * @brief  Scene class tester.
 * @author zer0
 * @date   2019-05-29
 */

#include <gtest/gtest.h>
#include <libtbag/scene/Scene.hpp>

using namespace libtbag;
using namespace libtbag::scene;

TEST(SceneTest, Default)
{
    Scene object;
    ASSERT_TRUE(true);
}

