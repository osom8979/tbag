#ifndef __SFLUA__UTILS__
#define __SFLUA__UTILS__

#include <lua.hpp>

template <typename T>
T* newDefault(lua_State * l)
{
    return new T();
}

template <typename T>
const char* metaTableName()
{
    return "";
}

template <typename T >
int generic_constructor(lua_State * l)
{
    T ** udata = (T **)lua_newuserdata(l, sizeof(T *));
    *udata = newDefault<T>(l);
    luaL_getmetatable(l, metaTableName<T>());
    lua_setmetatable(l, -2);
    return 1;
}

template <typename T >
int copy_constructor(lua_State * l,const T& copy)
{
    T ** udata = (T **)lua_newuserdata(l, sizeof(T *));
    *udata = new T(copy);
    luaL_getmetatable(l, metaTableName<T>());
    lua_setmetatable(l, -2);
    return 1;
}


template <typename T >
bool test_type(lua_State * l, int n)
{
    return test_type(l,n,metaTableName<T>());
}

bool test_type(lua_State * l, int n,const char* name);

template <typename T >
T* check_type(lua_State * l, int n)
{
    return *(T **)luaL_checkudata(l, n, metaTableName<T>());
}

template <typename T >
T check_pointertype(lua_State * l, int n)
{
    return *(T *)luaL_checkudata(l, n, metaTableName<T>());
}


template <typename T >
int destructor(lua_State * l)
{
    T* object = check_type<T>(l, 1);
    delete object;
    return 0;
}

template <typename T >
int tostring(lua_State* l)
{
    T* object = check_type<T>(l, 1);
    printf("%s__0x%llx",metaTableName<T>(),(unsigned long long)object);
    return 0;
}



#endif
