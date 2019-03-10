#ifndef  __SFLUA__MACRO__
#define  __SFLUA__MACRO__

#include "utils.hpp"

#define LUA_CLASS_NAME(name) const char* name##MetatableName = "luaL_" #name; const char* name##TypeName = #name;
#define IMPLEMENT_LUA_METATABLENAME(name,type) template <> const char* metaTableName<type>() {return name##MetatableName;}

#define IMPLEMENT_LUA_DEFAULT_CTOR(name,type) int   name##_ctor     (lua_State * l)             { return generic_constructor<type>(l);   }
#define IMPLEMENT_LUA_COPY_CTOR(name,type)    int   name##_copy_ctor(lua_State * l,const type& copy) { return copy_constructor<type>(l,copy); }
#define IMPLEMENT_LUA_DEFAULT_DTOR(name,type) int   name##_dtor     (lua_State * l)             { return destructor<type>(l);            }
//define IMPLEMENT_LUA_TYPE_CHECKER(name,type) type* name##_check    (lua_State * l,int n)       { return check_type<type>(l,n);          }
#define IMPLEMENT_LUA_TOSTRING(name,type)     int   name##_tostring (lua_State * l)             { return tostring<type>(l);              }

#define IMPLEMENT_LUA_CLASS(name,type)      \
    LUA_CLASS_NAME(name)                    \
    IMPLEMENT_LUA_METATABLENAME(name,type)  \
    IMPLEMENT_LUA_METATABLENAME(name,type*)  \
    IMPLEMENT_LUA_DEFAULT_CTOR(name,type)   \
    IMPLEMENT_LUA_DEFAULT_DTOR(name,type)   \
    IMPLEMENT_LUA_TOSTRING(name,type)


#define LUA_BEGIN_REGISTER_CLASS(name)             \
void register_##name(lua_State * l)                \
{                                                  \
    luaL_newmetatable(l, name##MetatableName );    \
    luaL_register(l,0, name##_regs);           \
    lua_pushvalue(l, -1);                          \
    lua_setfield(l, -1, "__index");                \
    lua_setglobal(l, name##TypeName);              \


#define LUA_END_REGISTER_CLASS                     \
}                                                  \

#define LUA_REGISTER_CLASS(name) \
    LUA_BEGIN_REGISTER_CLASS(name) \
    LUA_END_REGISTER_CLASS


#define LUA_DEFAULT_CLASS_FUNC(name)      \
    { "new"         , name##_ctor      }, \
    { "__gc"        , name##_dtor      }, \
    { "__tostring"  , name##_tostring  }, \


#define IMPLEMENT_LUA_GENERIC_GETTER(name,type,getType,getname) \
int name##_get_##getName(lua_State * l) \
{ \
    typpe* obj = check_type<type>(l,1);\
    return 1; \
} \



#endif
