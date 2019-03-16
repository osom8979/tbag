// Lua - An Extensible Extension Language

#ifndef lua_h
#define lua_h

#include <cstdarg>
#include <cstddef>

#include <libtbag/script/luajit-2.0.5/luaconf.h>

#define LUA_VERSION     "Lua 5.1"
#define LUA_RELEASE     "Lua 5.1.4"
#define LUA_VERSION_NUM 501
#define LUA_COPYRIGHT   "Copyright (C) 1994-2008 Lua.org, PUC-Rio"
#define LUA_AUTHORS     "R. Ierusalimschy, L. H. de Figueiredo & W. Celes"

// mark for precompiled code ('<esc>Lua')
#define LUA_SIGNATURE   "\033Lua"

// option for multiple returns in 'lua_pcall' and 'lua_call'
#define LUA_MULTRET    (-1)

// pseudo-indices
#define LUA_REGISTRYINDEX   (-10000)
#define LUA_ENVIRONINDEX    (-10001)
#define LUA_GLOBALSINDEX    (-10002)
#define lua_upvalueindex(i) (LUA_GLOBALSINDEX-(i))

// thread status; 0 is OK
#define LUA_YIELD       1
#define LUA_ERRRUN      2
#define LUA_ERRSYNTAX   3
#define LUA_ERRMEM      4
#define LUA_ERRERR      5

typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State *L);

// functions that read/write blocks when loading/dumping Lua chunks
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);
typedef int (*lua_Writer) (lua_State *L, const void* p, size_t sz, void* ud);

// prototype for memory-allocation functions
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);

// basic types
#define LUA_TNONE         (-1)
#define LUA_TNIL            0
#define LUA_TBOOLEAN        1
#define LUA_TLIGHTUSERDATA  2
#define LUA_TNUMBER         3
#define LUA_TSTRING         4
#define LUA_TTABLE          5
#define LUA_TFUNCTION       6
#define LUA_TUSERDATA       7
#define LUA_TTHREAD         8

// minimum Lua stack available to a C function
#define LUA_MINSTACK       20

// generic extra include file
//#if defined(LUA_USER_H)
//#include LUA_USER_H
//#endif

// type of numbers in Lua
typedef LUA_NUMBER lua_Number;

// type for integer functions
typedef LUA_INTEGER lua_Integer;

// garbage-collection function and options
#define LUA_GCSTOP          0
#define LUA_GCRESTART       1
#define LUA_GCCOLLECT       2
#define LUA_GCCOUNT         3
#define LUA_GCCOUNTB        4
#define LUA_GCSTEP          5
#define LUA_GCSETPAUSE      6
#define LUA_GCSETSTEPMUL    7

// ==================
// some useful macros
// ==================

#define  lua_pop(L,n)               lua_settop(L, -(n)-1)
#define  lua_newtable(L)            lua_createtable(L, 0, 0)
#define  lua_register(L,n,f)        (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))
#define  lua_pushcfunction(L,f)     lua_pushcclosure(L, (f), 0)
#define  lua_strlen(L,i)            lua_objlen(L, (i))
#define  lua_isfunction(L,n)        (lua_type(L, (n)) == LUA_TFUNCTION)
#define  lua_istable(L,n)           (lua_type(L, (n)) == LUA_TTABLE)
#define  lua_islightuserdata(L,n)   (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define  lua_isnil(L,n)             (lua_type(L, (n)) == LUA_TNIL)
#define  lua_isboolean(L,n)         (lua_type(L, (n)) == LUA_TBOOLEAN)
#define  lua_isthread(L,n)          (lua_type(L, (n)) == LUA_TTHREAD)
#define  lua_isnone(L,n)            (lua_type(L, (n)) == LUA_TNONE)
#define  lua_isnoneornil(L, n)      (lua_type(L, (n)) <= 0)
#define  lua_pushliteral(L, s)      lua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)
#define  lua_setglobal(L,s)         lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define  lua_getglobal(L,s)         lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define  lua_tostring(L,i)          lua_tolstring(L, (i), NULL)

// compatibility macros and functions
#define  lua_open()                 luaL_newstate()
#define  lua_getregistry(L)         lua_pushvalue(L, LUA_REGISTRYINDEX)
#define  lua_getgccount(L)          lua_gc(L, LUA_GCCOUNT, 0)
#define  lua_Chunkreader            lua_Reader
#define  lua_Chunkwriter            lua_Writer

// =========
// Debug API
// =========

// Event codes
#define LUA_HOOKCALL     0
#define LUA_HOOKRET      1
#define LUA_HOOKLINE     2
#define LUA_HOOKCOUNT    3
#define LUA_HOOKTAILRET  4

// Event masks
#define LUA_MASKCALL     (1 << LUA_HOOKCALL)
#define LUA_MASKRET      (1 << LUA_HOOKRET)
#define LUA_MASKLINE     (1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT    (1 << LUA_HOOKCOUNT)

// activation record
typedef struct lua_Debug lua_Debug;

// Functions to be called by the debuger in specific events
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);

struct lua_Debug
{
    int event;
    const char * name;          ///< (n)
    const char * namewhat;      ///< (n) 'global', 'local', 'field', 'method'
    const char * what;          ///< (S) 'Lua', 'C', 'main', 'tail'
    const char * source;        ///< (S)
    int currentline;            ///< (l)
    int nups;                   ///< (u) number of upvalues
    int linedefined;            ///< (S)
    int lastlinedefined;        ///< (S)
    char short_src[LUA_IDSIZE]; ///< (S)

    /* --[[ private part ]]-- */
    int i_ci;  ///< active function
};

#endif // lua_h

