/**
 * @file   RayLuaBindingTest.cpp
 * @brief  RayLuaBinding class tester.
 * @author zer0
 * @date   2019-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/ray/RayBypass.hpp>
#include <libtbag/ray/RayLuaBinding.hpp>
#include <libtbag/script/Sol2Bypass.hpp>

using namespace libtbag;
using namespace libtbag::ray;

TEST(RayLuaBindingTest, Default)
{
    sol::state lua;
    ASSERT_TRUE(bindRayForLua(lua.lua_state()));
    char const * const TEST_LUA_CODE = R"(
        v1 = tbag.ray.Vector2.new()
        v1.x = 10
        v1.y = 20

        t1 = tbag.ray.Texture2D.new()
        t1.id = 30
        t1.width = 40
        t1.height = 50
        t1.mipmaps = 60
        t1.format = 70

        r1 = tbag.ray.RenderTexture2D.new()
        r1.id = 80
        r1.texture = t1
        r1.depth.id = 100
        r1.depth.width = 101
        r1.depth.height = 102
        r1.depth.mipmaps = 103
        r1.depth.format = 104
        r1.depthTexture = true
)";

    lua.script(TEST_LUA_CODE);
    ASSERT_EQ(10, (int)lua["v1"]["x"]);
    ASSERT_EQ(20, (int)lua["v1"]["y"]);

    ASSERT_EQ(30, (int)lua["t1"]["id"]);
    ASSERT_EQ(40, (int)lua["t1"]["width"]);
    ASSERT_EQ(50, (int)lua["t1"]["height"]);
    ASSERT_EQ(60, (int)lua["t1"]["mipmaps"]);
    ASSERT_EQ(70, (int)lua["t1"]["format"]);

    ASSERT_EQ(80, (int)lua["r1"]["id"]);
    Texture2D texture = lua["r1"]["texture"];
    Texture2D depth = lua["r1"]["depth"];
    ASSERT_EQ(30, texture.id);
    ASSERT_EQ(40, texture.width);
    ASSERT_EQ(50, texture.height);
    ASSERT_EQ(60, texture.mipmaps);
    ASSERT_EQ(70, texture.format);
    ASSERT_EQ(100, depth.id);
    ASSERT_EQ(101, depth.width);
    ASSERT_EQ(102, depth.height);
    ASSERT_EQ(103, depth.mipmaps);
    ASSERT_EQ(104, depth.format);
    ASSERT_EQ(true, (bool)lua["r1"]["depthTexture"]);
}

