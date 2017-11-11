/**
 * @file   Sol2Test.cpp
 * @brief  Sol2 library tester.
 * @author zer0
 * @date   2017-11-11
 */

#include <gtest/gtest.h>
#include <libtbag/3rd/sol2/sol.hpp>

TEST(Sol2Test, Default)
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

TEST(Sol2Test, LuaClass)
{
    sol::state lua;
    lua.new_usertype<Sol2TestClass>("Sol2TestClass", "var", &Sol2TestClass::var);
    lua.script("temp = Sol2TestClass.new() \n temp.var = 100");
    ASSERT_EQ(100, lua.get<Sol2TestClass>("temp").var);
}

