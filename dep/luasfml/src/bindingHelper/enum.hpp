
#ifndef _LUA_TYPED_ENUMS_H_
#define _LUA_TYPED_ENUMS_H_

#include<lua.hpp>

// Adds an enumerated type into Lua.
//
// L - Lua state.
// tname - The name of the enum type.
// <name:string><value:int> pairs, terminated by a null (0).
//
// EX: Assume the following enum in C-code:
//
//  typedef enum _TYPE { TYPE_FOO=0, TYPE_BAR, TYPE_BAZ, TYPE_MAX } TYPE;
//
// To map this to Lua, do the following:
//
//  add_enum_to_lua( L, "type",
//    "foo", TYPE_FOO,
//    "bar", TYPE_BAR,
//    "baz", TYPE_BAZ,
//    0);
//
// In Lua, you can access the enum as:
//  type.foo
//  type.bar
//  type.baz
//
// You can print the value in Lua by:
//  > print(type.foo)
//

bool add_enum_to_lua(lua_State* L, const char* tname, ...);

#endif // _LUA_TYPED_ENUMS_H_
