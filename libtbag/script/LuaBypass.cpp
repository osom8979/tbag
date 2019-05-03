/**
 * @file   LuaBypass.cpp
 * @brief  LuaBypass class implementation.
 * @author zer0
 * @date   2018-10-30
 */

// [WARNING] Don't change the order of including.
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>
}

#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Err.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

// -----------
// Lua section
// -----------

lua_State * lua_newstate(lua_Alloc f, void * ud)
{
    return ::lua_newstate(f, ud);
}

void lua_close(lua_State * L)
{
    ::lua_close(L);
}

lua_State * lua_newthread(lua_State * L)
{
    return (lua_State*)::lua_newthread(L);
}

lua_CFunction lua_atpanic(lua_State * L, lua_CFunction panicf)
{
    return ::lua_atpanic(L, panicf);
}

int lua_gettop(lua_State * L)
{
    return ::lua_gettop(L);
}

void lua_settop(lua_State * L, int idx)
{
    ::lua_settop(L, idx);
}

void lua_pushvalue(lua_State * L, int idx)
{
    ::lua_pushvalue(L, idx);
}

void lua_remove(lua_State * L, int idx)
{
    ::lua_remove(L, idx);
}

void lua_insert(lua_State * L, int idx)
{
    ::lua_insert(L, idx);
}

void lua_replace(lua_State * L, int idx)
{
    ::lua_replace(L, idx);
}

int lua_checkstack(lua_State * L, int sz)
{
    return ::lua_checkstack(L, sz);
}

void lua_xmove(lua_State * from, lua_State * to, int n)
{
    ::lua_xmove(from, to, n);
}

int lua_isnumber(lua_State * L, int idx)
{
    return ::lua_isnumber(L, idx);
}

int lua_isstring(lua_State * L, int idx)
{
    return ::lua_isstring(L, idx);
}

int lua_iscfunction(lua_State *L, int idx)
{
    return ::lua_iscfunction(L, idx);
}

int lua_isuserdata(lua_State * L, int idx)
{
    return ::lua_isuserdata(L, idx);
}

int lua_type(lua_State * L, int idx)
{
    return ::lua_type(L, idx);
}

char const * lua_typename(lua_State * L, int tp)
{
    return ::lua_typename(L, tp);
}

int lua_equal(lua_State * L, int idx1, int idx2)
{
    return ::lua_equal(L, idx1, idx2);
}

int lua_rawequal(lua_State * L, int idx1, int idx2)
{
    return ::lua_rawequal(L, idx1, idx2);
}

int lua_lessthan(lua_State * L, int idx1, int idx2)
{
    return ::lua_lessthan(L, idx1, idx2);
}

lua_Number lua_tonumber(lua_State * L, int idx)
{
    return (lua_Number)::lua_tonumber(L, idx);
}

lua_Integer lua_tointeger(lua_State * L, int idx)
{
    return (lua_Integer)::lua_tointeger(L, idx);
}

int lua_toboolean(lua_State * L, int idx)
{
    return ::lua_toboolean(L, idx);
}

char const * lua_tolstring(lua_State * L, int idx, size_t * len)
{
    return ::lua_tolstring(L, idx, len);
}

size_t lua_objlen(lua_State * L, int idx)
{
    return ::lua_objlen(L, idx);
}

lua_CFunction lua_tocfunction(lua_State * L, int idx)
{
    return (lua_CFunction)::lua_tocfunction(L, idx);
}

void * lua_touserdata(lua_State * L, int idx)
{
    return ::lua_touserdata(L, idx);
}

lua_State * lua_tothread(lua_State * L, int idx)
{
    return (lua_State*)::lua_tothread(L, idx);
}

void const * lua_topointer(lua_State * L, int idx)
{
    return ::lua_topointer(L, idx);
}

void lua_pushnil(lua_State * L)
{
    ::lua_pushnil(L);
}

void lua_pushnumber(lua_State * L, lua_Number n)
{
    ::lua_pushnumber(L, (::lua_Number)n);
}

