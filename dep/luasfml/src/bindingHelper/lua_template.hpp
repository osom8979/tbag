#ifndef  __SFLUA__TEMPLATE__
#define  __SFLUA__TEMPLATE__

#include <lua.hpp>
#include <string>
#include "utils.hpp"

template< typename T>
T lua_get(lua_State * l,int i)
{
    return check_pointertype<T>(l,i);
}

template< typename T>
int lua_push(lua_State * l,const T& value)
{
    copy_constructor<T>(l,value);
    return 1;
}

template<>
int lua_get<int>(lua_State * l,int i);

template<>
unsigned int lua_get<unsigned int>(lua_State * l,int i);

template<>
unsigned char lua_get<unsigned char>(lua_State * l,int i);

template<>
float lua_get<float>(lua_State * l,int i);

template<>
double lua_get<double>(lua_State * l,int i);

template<>
std::string lua_get<std::string>(lua_State * l,int i);

template<>
bool lua_get<bool>(lua_State * l,int i);

template<>
int lua_push<int>(lua_State * l,const int& value);

template<>
int lua_push<unsigned int>(lua_State * l,const unsigned int& value);

template<>
int lua_push<unsigned char>(lua_State * l,const unsigned char& value);

template<>
int lua_push<float>(lua_State * l,const float& value);

template<>
int lua_push<std::string>(lua_State * l,const std::string& value);

template<>
int lua_push<bool>(lua_State * l,const bool& value);

#endif
