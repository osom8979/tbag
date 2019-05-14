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

int luaE_unsupport(lua_State * L)
{
    return luaL_error(L, "Unsupported operation error");
}

void luaE_register_metatable(lua_State * L, char const * name, luaL_Reg const * l)
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

void luaE_printstack(lua_State * L)
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

std::vector<lua_Integer> luaE_checkinteger_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<lua_Integer> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkinteger(L, -1);
    }
    return result;
}

std::vector<lua_Number> luaE_checknumber_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<lua_Number> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkinteger(L, -1);
    }
    return result;
}

std::vector<std::string> luaE_checkstring_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<std::string> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkstring(L, -1);
    }
    return result;
}

std::vector<luaL_Reg> mergeLuaRegister(luaL_Reg const * a, luaL_Reg const * b)
{
    std::vector<luaL_Reg> result;
    while (a != nullptr && a->name != nullptr && a->func != nullptr) {
        result.push_back(luaL_Reg{a->name, a->func});
        ++a;
    }
    while (b != nullptr && b->name != nullptr && b->func != nullptr) {
        result.push_back(luaL_Reg{b->name, b->func});
        ++b;
    }
    result.push_back(luaL_Reg{nullptr, nullptr});
    return result;
}

# /**************/
# /* PodWrapper */
# /**************/

static int _BooleanWrapper_get(lua_State * L)
{
    auto * wrapper = luaE_checkbooleanwrapper(L, 1);
    lua_pushboolean(L, wrapper->value);
    return 1;
}

static int _BooleanWrapper_set(lua_State * L)
{
    auto * wrapper = luaE_checkbooleanwrapper(L, 1);
    wrapper->value = luaL_checkinteger(L, 2);
    return 0;
}

static luaL_Reg const __lua_reg_BooleanWrapper[] = {
        { "get", _BooleanWrapper_get },
        { "set", _BooleanWrapper_set },
        { nullptr, nullptr }
};

static int _IntegerWrapper_get(lua_State * L)
{
    auto * wrapper = luaE_checkintegerwrapper(L, 1);
    lua_pushboolean(L, wrapper->value);
    return 1;
}

static int _IntegerWrapper_set(lua_State * L)
{
    auto * wrapper = luaE_checkintegerwrapper(L, 1);
    wrapper->value = luaL_checkinteger(L, 2);
    return 0;
}

static luaL_Reg const __lua_reg_IntegerWrapper[] = {
        { "get", _IntegerWrapper_get },
        { "set", _IntegerWrapper_set },
        { nullptr, nullptr }
};

static int _UnsignedWrapper_get(lua_State * L)
{
    auto * wrapper = luaE_checkunsignedwrapper(L, 1);
    lua_pushboolean(L, wrapper->value);
    return 1;
}

static int _UnsignedWrapper_set(lua_State * L)
{
    auto * wrapper = luaE_checkunsignedwrapper(L, 1);
    wrapper->value = luaL_checkinteger(L, 2);
    return 0;
}

static luaL_Reg const __lua_reg_UnsignedWrapper[] = {
        { "get", _UnsignedWrapper_get },
        { "set", _UnsignedWrapper_set },
        { nullptr, nullptr }
};

static int _FloatWrapper_get(lua_State * L)
{
    auto * wrapper = luaE_checkfloatwrapper(L, 1);
    lua_pushboolean(L, wrapper->value);
    return 1;
}

static int _FloatWrapper_set(lua_State * L)
{
    auto * wrapper = luaE_checkfloatwrapper(L, 1);
    wrapper->value = luaL_checknumber(L, 2);
    return 0;
}

static luaL_Reg const __lua_reg_FloatWrapper[] = {
        { "get", _FloatWrapper_get },
        { "set", _FloatWrapper_set },
        { nullptr, nullptr }
};

static int _DoubleWrapper_get(lua_State * L)
{
    auto * wrapper = luaE_checkdoublewrapper(L, 1);
    lua_pushboolean(L, wrapper->value);
    return 1;
}

static int _DoubleWrapper_set(lua_State * L)
{
    auto * wrapper = luaE_checkdoublewrapper(L, 1);
    wrapper->value = luaL_checknumber(L, 2);
    return 0;
}

static luaL_Reg const __lua_reg_DoubleWrapper[] = {
        { "get", _DoubleWrapper_get },
        { "set", _DoubleWrapper_set },
        { nullptr, nullptr }
};

// clang-format off
TBAG_LUA_USERDATA_IMPL(BooleanWrapper , BooleanWrapper , BOOLEANWRAPPER , booleanwrapper , __lua_reg_BooleanWrapper )
TBAG_LUA_USERDATA_IMPL(IntegerWrapper , IntegerWrapper , INTEGERWRAPPER , integerwrapper , __lua_reg_IntegerWrapper )
TBAG_LUA_USERDATA_IMPL(UnsignedWrapper, UnsignedWrapper, UNSIGNEDWRAPPER, unsignedwrapper, __lua_reg_UnsignedWrapper)
TBAG_LUA_USERDATA_IMPL(FloatWrapper   , FloatWrapper   , FLOATWRAPPER   , floatwrapper   , __lua_reg_FloatWrapper   )
TBAG_LUA_USERDATA_IMPL(DoubleWrapper  , DoubleWrapper  , DOUBLEWRAPPER  , doublewrapper  , __lua_reg_DoubleWrapper  )
// clang-format on

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