void lua_pushinteger(lua_State * L, lua_Integer n)
{
    ::lua_pushinteger(L, (::lua_Integer)n);
}

void lua_pushlstring(lua_State * L, char const * s, size_t l)
{
    ::lua_pushlstring(L, s, l);
}

void lua_pushstring(lua_State * L, char const * s)
{
    ::lua_pushstring(L, s);
}

char const * lua_pushvfstring(lua_State * L, char const * fmt, va_list argp)
{
    return ::lua_pushvfstring(L, fmt, argp);
}

char const * lua_pushfstring(lua_State * L, char const * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char const * result = ::lua_pushvfstring(L, fmt, args);
    va_end(args);
    return result;
}

void lua_pushcclosure(lua_State * L, lua_CFunction fn, int n)
{
    ::lua_pushcclosure(L, fn, n);
}

void lua_pushboolean(lua_State * L, int b)
{
    ::lua_pushboolean(L, b);
}

void lua_pushlightuserdata(lua_State * L, void * p)
{
    ::lua_pushlightuserdata(L, p);
}

int lua_pushthread(lua_State * L)
{
    return ::lua_pushthread(L);
}

void lua_gettable(lua_State * L, int idx)
{
    ::lua_gettable(L, idx);
}

void lua_getfield(lua_State * L, int idx, char const * k)
{
    ::lua_getfield(L, idx, k);
}

void lua_rawget(lua_State * L, int idx)
{
    ::lua_rawget(L, idx);
}

void lua_rawgeti(lua_State * L, int idx, int n)
{
    ::lua_rawgeti(L, idx, n);
}

void lua_createtable(lua_State * L, int narr, int nrec)
{
    ::lua_createtable(L, narr, nrec);
}

void * lua_newuserdata(lua_State * L, size_t sz)
{
    return ::lua_newuserdata(L, sz);
}

int lua_getmetatable(lua_State * L, int objindex)
{
    return ::lua_getmetatable(L, objindex);
}

void lua_getfenv(lua_State * L, int idx)
{
    ::lua_getfenv(L, idx);
}

void lua_settable(lua_State * L, int idx)
{
    ::lua_settable(L, idx);
}

void lua_setfield(lua_State * L, int idx, char const * k)
{
    ::lua_setfield(L, idx, k);
}

void lua_rawset(lua_State * L, int idx)
{
    ::lua_rawset(L, idx);
}

void lua_rawseti(lua_State * L, int idx, int n)
{
    ::lua_rawseti(L, idx, n);
}

int lua_setmetatable(lua_State * L, int objindex)
{
    return ::lua_setmetatable(L, objindex);
}

int lua_setfenv(lua_State * L, int idx)
{
    return ::lua_setfenv(L, idx);
}

void lua_call(lua_State * L, int nargs, int nresults)
{
    ::lua_call(L, nargs, nresults);
}

int lua_pcall(lua_State * L, int nargs, int nresults, int errfunc)
{
    return ::lua_pcall(L, nargs, nresults, errfunc);
}

int lua_cpcall(lua_State * L, lua_CFunction func, void * ud)
{
    return ::lua_cpcall(L, func, ud);
}

int lua_load(lua_State * L, lua_Reader reader, void * dt, char const * chunkname)
{
    return ::lua_load(L, reader, dt, chunkname);
}

int lua_dump(lua_State * L, lua_Writer writer, void * data)
{
    return ::lua_dump(L, writer, data);
}

int lua_yield(lua_State * L, int nresults)
{
    return ::lua_yield(L, nresults);
}

int lua_resume(lua_State * L, int narg)
{
    return ::lua_resume(L, narg);
}

int lua_status(lua_State * L)
{
    return ::lua_status(L);
}

int lua_gc(lua_State * L, int what, int data)
{
    return ::lua_gc(L, what, data);
}

int lua_error(lua_State * L)
{
    return ::lua_error(L);
}

int lua_next(lua_State * L, int idx)
{
    return ::lua_next(L, idx);
}

