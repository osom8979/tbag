/**
 * @file   LuaExtension.cpp
 * @brief  LuaExtension class implementation.
 * @author zer0
 * @date   2019-05-13
 */

#include <libtbag/script/LuaExtension.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

//Pairs getRegistryPairs(lua_State * L)
//{
//    Pairs result;
//    ::lua_pushnil(L);  // First key.
//    while (::lua_next(L, LUA_REGISTRYINDEX) != 0) {
//        // Key index: -2
//        // Value index: -1
//        // printf("%s - %s\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
//        // lua_pop(L, 1); // removes 'value'; keeps 'key' for next iteration
//    }
//    return result;
//}

int luaL_unsupport(lua_State * L)
{
    return luaL_error(L, "Unsupported operation error");
}

void luadebug_printstack(lua_State * L)
{
    int const TOP = lua_gettop(L);
    if (TOP == 0) {
        std::cout << "||" << std::endl;
    } else {
        std::cout << '|';
    }

    for (int i = 1; i <= TOP; ++i) {
        auto const TYPE = lua_type(L, i);
        switch (TYPE) {
        case LUA_TNUMBER:
            std::cout << lua_tonumber(L, i);
            break;
        case LUA_TBOOLEAN:
            std::cout << (lua_toboolean(L, i) == 1 ? "true" : "false");
            break;
        case LUA_TSTRING:
            std::cout << '"' << lua_tolstring(L, i, nullptr) << '"';
            break;
        case LUA_TTABLE:
            if (luaL_getmetafield(L, i, "__name")) {
                std::cout << lua_tolstring(L, -1, nullptr);
                lua_settop(L, -2);
            } else {
                std::cout << "table";
            }
            break;
        default:
            std::cout << lua_typename(L, TYPE);
            break;
        }
        std::cout << '|';
    }
    std::cout << std::endl;
}

void luaL_register_metatable(lua_State * L, char const * name, luaL_Reg const * l)
{
    luaL_newmetatable(L, name);
    {
        luaL_register(L, nullptr, l);

        lua_pushliteral(L, "__index");
        lua_pushvalue(L, -2); // Duplicate metatable.
        lua_rawset(L, -3);
    }
    lua_pop(L, 1);
}

std::vector<lua_Integer> luaL_checkinteger_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<lua_Integer> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkinteger(L, -1);
    }
    return result;
}

std::vector<lua_Number> luaL_checknumber_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<lua_Number> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkinteger(L, -1);
    }
    return result;
}

std::vector<std::string> luaL_checkstring_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<std::string> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkstring(L, -1);
    }
    return result;
}

# /**************/
# /* PosWrapper */
# /**************/

TBAG_LUA_USERDATA_DEFAULT_REG(BooleanWrapper, BOOLEANWRAPPER, booleanwrapper)
TBAG_LUA_USERDATA_DEFAULT_REG(IntegerWrapper, INTEGERWRAPPER, integerwrapper)
TBAG_LUA_USERDATA_DEFAULT_REG(FloatWrapper, FLOATWRAPPER, floatwrapper)
TBAG_LUA_USERDATA_DEFAULT_REG(DoubleWrapper, DOUBLEWRAPPER, doublewrapper)

BooleanWrapper * luaL_pushbooleanwrapper(lua_State * L, BooleanWrapper const * src)
{ return _luaL_pushbooleanwrapper(L, src); }
BooleanWrapper * luaL_checkbooleanwrapper(lua_State * L, int num_arg)
{ return _luaL_checkbooleanwrapper(L, num_arg); }

IntegerWrapper * luaL_pushintegerwrapper(lua_State * L, IntegerWrapper const * src)
{ return _luaL_pushintegerwrapper(L, src); }
IntegerWrapper * luaL_checkintegerwrapper(lua_State * L, int num_arg)
{ return _luaL_checkintegerwrapper(L, num_arg); }

FloatWrapper * luaL_pushfloatwrapper(lua_State * L, FloatWrapper const * src)
{ return _luaL_pushfloatwrapper(L, src); }
FloatWrapper * luaL_checkfloatwrapper(lua_State * L, int num_arg)
{ return _luaL_checkfloatwrapper(L, num_arg); }

DoubleWrapper * luaL_pushdoublewrapper(lua_State * L, DoubleWrapper const * src)
{ return _luaL_pushdoublewrapper(L, src); }
DoubleWrapper * luaL_checkdoublewrapper(lua_State * L, int num_arg)
{ return _luaL_checkdoublewrapper(L, num_arg); }

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

