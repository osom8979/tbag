// Auxiliary functions for building Lua libraries

#ifndef lauxlib_h
#define lauxlib_h

#include <stddef.h>
#include <stdio.h>

#include <libtbag/script/luajit-2.0.3/lua.h>

#define luaL_getn(L,i)          ((int)lua_objlen(L, i))
#define luaL_setn(L,i,j)        ((void)0)  /* no op! */

// extra error code for 'luaL_load'
#define LUA_ERRFILE             (LUA_ERRERR+1)

typedef struct luaL_Reg {
    char const * name;
    lua_CFunction func;
} luaL_Reg;

// ==================
// some useful macros
// ==================

#define luaL_argcheck(L, cond,numarg, extramsg) \
        ((void)((cond) || luaL_argerror(L, (numarg), (extramsg))))

#define luaL_checkstring(L,n)   (luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)   (luaL_optlstring(L, (n), (d), NULL))
#define luaL_checkint(L,n)      ((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)      ((int)luaL_optinteger(L, (n), (d)))
#define luaL_checklong(L,n)     ((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)     ((long)luaL_optinteger(L, (n), (d)))
#define luaL_typename(L,i)      lua_typename(L, lua_type(L,(i)))
#define luaL_dofile(L, fn)      (luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define luaL_dostring(L, s)     (luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
#define luaL_getmetatable(L,n)  (lua_getfield(L, LUA_REGISTRYINDEX, (n)))
#define luaL_opt(L,f,n,d)       (lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

// ===========================
// Generic Buffer manipulation
// ===========================

typedef struct luaL_Buffer {
    char * p;         ///< current position in buffer
    int lvl;          ///< number of strings in the stack (level)
    lua_State * L;
    char buffer[LUAL_BUFFERSIZE];
} luaL_Buffer;

#define luaL_addchar(B,c) \
        ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || luaL_prepbuffer(B)), \
        (*(B)->p++ = (char)(c)))

// ------------------
// compatibility only
// ------------------

#define luaL_putchar(B,c)    luaL_addchar(B,c)
#define luaL_addsize(B,n)    ((B)->p += (n))

// -----------------------------
// compatibility with ref system
// -----------------------------

// pre-defined references
#define LUA_NOREF   (-2)
#define LUA_REFNIL  (-1)

#define lua_ref(L,lock) \
        ((lock) ? luaL_ref(L, LUA_REGISTRYINDEX) : \
        (lua_pushstring(L, "unlocked references are obsolete"), lua_error(L), 0))

#define lua_unref(L,ref)   luaL_unref(L, LUA_REGISTRYINDEX, (ref))
#define lua_getref(L,ref)  lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))

#define luaL_reg  luaL_Reg

#endif // lauxlib_h