void lua_concat(lua_State * L, int n)
{
    ::lua_concat(L, n);
}

lua_Alloc lua_getallocf(lua_State * L, void ** ud)
{
    return ::lua_getallocf(L, ud);
}

void lua_setallocf(lua_State * L, lua_Alloc f, void * ud)
{
    ::lua_setallocf(L, f, ud);
}

void lua_setlevel(lua_State * from, lua_State * to)
{
    tDLogW("lua_setlevel() Not implementation");
}

int lua_getstack(lua_State * L, int level, lua_Debug * ar)
{
    return ::lua_getstack(L, level, ar);
}

int lua_getinfo(lua_State * L, char const * what, lua_Debug * ar)
{
    return ::lua_getinfo(L, what, ar);
}

char const * lua_getlocal(lua_State * L, lua_Debug const * ar, int n)
{
    return ::lua_getlocal(L, ar, n);
}

char const * lua_setlocal(lua_State * L, lua_Debug const * ar, int n)
{
    return ::lua_setlocal(L, ar, n);
}

char const * lua_getupvalue(lua_State * L, int funcindex, int n)
{
    return ::lua_getupvalue(L, funcindex, n);
}

char const * lua_setupvalue(lua_State * L, int funcindex, int n)
{
    return ::lua_setupvalue(L, funcindex, n);
}

int lua_sethook(lua_State * L, lua_Hook func, int mask, int count)
{
    return ::lua_sethook(L, func, mask, count);
}

lua_Hook lua_gethook(lua_State * L)
{
    return ::lua_gethook(L);
}

int lua_gethookmask(lua_State * L)
{
    return ::lua_gethookmask(L);
}

int lua_gethookcount(lua_State * L)
{
    return ::lua_gethookcount(L);
}

void * lua_upvalueid(lua_State * L, int idx, int n)
{
    return ::lua_upvalueid(L, idx, n);
}

void lua_upvaluejoin(lua_State * L, int idx1, int n1, int idx2, int n2)
{
    ::lua_upvaluejoin(L, idx1, n1, idx2, n2);
}

int lua_loadx(lua_State * L, lua_Reader reader, void * dt, char const * chunkname, char const * mode)
{
    return ::lua_loadx(L, reader, dt, chunkname, mode);
}

// ---------------
// LuaJIT section.
// ---------------

int luaJIT_setmode(lua_State * L, int idx, int mode)
{
    return ::luaJIT_setmode(L, idx, mode);
}

void LUAJIT_VERSION_SYM()
{
    ::LUAJIT_VERSION_SYM();
}

// -----------------------
// Lua Auxiliary functions
// -----------------------

void luaL_openlib(lua_State * L, char const * libname, luaL_Reg const * l, int nup)
{
    ::luaL_openlib(L, libname, l, nup);
}

void luaL_register(lua_State * L, char const * libname, luaL_Reg const * l)
{
    ::luaL_register(L, libname, l);
}

int luaL_getmetafield(lua_State * L, int obj, char const * e)
{
    return ::luaL_getmetafield(L, obj, e);
}

int luaL_callmeta(lua_State * L, int obj, char const * e)
{
    return ::luaL_callmeta(L, obj, e);
}

int luaL_typerror(lua_State * L, int narg, char const * tname)
{
    return ::luaL_typerror(L, narg, tname);
}

int luaL_argerror(lua_State * L, int numarg, char const * extramsg)
{
    return ::luaL_argerror(L, numarg, extramsg);
}

char const * luaL_checklstring(lua_State * L, int numArg, size_t * l)
{
    return ::luaL_checklstring(L, numArg, l);
}

char const * luaL_optlstring(lua_State * L, int numArg, char const * def, size_t * l)
{
    return ::luaL_optlstring(L, numArg, def, l);
}

lua_Number luaL_checknumber(lua_State * L, int numArg)
{
    return ::luaL_checknumber(L, numArg);
}

