/**
 * @file   LuaBypass.hpp
 * @brief  LuaBypass class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUABYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUABYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/luajit-2.0.5/lua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

// -----------
// Lua section
// -----------

// state manipulation
TBAG_API lua_State *   lua_newstate (lua_Alloc f, void * ud);
TBAG_API void          lua_close    (lua_State * L);
TBAG_API lua_State *   lua_newthread(lua_State * L);
TBAG_API lua_CFunction lua_atpanic  (lua_State * L, lua_CFunction panicf);

// basic stack manipulation
TBAG_API int  lua_gettop    (lua_State * L);
TBAG_API void lua_settop    (lua_State * L, int idx);
TBAG_API void lua_pushvalue (lua_State * L, int idx);
TBAG_API void lua_remove    (lua_State * L, int idx);
TBAG_API void lua_insert    (lua_State * L, int idx);
TBAG_API void lua_replace   (lua_State * L, int idx);
TBAG_API int  lua_checkstack(lua_State * L, int sz);
TBAG_API void lua_xmove     (lua_State * from, lua_State * to, int n);

// access functions (stack -> C)
TBAG_API int          lua_isnumber   (lua_State * L, int idx);
TBAG_API int          lua_isstring   (lua_State * L, int idx);
TBAG_API int          lua_iscfunction(lua_State * L, int idx);
TBAG_API int          lua_isuserdata (lua_State * L, int idx);
TBAG_API int          lua_type       (lua_State * L, int idx);
TBAG_API char const * lua_typename   (lua_State * L, int tp);

TBAG_API int          lua_equal   (lua_State * L, int idx1, int idx2);
TBAG_API int          lua_rawequal(lua_State * L, int idx1, int idx2);
TBAG_API int          lua_lessthan(lua_State * L, int idx1, int idx2);

TBAG_API lua_Number    lua_tonumber   (lua_State * L, int idx);
TBAG_API lua_Integer   lua_tointeger  (lua_State * L, int idx);
TBAG_API int           lua_toboolean  (lua_State * L, int idx);
TBAG_API char const *  lua_tolstring  (lua_State * L, int idx, size_t *len);
TBAG_API size_t        lua_objlen     (lua_State * L, int idx);
TBAG_API lua_CFunction lua_tocfunction(lua_State * L, int idx);
TBAG_API void *        lua_touserdata (lua_State * L, int idx);
TBAG_API lua_State *   lua_tothread   (lua_State * L, int idx);
TBAG_API void const *  lua_topointer  (lua_State * L, int idx);

// push functions (C -> stack)
TBAG_API void         lua_pushnil          (lua_State * L);
TBAG_API void         lua_pushnumber       (lua_State * L, lua_Number n);
TBAG_API void         lua_pushinteger      (lua_State * L, lua_Integer n);
TBAG_API void         lua_pushlstring      (lua_State * L, char const * s, size_t l);
TBAG_API void         lua_pushstring       (lua_State * L, char const * s);
TBAG_API char const * lua_pushvfstring     (lua_State * L, char const * fmt, va_list argp);
TBAG_API char const * lua_pushfstring      (lua_State * L, char const * fmt, ...);
TBAG_API void         lua_pushcclosure     (lua_State * L, lua_CFunction fn, int n);
TBAG_API void         lua_pushboolean      (lua_State * L, int b);
TBAG_API void         lua_pushlightuserdata(lua_State * L, void * p);
TBAG_API int          lua_pushthread       (lua_State * L);

// get functions (Lua -> stack)
TBAG_API void   lua_gettable    (lua_State * L, int idx);
TBAG_API void   lua_getfield    (lua_State * L, int idx, char const * k);
TBAG_API void   lua_rawget      (lua_State * L, int idx);
TBAG_API void   lua_rawgeti     (lua_State * L, int idx, int n);
TBAG_API void   lua_createtable (lua_State * L, int narr, int nrec);
TBAG_API void * lua_newuserdata (lua_State * L, size_t sz);
TBAG_API int    lua_getmetatable(lua_State * L, int objindex);
TBAG_API void   lua_getfenv     (lua_State * L, int idx);

// set functions (stack -> Lua)
TBAG_API void lua_settable    (lua_State * L, int idx);
TBAG_API void lua_setfield    (lua_State * L, int idx, char const * k);
TBAG_API void lua_rawset      (lua_State * L, int idx);
TBAG_API void lua_rawseti     (lua_State * L, int idx, int n);
TBAG_API int  lua_setmetatable(lua_State * L, int objindex);
TBAG_API int  lua_setfenv     (lua_State * L, int idx);

// 'load' and 'call' functions (load and run Lua code)
TBAG_API void lua_call  (lua_State * L, int nargs, int nresults);
TBAG_API int  lua_pcall (lua_State * L, int nargs, int nresults, int errfunc);
TBAG_API int  lua_cpcall(lua_State * L, lua_CFunction func, void * ud);
TBAG_API int  lua_load  (lua_State * L, lua_Reader reader, void * dt, char const * chunkname);
TBAG_API int  lua_dump  (lua_State * L, lua_Writer writer, void * data);

// coroutine functions
TBAG_API int lua_yield (lua_State * L, int nresults);
TBAG_API int lua_resume(lua_State * L, int narg);
TBAG_API int lua_status(lua_State * L);

// garbage-collection function
TBAG_API int lua_gc(lua_State * L, int what, int data);

// miscellaneous functions
TBAG_API int        lua_error    (lua_State * L);
TBAG_API int        lua_next     (lua_State * L, int idx);
TBAG_API void       lua_concat   (lua_State * L, int n);
TBAG_API lua_Alloc  lua_getallocf(lua_State * L, void ** ud);
TBAG_API void       lua_setallocf(lua_State * L, lua_Alloc f, void * ud);

// hack
TBAG_API void lua_setlevel(lua_State * from, lua_State * to);

// Debug API
TBAG_API int          lua_getstack    (lua_State * L, int level, lua_Debug * ar);
TBAG_API int          lua_getinfo     (lua_State * L, char const * what, lua_Debug * ar);
TBAG_API char const * lua_getlocal    (lua_State * L, lua_Debug const * ar, int n);
TBAG_API char const * lua_setlocal    (lua_State * L, lua_Debug const * ar, int n);
TBAG_API char const * lua_getupvalue  (lua_State * L, int funcindex, int n);
TBAG_API char const * lua_setupvalue  (lua_State * L, int funcindex, int n);
TBAG_API int          lua_sethook     (lua_State * L, lua_Hook func, int mask, int count);
TBAG_API lua_Hook     lua_gethook     (lua_State * L);
TBAG_API int          lua_gethookmask (lua_State * L);
TBAG_API int          lua_gethookcount(lua_State * L);

// From Lua 5.2
TBAG_API void * lua_upvalueid  (lua_State * L, int idx, int n);
TBAG_API void   lua_upvaluejoin(lua_State * L, int idx1, int n1, int idx2, int n2);
TBAG_API int    lua_loadx      (lua_State * L, lua_Reader reader, void * dt, char const * chunkname, char const * mode);

// -------------------
// LuaJIT public C API
// -------------------

// Control the JIT engine.
TBAG_API int luaJIT_setmode(lua_State * L, int idx, int mode);

// Enforce (dynamic) linker error for version mismatches. Call from main.
TBAG_API void LUAJIT_VERSION_SYM();

// -----------------------
// Lua Auxiliary functions
// -----------------------

TBAG_API void         luaL_openlib     (lua_State * L, char const * libname, luaL_Reg const * l, int nup);
TBAG_API void         luaL_register    (lua_State * L, char const * libname, luaL_Reg const * l);
TBAG_API int          luaL_getmetafield(lua_State * L, int obj, char const * e);
TBAG_API int          luaL_callmeta    (lua_State * L, int obj, char const * e);
TBAG_API int          luaL_typerror    (lua_State * L, int narg, char const * tname);
TBAG_API int          luaL_argerror    (lua_State * L, int numarg, char const * extramsg);
TBAG_API char const * luaL_checklstring(lua_State * L, int numArg, size_t * l);
TBAG_API char const * luaL_optlstring  (lua_State * L, int numArg, char const * def, size_t * l);
TBAG_API lua_Number   luaL_checknumber (lua_State * L, int numArg);
TBAG_API lua_Number   luaL_optnumber   (lua_State * L, int nArg, lua_Number def);

TBAG_API lua_Integer  luaL_checkinteger(lua_State * L, int numArg);
TBAG_API lua_Integer  luaL_optinteger  (lua_State * L, int nArg, lua_Integer def);

TBAG_API void luaL_checkstack(lua_State * L, int sz, char const * msg);
TBAG_API void luaL_checktype (lua_State * L, int narg, int t);
TBAG_API void luaL_checkany  (lua_State * L, int narg);

TBAG_API int    luaL_newmetatable(lua_State * L, char const * tname);
TBAG_API void * luaL_checkudata  (lua_State * L, int ud, char const * tname);

TBAG_API void luaL_where(lua_State * L, int lvl);
TBAG_API int  luaL_error(lua_State * L, char const * fmt, ...);

TBAG_API int  luaL_checkoption(lua_State * L, int narg, char const * def, char const * const lst[]);

TBAG_API int  luaL_ref  (lua_State * L, int t);
TBAG_API void luaL_unref(lua_State * L, int t, int ref);

TBAG_API int luaL_loadfile  (lua_State * L, char const * filename);
TBAG_API int luaL_loadbuffer(lua_State * L, char const * buff, size_t sz, char const * name);
TBAG_API int luaL_loadstring(lua_State * L, char const * s);

TBAG_API lua_State * luaL_newstate();

TBAG_API char const * luaL_gsub(lua_State * L, char const * s, char const * p, char const * r);

TBAG_API char const * luaL_findtable(lua_State * L, int idx, char const * fname, int szhint);

// From Lua 5.2.
TBAG_API int  luaL_fileresult (lua_State * L, int stat, char const * fname);
TBAG_API int  luaL_execresult (lua_State * L, int stat);
TBAG_API int  luaL_loadfilex  (lua_State * L, char const * filename, char const * mode);
TBAG_API int  luaL_loadbufferx(lua_State * L, char const * buff, size_t sz, char const * name, char const * mode);
TBAG_API void luaL_traceback  (lua_State * L, lua_State * L1, char const * msg, int level);

// compatibility only
TBAG_API void   luaL_buffinit  (lua_State * L, luaL_Buffer * B);
TBAG_API char * luaL_prepbuffer(luaL_Buffer * B);
TBAG_API void   luaL_addlstring(luaL_Buffer * B, char const * s, size_t l);
TBAG_API void   luaL_addstring (luaL_Buffer * B, char const * s);
TBAG_API void   luaL_addvalue  (luaL_Buffer * B);
TBAG_API void   luaL_pushresult(luaL_Buffer * B);

// --------------------
// Lua Standard library
// --------------------

TBAG_API int  luaopen_base   (lua_State * L);
TBAG_API int  luaopen_math   (lua_State * L);
TBAG_API int  luaopen_string (lua_State * L);
TBAG_API int  luaopen_table  (lua_State * L);
TBAG_API int  luaopen_io     (lua_State * L);
TBAG_API int  luaopen_os     (lua_State * L);
TBAG_API int  luaopen_package(lua_State * L);
TBAG_API int  luaopen_debug  (lua_State * L);
TBAG_API int  luaopen_bit    (lua_State * L);
TBAG_API int  luaopen_jit    (lua_State * L);
TBAG_API int  luaopen_ffi    (lua_State * L);
TBAG_API void luaL_openlibs  (lua_State * L);

// ------------------
// more compatibility
// ------------------

TBAG_API int lua_absindex(lua_State * L, int i);
TBAG_API void luaL_setmetatable(lua_State * L, char const * tname);

// -----
// Extra
// -----

TBAG_API void luadebug_printstack(lua_State * L);

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUABYPASS_HPP__

