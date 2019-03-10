#include "lua_template.hpp"
#include <lua.hpp>

template<>
int lua_get<int>(lua_State * l,int i)
{
    return luaL_checkinteger(l,i);
}

template<>
unsigned int lua_get<unsigned int>(lua_State * l,int i)
{
    int value = (int)luaL_checkinteger(l,i);
    if(value < 0 ) luaL_argerror(l,i,"Must be strictly positive");
    return value;
}

template<>
unsigned char lua_get<unsigned char>(lua_State * l,int i)
{
    unsigned int value = lua_get<unsigned int>(l,i);
    if(value > 255 ) luaL_argerror(l,i,"Must be inferior to 256");
    return value;
}


template<>
float lua_get<float>(lua_State * l,int i)
{
    return luaL_checknumber(l,i);
}

template<>
double lua_get<double>(lua_State * l,int i)
{
    return luaL_checknumber(l,i);
}

template<>
std::string lua_get<std::string>(lua_State * l,int i)
{
    size_t lenght = 0;
    const char* tmp = luaL_checklstring(l,i,&lenght);
    return std::string(tmp,lenght);
}

template<>
bool lua_get<bool>(lua_State * l,int i)
{
    return lua_toboolean(l,i) == 0;
}

template<>
int lua_push<int>(lua_State * l,const int& value)
{
    lua_pushinteger(l,value);
    return 1;
}

template<>
int lua_push<unsigned int>(lua_State * l,const unsigned int& value)
{
    lua_pushinteger(l,value);
    return 1;
}

template<>
int lua_push<unsigned char>(lua_State * l,const unsigned char& value)
{
    lua_pushinteger(l,value);
    return 1;
}

template<>
int lua_push<float>(lua_State * l,const float& value)
{
    lua_pushnumber(l,value);
    return 1;
}

template<>
int lua_push<std::string>(lua_State * l,const std::string& value)
{
    lua_pushstring(l,value.c_str());
    return 1;
}

template<>
int lua_push<bool>(lua_State * l,const bool& value)
{
    lua_pushboolean(l,value?1:0);
    return 1;
}