lua_Number luaL_optnumber(lua_State * L, int nArg, lua_Number def)
{
    return ::luaL_optnumber(L, nArg, def);
}

lua_Integer luaL_checkinteger(lua_State * L, int numArg)
{
    return ::luaL_checkinteger(L, numArg);
}

lua_Integer luaL_optinteger(lua_State * L, int nArg, lua_Integer def)
{
    return ::luaL_optinteger(L, nArg, def);
}

void luaL_checkstack(lua_State * L, int sz, char const * msg)
{
    ::luaL_checkstack(L, sz, msg);
}

void luaL_checktype(lua_State * L, int narg, int t)
{
    ::luaL_checktype(L, narg, t);
}

void luaL_checkany(lua_State * L, int narg)
{
    ::luaL_checkany(L, narg);
}

int luaL_newmetatable(lua_State * L, char const * tname)
{
    auto const CODE = ::luaL_newmetatable(L, tname);
    if (CODE == 0) {
        tDLogD("luaL_newmetatable() Exists MetaTable: {}", tname);
    } else {
        // Insert 'tname = [New Table]' into the registry.
        // Then, push the table onto the stack.
        tDLogD("luaL_newmetatable() Create new MetaTable: {}", tname);
#if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM <= 501)
        ::lua_pushlstring(L, "__name", 6);
        ::lua_pushlstring(L, tname, ::strlen(tname));
        ::lua_rawset(L, -3);
#endif
    }
    return CODE;
}

void * luaL_checkudata(lua_State * L, int ud, char const * tname)
{
    return ::luaL_checkudata(L, ud, tname);
}

void luaL_where(lua_State * L, int lvl)
{
    ::luaL_where(L, lvl);
}

int luaL_error(lua_State * L, char const * fmt, ...)
{
    using namespace libtbag::string;
    va_list args;
    va_start(args, fmt);
    std::string const RESULT = vformat(fmt, DEFAULT_FORMAT_BUFFER_SIZE, args);
    va_end(args);
    return ::luaL_error(L, RESULT.c_str());
}

int luaL_checkoption(lua_State * L, int narg, char const * def, char const * const lst[])
{
    return ::luaL_checkoption(L, narg, def, lst);
}

int luaL_ref(lua_State * L, int t)
{
    return ::luaL_ref(L, t);
}

void luaL_unref(lua_State * L, int t, int ref)
{
    ::luaL_unref(L, t, ref);
}

int luaL_loadfile(lua_State * L, char const * filename)
{
    return ::luaL_loadfile(L, filename);
}

int luaL_loadbuffer(lua_State * L, char const * buff, size_t sz, char const * name)
{
    return ::luaL_loadbuffer(L, buff, sz, name);
}

int luaL_loadstring(lua_State * L, char const * s)
{
    return ::luaL_loadstring(L, s);
}

lua_State * luaL_newstate()
{
    auto * result = ::luaL_newstate();
    if (result == nullptr) {
        Err const CODE = getGlobalSystemError();
        tDLogE("luaL_newstate() error({}): {}", CODE, libtbag::getErrDetail(CODE));
    }
    return result;
}

char const * luaL_gsub(lua_State * L, char const * s, char const * p, char const * r)
{
    return ::luaL_gsub(L, s, p, r);
}

char const * luaL_findtable(lua_State * L, int idx, char const * fname, int szhint)
{
    return ::luaL_findtable(L, idx, fname, szhint);
}

int luaL_fileresult(lua_State * L, int stat, char const * fname)
{
    return ::luaL_fileresult(L, stat, fname);
}

int luaL_execresult(lua_State * L, int stat)
{
    return ::luaL_execresult(L, stat);
}

int luaL_loadfilex(lua_State * L, char const * filename, char const * mode)
{
    return ::luaL_loadfilex(L, filename, mode);
}

int luaL_loadbufferx(lua_State * L, char const * buff, size_t sz, char const * name, char const * mode)
{
    return ::luaL_loadbufferx(L, buff, sz, name, mode);
}

