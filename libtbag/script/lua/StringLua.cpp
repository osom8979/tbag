/**
 * @file   StringLua.cpp
 * @brief  StringLua class implementation.
 * @author zer0
 * @date   2019-05-26
 */

#include <libtbag/script/lua/StringLua.hpp>
#include <libtbag/string/fmt/format.h>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

std::string luaE_checkfformat(lua_State * L, int greater_equals_index)
{
    greater_equals_index = lua_absindex(L, greater_equals_index);
    int const top = lua_gettop(L);
    assert(top >= greater_equals_index);
    int const vargs_size = top - greater_equals_index;
    assert(vargs_size >= 0);

    char const * format_text = luaL_checkstring(L, greater_equals_index);
    if (vargs_size == 0) {
        return format_text;
    }
    assert(vargs_size >= 1);

    auto const DATA_SIZE = vargs_size + 1;
    using Context = fmt::format_context;
    using ValueType = fmt::basic_format_arg<Context>;
    std::shared_ptr<ValueType> data(new ValueType [DATA_SIZE], std::default_delete<ValueType[]>());
    auto * array = data.get();

    int next_lua_arg_index = greater_equals_index + 1;
    int insert_index = 0;

    for (; next_lua_arg_index <= top; ++next_lua_arg_index) {
        auto const current_type = lua_type(L, next_lua_arg_index);
        switch (current_type) {
        case LUA_TNONE:
            break;

        case LUA_TNIL:
            break;

        case LUA_TBOOLEAN:
            BLOCK_DO("Lua boolean type") {
                bool value = luaL_checkboolean(L, next_lua_arg_index);
                array[insert_index] = fmt::internal::make_arg<Context>(value);
            } BLOCK_END();
            ++insert_index;
            break;

        case LUA_TLIGHTUSERDATA:
            break;

        case LUA_TNUMBER:
            if (lua_isinteger(L, next_lua_arg_index)) {
                int value = luaL_checkinteger(L, next_lua_arg_index);
                array[insert_index] = fmt::internal::make_arg<Context>(value);
            } else {
                double value = luaL_checknumber(L, next_lua_arg_index);
                array[insert_index] = fmt::internal::make_arg<Context>(value);
            }
            ++insert_index;
            break;

        case LUA_TSTRING:
            BLOCK_DO("Lua string type") {
                char const * value = luaL_checkstring(L, next_lua_arg_index);
                array[insert_index] = fmt::internal::make_arg<Context>(value);
            } BLOCK_END();
            ++insert_index;
            break;

        case LUA_TTABLE:
            break;

        case LUA_TFUNCTION:
            break;

        case LUA_TUSERDATA:
            break;

        case LUA_TTHREAD:
            break;

        default:
            luaL_error(L, "Unknown lua type.");
            return std::string();
        }
    }

    fmt::basic_memory_buffer<char> buffer;
    fmt::basic_format_args<Context> args(array, vargs_size);
    fmt::internal::vformat_to(buffer, fmt::to_string_view(format_text), args);
    return fmt::to_string(buffer);
}

int _format(lua_State * L)
{
    lua_pushstring(L, luaE_checkfformat(L, 1).c_str());
    return 1;
}

#ifndef STRING_REGISTER
#define STRING_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_string[] = {
        STRING_REGISTER(format),
        { nullptr, nullptr }
};

bool luaE_open_string(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_string);
    {
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

