/**
 * @file   Sol2BypassTest.cpp
 * @brief  Sol2Bypass class tester.
 * @author zer0
 * @date   2018-10-30
 */

#include <gtest/gtest.h>
#include <libtbag/script/Sol2Bypass.hpp>

using namespace libtbag;
using namespace libtbag::script;

TEST(Sol2BypassTest, Default)
{
    sol::state lua;
    int value = 0;
    lua.set_function("test_lua_function", [&]{ value = 100; });
    lua.script("test_lua_function()");
    ASSERT_EQ(100, value);
}

struct Sol2TestClass
{
    int var = 0;
};

TEST(Sol2BypassTest, LuaClass)
{
    sol::state lua;
    lua.new_usertype<Sol2TestClass>("Sol2TestClass", "var", &Sol2TestClass::var);
    lua.script("temp = Sol2TestClass.new() \n temp.var = 100");
    ASSERT_EQ(100, lua.get<Sol2TestClass>("temp").var);
}