void luaL_traceback(lua_State * L, lua_State * L1, char const * msg, int level)
{
    ::luaL_traceback(L, L1, msg, level);
}

void luaL_buffinit(lua_State * L, luaL_Buffer * B)
{
    ::luaL_buffinit(L, B);
}

char * luaL_prepbuffer(luaL_Buffer * B)
{
    return ::luaL_prepbuffer(B);
}

void luaL_addlstring(luaL_Buffer * B, char const * s, size_t l)
{
    ::luaL_addlstring(B, s, l);
}

void luaL_addstring(luaL_Buffer * B, char const * s)
{
    ::luaL_addstring(B, s);
}

void luaL_addvalue(luaL_Buffer * B)
{
    ::luaL_addvalue(B);
}

void luaL_pushresult(luaL_Buffer * B)
{
    ::luaL_pushresult(B);
}

// --------------------
// Lua Standard library
// --------------------

int luaopen_base(lua_State * L)
{
    return ::luaopen_base(L);
}

int luaopen_math(lua_State * L)
{
    return ::luaopen_math(L);
}

int luaopen_string(lua_State * L)
{
    return ::luaopen_string(L);
}

int luaopen_table(lua_State * L)
{
    return ::luaopen_table(L);
}

int luaopen_io(lua_State * L)
{
    return ::luaopen_io(L);
}

int luaopen_os(lua_State * L)
{
    return ::luaopen_os(L);
}

int luaopen_package(lua_State * L)
{
    return ::luaopen_package(L);
}

int luaopen_debug(lua_State * L)
{
    return ::luaopen_debug(L);
}

int luaopen_bit(lua_State * L)
{
    return ::luaopen_bit(L);
}

int luaopen_jit(lua_State * L)
{
    return ::luaopen_jit(L);
}

int luaopen_ffi(lua_State * L)
{
    return ::luaopen_ffi(L);
}

void luaL_openlibs(lua_State * L)
{
    return ::luaL_openlibs(L);
}

int lua_absindex(lua_State * L, int i)
{
    if (i < 0 && i > LUA_REGISTRYINDEX) {
        i += ::lua_gettop(L) + 1;
    }
    return i;
}

void luaL_setmetatable(lua_State * L, char const * tname)
{
    ::luaL_checkstack(L, 1, "Not enough stack slots");
    ::lua_getfield(L, LUA_REGISTRYINDEX, tname); // == luaL_getmetatable(L, tname);
    ::lua_setmetatable(L, -2);
}

Pairs getRegistryPairs(lua_State * L)
{
    Pairs result;
    ::lua_pushnil(L);  // First key.
    while (::lua_next(L, LUA_REGISTRYINDEX) != 0) {
        // Key index: -2
        // Value index: -1
        // printf("%s - %s\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
        // lua_pop(L, 1); // removes 'value'; keeps 'key' for next iteration
    }
    return result;
}

void luadebug_printstack(lua_State * L)
{
    int const TOP = ::lua_gettop(L);
    if (TOP == 0) {
        std::cout << "||" << std::endl;
    } else {
        std::cout << '|';
    }

    for (int i = 1; i <= TOP; ++i) {
        auto const TYPE = ::lua_type(L, i);
        switch (TYPE) {
        case LUA_TNUMBER:
            std::cout << ::lua_tonumber(L, i);
            break;
        case LUA_TBOOLEAN:
            std::cout << (::lua_toboolean(L, i) == 1 ? "true" : "false");
            break;
        case LUA_TSTRING:
            std::cout << '"' << ::lua_tolstring(L, i, nullptr) << '"';
            break;
        case LUA_TTABLE:
            if (::luaL_getmetafield(L, i, "__name")) {
                std::cout << ::lua_tolstring(L, -1, nullptr);
                ::lua_settop(L, -2);
            } else {
                std::cout << "table";
            }
            break;
        default:
            std::cout << ::lua_typename(L, TYPE);
            break;
        }
        std::cout << '|';
    }
    std::cout << std::endl;
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

