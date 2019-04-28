/**
 * @file   LuaMachine.cpp
 * @brief  LuaMachine class implementation.
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */

#include <libtbag/script/LuaMachine.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/script/lua/RayLua.hpp>

#include <cassert>
#include <cstring>
#include <cstdarg>

#include <sstream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

LuaMachine::LuaMachine() : _state(nullptr)
{
    auto * L = luaL_newstate();
    assert(L != nullptr);

    _state.reset(L, [](lua_State * l){
        lua_close(l);
    });
    assert(static_cast<bool>(_state));

    initDefaultOpenLibraries();
    initDefaultLuaPath();
    initDefaultLuaCPath();
    initDefaultTbagTable();

    if (!libtbag::script::lua::luaopen_ray(L)) {
        tDLogW("LuaMachine::LuaMachine() Ray bind failed.");
    }
}

LuaMachine::LuaMachine(std::nullptr_t) TBAG_NOEXCEPT : _state(nullptr)
{
    // EMPTY.
}

LuaMachine::LuaMachine(LuaMachine const & obj) TBAG_NOEXCEPT : LuaMachine(nullptr)
{
    (*this) = obj;
}

LuaMachine::LuaMachine(LuaMachine && obj) TBAG_NOEXCEPT : LuaMachine(nullptr)
{
    (*this) = std::move(obj);
}

LuaMachine::~LuaMachine()
{
    // EMPTY.
}

LuaMachine & LuaMachine::operator =(LuaMachine const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

LuaMachine & LuaMachine::operator =(LuaMachine && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void LuaMachine::copy(LuaMachine const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state = obj._state;
    }
}

void LuaMachine::swap(LuaMachine & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state.swap(obj._state);
    }
}

std::string LuaMachine::getDefaultLuaPath()
{
    std::stringstream path;
    path << "." LUA_DIRSEP LUA_PATH_MARK ".lua" << LUA_PATHSEP
         << "." LUA_DIRSEP LUA_PATH_MARK LUA_DIRSEP "init.lua";
    return path.str();
}

std::string LuaMachine::getDefaultLuaCPath()
{
    std::stringstream cpath;
    cpath << "." LUA_DIRSEP LUA_PATH_MARK LUA_MODULE_SUFFIX;
    return cpath.str();
}

void LuaMachine::initDefaultOpenLibraries()
{
    auto * L = _state.get();
    assert(L != nullptr);

    lua_gc(L, LUA_GCSTOP, 0);
    luaL_openlibs(L);
    lua_gc(L, LUA_GCRESTART, -1);
}

void LuaMachine::initDefaultLuaPath()
{
    setLuaPath(getDefaultLuaPath());
}

void LuaMachine::initDefaultLuaCPath()
{
    setLuaCPath(getDefaultLuaCPath());
}

void LuaMachine::initDefaultTbagTable()
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_newtable(L);
    lua_pushstring(L, libtbag::util::getTbagVersion().toString().c_str());
    lua_setfield(L, -2, "version");
    lua_setglobal(L, lua_tbag_name());
}

void LuaMachine::reset()
{
    _state.reset();
}

std::string LuaMachine::getLuaPath() const
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    std::string const PACKAGE_PATH = lua_tostring(L, -1);
    lua_pop(L, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaPaths() const
{
    return libtbag::string::splitTokens(getLuaPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaPath(std::string const & path)
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_getglobal(L, "package");
    lua_pushstring(L, path.c_str());
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);
}

bool LuaMachine::appendLuaPath(std::string const & path)
{
    auto const PATH = getLuaPath();
    if (PATH.empty()) {
        setLuaPath(path);
    } else {
        setLuaPath(PATH + LUA_PATHSEP + path);
    }
    return true;
}

std::string LuaMachine::getLuaCPath() const
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "cpath");
    std::string const PACKAGE_PATH = lua_tostring(L, -1);
    lua_pop(L, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaCPaths() const
{
    return libtbag::string::splitTokens(getLuaCPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaCPath(std::string const & path)
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_getglobal(L, "package");
    lua_pushstring(L, path.c_str());
    lua_setfield(L, -2, "cpath");
    lua_pop(L, 1);
}

bool LuaMachine::appendLuaCPath(std::string const & path)
{
    auto const PATH = getLuaCPath();
    if (PATH.empty()) {
        setLuaCPath(path);
    } else {
        setLuaCPath(PATH + LUA_PATHSEP + path);
    }
    return true;
}

std::string LuaMachine::getTbagVersion() const
{
    auto * L = _state.get();
    assert(L != nullptr);
    lua_getglobal(L, lua_tbag_name());
    lua_getfield(L, -1, "version");
    std::string const VERSION_PATH = lua_tostring(L, -1);
    lua_pop(L, 2);
    return VERSION_PATH;
}

bool LuaMachine::runScriptFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    if (!Path(path).isRegularFile()) {
        return false;
    }
    auto * L = _state.get();
    assert(L != nullptr);
    return luaL_dofile(L, path.c_str()) == 0;
}

bool LuaMachine::runScript(std::string const & code)
{
    if (code.empty()) {
        return false;
    }
    auto * L = _state.get();
    assert(L != nullptr);
    return luaL_dostring(L, code.c_str()) == 0;
}

std::string LuaMachine::findScriptPath(std::string const & filename, bool include_working) const
{
    for (auto & path : getLuaPaths()) {
        using namespace libtbag::filesystem;
        auto const FILE_PATH = Path(path) / filename;
        if (FILE_PATH.isRegularFile()) {
            return FILE_PATH;
        }
    }

    if (include_working) {
        auto const FILE_PATH = libtbag::filesystem::Path::getWorkDir() / filename;
        if (FILE_PATH.isRegularFile()) {
            return FILE_PATH;
        }
    }

    return std::string();
}

LuaMachine::Version LuaMachine::getLuaVersion() const
{
    return Version((LUA_VERSION_NUM/100), (LUA_VERSION_NUM%100));
}

LuaMachine::Version LuaMachine::getLuaJITVersion() const
{
    return Version(LUAJIT_VERSION_NUM, 10000, 100, 1);
}

lua_CFunction LuaMachine::_atpanic(lua_CFunction panicf) const
{
    return lua_atpanic(_state.get(), panicf);
}

int LuaMachine::_gettop() const
{
    return lua_gettop(_state.get());
}

void LuaMachine::_settop(int idx) const
{
    lua_settop(_state.get(), idx);
}

void LuaMachine::_pushvalue(int idx) const
{
    lua_pushvalue(_state.get(), idx);
}

void LuaMachine::_remove(int idx) const
{
    lua_remove(_state.get(), idx);
}

void LuaMachine::_insert(int idx) const
{
    lua_insert(_state.get(), idx);
}

void LuaMachine::_replace(int idx) const
{
    lua_replace(_state.get(), idx);
}

int LuaMachine::_checkstack(int sz) const
{
    return lua_checkstack(_state.get(), sz);
}

void LuaMachine::_xmove(lua_State * to, int n) const
{
    lua_xmove(_state.get(), to, n);
}

int LuaMachine::_isnumber(int idx) const
{
    return lua_isnumber(_state.get(), idx);
}

int LuaMachine::_isstring(int idx) const
{
    return lua_isstring(_state.get(), idx);
}

int LuaMachine::_iscfunction(int idx) const
{
    return lua_iscfunction(_state.get(), idx);
}

int LuaMachine::_isuserdata(int idx) const
{
    return lua_isuserdata(_state.get(), idx);
}

int LuaMachine::_type(int idx) const
{
    return lua_type(_state.get(), idx);
}

char const * LuaMachine::_typename(int tp) const
{
    return lua_typename(_state.get(), tp);
}

int LuaMachine::_equal(int idx1, int idx2) const
{
    return lua_equal(_state.get(), idx1, idx2);
}

int LuaMachine::_rawequal(int idx1, int idx2) const
{
    return lua_rawequal(_state.get(), idx1, idx2);
}

int LuaMachine::_lessthan(int idx1, int idx2) const
{
    return lua_lessthan(_state.get(), idx1, idx2);
}

lua_Number LuaMachine::_tonumber(int idx) const
{
    return lua_tonumber(_state.get(), idx);
}

lua_Integer LuaMachine::_tointeger(int idx) const
{
    return lua_tointeger(_state.get(), idx);
}

int LuaMachine::_toboolean(int idx) const
{
    return lua_toboolean(_state.get(), idx);
}

char const * LuaMachine::_tolstring(int idx, size_t * len) const
{
    return lua_tolstring(_state.get(), idx, len);
}

size_t LuaMachine::_objlen(int idx) const
{
    return lua_objlen(_state.get(), idx);
}

lua_CFunction LuaMachine::_tocfunction(int idx) const
{
    return lua_tocfunction(_state.get(), idx);
}

void * LuaMachine::_touserdata(int idx) const
{
    return lua_touserdata(_state.get(), idx);
}

lua_State * LuaMachine::_tothread(int idx) const
{
    return lua_tothread(_state.get(), idx);
}

void const * LuaMachine::_topointer(int idx) const
{
    return lua_topointer(_state.get(), idx);
}

void LuaMachine::_pushnil() const
{
    lua_pushnil(_state.get());
}

void LuaMachine::_pushnumber(lua_Number n) const
{
    lua_pushnumber(_state.get(), n);
}

void LuaMachine::_pushinteger(lua_Integer n) const
{
    lua_pushinteger(_state.get(), n);
}

void LuaMachine::_pushlstring(char const * s, size_t l) const
{
    lua_pushlstring(_state.get(), s, l);
}

void LuaMachine::_pushstring(char const * s) const
{
    lua_pushstring(_state.get(), s);
}

char const * LuaMachine::_pushvfstring(char const * fmt, va_list argp) const
{
    return lua_pushvfstring(_state.get(), fmt, argp);
}

char const * LuaMachine::_pushfstring(char const * fmt, ...) const
{
    va_list args;
    va_start(args, fmt);
    char const * result = _pushvfstring(fmt, args);
    va_end(args);
    return result;
}

void LuaMachine::_pushcclosure(lua_CFunction fn, int n) const
{
    lua_pushcclosure(_state.get(), fn, n);
}

void LuaMachine::_pushboolean(int b) const
{
    lua_pushboolean(_state.get(), b);
}

void LuaMachine::_pushlightuserdata(void * p) const
{
    lua_pushlightuserdata(_state.get(), p);
}

int LuaMachine::_pushthread() const
{
    return lua_pushthread(_state.get());
}

void LuaMachine::_gettable(int idx) const
{
    lua_gettable(_state.get(), idx);
}

void LuaMachine::_getfield(int idx, char const * k) const
{
    lua_getfield(_state.get(), idx, k);
}

void LuaMachine::_rawget(int idx) const
{
    lua_rawget(_state.get(), idx);
}

void LuaMachine::_rawgeti(int idx, int n) const
{
    lua_rawgeti(_state.get(), idx, n);
}

void LuaMachine::_createtable(int narr, int nrec) const
{
    lua_createtable(_state.get(), narr, nrec);
}

void * LuaMachine::_newuserdata(size_t sz) const
{
    return lua_newuserdata(_state.get(), sz);
}

int LuaMachine::_getmetatable(int objindex) const
{
    return lua_getmetatable(_state.get(), objindex);
}

void LuaMachine::_getfenv(int idx) const
{
    lua_getfenv(_state.get(), idx);
}

void LuaMachine::_settable(int idx) const
{
    lua_settable(_state.get(), idx);
}

void LuaMachine::_setfield(int idx, char const * k) const
{
    lua_setfield(_state.get(), idx, k);
}

void LuaMachine::_rawset(int idx) const
{
    lua_rawset(_state.get(), idx);
}

void LuaMachine::_rawseti(int idx, int n) const
{
    lua_rawseti(_state.get(), idx, n);
}

int LuaMachine::_setmetatable(int objindex) const
{
    return lua_setmetatable(_state.get(), objindex);
}

int LuaMachine::_setfenv(int idx) const
{
    return lua_setfenv(_state.get(), idx);
}

void LuaMachine::_call(int nargs, int nresults) const
{
    lua_call(_state.get(), nargs, nresults);
}

int LuaMachine::_pcall(int nargs, int nresults, int errfunc) const
{
    return lua_pcall(_state.get(), nargs, nresults, errfunc);
}

int LuaMachine::_cpcall(lua_CFunction func, void * ud) const
{
    return lua_cpcall(_state.get(), func, ud);
}

int LuaMachine::_load(lua_Reader reader, void * dt, char const * chunkname) const
{
    return lua_load(_state.get(), reader, dt, chunkname);
}

int LuaMachine::_dump(lua_Writer writer, void * data) const
{
    return lua_dump(_state.get(), writer, data);
}

int LuaMachine::_yield(int nresults) const
{
    return lua_yield(_state.get(), nresults);
}

int LuaMachine::_resume(int narg) const
{
    return lua_resume(_state.get(), narg);
}

int LuaMachine::_status() const
{
    return lua_status(_state.get());
}

int LuaMachine::_gc(int what, int data) const
{
    return lua_gc(_state.get(), what, data);
}

int LuaMachine::_error() const
{
    return lua_error(_state.get());
}

int LuaMachine::_next(int idx) const
{
    return lua_next(_state.get(), idx);
}

void LuaMachine::_concat(int n) const
{
    lua_concat(_state.get(), n);
}

lua_Alloc LuaMachine::_getallocf(void ** ud) const
{
    return lua_getallocf(_state.get(), ud);
}

void LuaMachine::_setallocf(lua_Alloc f, void * ud) const
{
    lua_setallocf(_state.get(), f, ud);
}

void LuaMachine::_setlevel(lua_State * to) const
{
    lua_setlevel(_state.get(), to);
}

int LuaMachine::_getstack(int level, lua_Debug * ar) const
{
    return lua_getstack(_state.get(), level, ar);
}

int LuaMachine::_getinfo(char const * what, lua_Debug * ar) const
{
    return lua_getinfo(_state.get(), what, ar);
}

char const * LuaMachine::_getlocal(lua_Debug const * ar, int n) const
{
    return lua_getlocal(_state.get(), ar, n);
}

char const * LuaMachine::_setlocal(lua_Debug const * ar, int n) const
{
    return lua_setlocal(_state.get(), ar, n);
}

char const * LuaMachine::_getupvalue(int funcindex, int n) const
{
    return lua_getupvalue(_state.get(), funcindex, n);
}

char const * LuaMachine::_setupvalue(int funcindex, int n) const
{
    return lua_setupvalue(_state.get(), funcindex, n);
}

int LuaMachine::_sethook(lua_Hook func, int mask, int count) const
{
    return lua_sethook(_state.get(), func, mask, count);
}

lua_Hook LuaMachine::_gethook() const
{
    return lua_gethook(_state.get());
}

int LuaMachine::_gethookmask() const
{
    return lua_gethookmask(_state.get());
}

int LuaMachine::_gethookcount() const
{
    return lua_gethookcount(_state.get());
}

void * LuaMachine::_upvalueid(int idx, int n) const
{
    return lua_upvalueid(_state.get(), idx, n);
}

void LuaMachine::_upvaluejoin(int idx1, int n1, int idx2, int n2) const
{
    lua_upvaluejoin(_state.get(), idx1, n1, idx2, n2);
}

int LuaMachine::_loadx(lua_Reader reader, void * dt, char const * chunkname, char const * mode) const
{
    return lua_loadx(_state.get(), reader, dt, chunkname, mode);
}

int LuaMachine::_setmode(int idx, int mode) const
{
    return luaJIT_setmode(_state.get(), idx, mode);
}

void LuaMachine::_openlib(char const * libname, luaL_Reg const * l, int nup) const
{
    luaL_openlib(_state.get(), libname, l, nup);
}

void LuaMachine::_register(char const * libname, luaL_Reg const * l) const
{
    luaL_register(_state.get(), libname, l);
}

int LuaMachine::_getmetafield(int obj, char const * e) const
{
    return luaL_getmetafield(_state.get(), obj, e);
}

int LuaMachine::_callmeta(int obj, char const * e) const
{
    return luaL_callmeta(_state.get(), obj, e);
}

int LuaMachine::_typerror(int narg, char const * tname) const
{
    return luaL_typerror(_state.get(), narg, tname);
}

int LuaMachine::_argerror(int numarg, char const * extramsg) const
{
    return luaL_argerror(_state.get(), numarg, extramsg);
}

char const * LuaMachine::_checklstring(int numArg, size_t * l) const
{
    return luaL_checklstring(_state.get(), numArg, l);
}

char const * LuaMachine::_optlstring(int numArg, char const * def, size_t * l) const
{
    return luaL_optlstring(_state.get(), numArg, def, l);
}

lua_Number LuaMachine::_checknumber(int numArg) const
{
    return luaL_checknumber(_state.get(), numArg);
}

lua_Number LuaMachine::_optnumber(int nArg, lua_Number def) const
{
    return luaL_optnumber(_state.get(), nArg, def);
}

lua_Integer LuaMachine::_checkinteger(int numArg) const
{
    return luaL_checkinteger(_state.get(), numArg);
}

lua_Integer LuaMachine::_optinteger(int nArg, lua_Integer def) const
{
    return luaL_optinteger(_state.get(), nArg, def);
}

void LuaMachine::_checkstack(int sz, char const * msg) const
{
    luaL_checkstack(_state.get(), sz, msg);
}

void LuaMachine::_checktype(int narg, int t) const
{
    luaL_checktype(_state.get(), narg, t);
}

void LuaMachine::_checkany(int narg) const
{
    luaL_checkany(_state.get(), narg);
}

int LuaMachine::_newmetatable(char const * tname) const
{
    return luaL_newmetatable(_state.get(), tname);
}

void * LuaMachine::_checkudata(int ud, char const * tname) const
{
    return luaL_checkudata(_state.get(), ud, tname);
}

void LuaMachine::_where(int lvl) const
{
    luaL_where(_state.get(), lvl);
}

int LuaMachine::_error(char const * fmt, ...) const
{
    using namespace libtbag::string;
    va_list args;
    va_start(args, fmt);
    std::string const RESULT = vformat(fmt, DEFAULT_FORMAT_BUFFER_SIZE, args);
    va_end(args);
    return luaL_error(_state.get(), RESULT.c_str());
}

int LuaMachine::_checkoption(int narg, char const * def, char const * const lst[]) const
{
    return luaL_checkoption(_state.get(), narg, def, lst);
}

int LuaMachine::_ref(int t) const
{
    return luaL_ref(_state.get(), t);
}

void LuaMachine::_unref(int t, int ref) const
{
    luaL_unref(_state.get(), t, ref);
}

int LuaMachine::_loadfile(char const * filename) const
{
    return luaL_loadfile(_state.get(), filename);
}

int LuaMachine::_loadbuffer(char const * buff, size_t sz, char const * name) const
{
    return luaL_loadbuffer(_state.get(), buff, sz, name);
}

int LuaMachine::_loadstring(char const * s) const
{
    return luaL_loadstring(_state.get(), s);
}

char const * LuaMachine::_gsub(char const * s, char const * p, char const * r) const
{
    return luaL_gsub(_state.get(), s, p, r);
}

char const * LuaMachine::_findtable(int idx, char const * fname, int szhint) const
{
    return luaL_findtable(_state.get(), idx, fname, szhint);
}

int LuaMachine::_fileresult(int stat, char const * fname) const
{
    return luaL_fileresult(_state.get(), stat, fname);
}

int LuaMachine::_execresult(int stat) const
{
    return luaL_execresult(_state.get(), stat);
}

int LuaMachine::_loadfilex(char const * filename, char const * mode) const
{
    return luaL_loadfilex(_state.get(), filename, mode);
}

int LuaMachine::_loadbufferx(char const * buff, size_t sz, char const * name, char const * mode) const
{
    return luaL_loadbufferx(_state.get(), buff, sz, name, mode);
}

void LuaMachine::_traceback(lua_State * L1, char const * msg, int level) const
{
    return luaL_traceback(_state.get(), L1, msg, level);
}

int LuaMachine::_open_base() const
{
    return luaopen_base(_state.get());
}

int LuaMachine::_open_math() const
{
    return luaopen_math(_state.get());
}

int LuaMachine::_open_string() const
{
    return luaopen_string(_state.get());
}

int LuaMachine::_open_table() const
{
    return luaopen_table(_state.get());
}

int LuaMachine::_open_io() const
{
    return luaopen_io(_state.get());
}

int LuaMachine::_open_os() const
{
    return luaopen_os(_state.get());
}

int LuaMachine::_open_package() const
{
    return luaopen_package(_state.get());
}

int LuaMachine::_open_debug() const
{
    return luaopen_debug(_state.get());
}

int LuaMachine::_open_bit() const
{
    return luaopen_bit(_state.get());
}

int LuaMachine::_open_jit() const
{
    return luaopen_jit(_state.get());
}

int LuaMachine::_open_ffi() const
{
    return luaopen_ffi(_state.get());
}

void LuaMachine::_openlibs() const
{
    luaL_openlibs(_state.get());
}

int LuaMachine::_absindex(int i) const
{
    return lua_absindex(_state.get(), i);
}

void LuaMachine::_pop(int n) const
{
    lua_pop(_state.get(), n);
}

void LuaMachine::_newtable() const
{
    lua_newtable(_state.get());
}

void LuaMachine::_register(char const * n, lua_CFunction f) const
{
    lua_register(_state.get(), n, f);
}

void LuaMachine::_pushcfunction(lua_CFunction f) const
{
    lua_pushcfunction(_state.get(), f);
}

size_t LuaMachine::_strlen(int i) const
{
    return lua_strlen(_state.get(), i);
}

bool LuaMachine::_isfunction(int n) const
{
    return lua_isfunction(_state.get(), n);
}

bool LuaMachine::_istable(int n) const
{
    return lua_istable(_state.get(), n);
}

bool LuaMachine::_islightuserdata(int n) const
{
    return lua_islightuserdata(_state.get(), n);
}

bool LuaMachine::_isnil(int n) const
{
    return lua_isnil(_state.get(), n);
}

bool LuaMachine::_isboolean(int n) const
{
    return lua_isboolean(_state.get(), n);
}

bool LuaMachine::_isthread(int n) const
{
    return lua_isthread(_state.get(), n);
}

bool LuaMachine::_isnone(int n) const
{
    return lua_isnone(_state.get(), n);
}

bool LuaMachine::_isnoneornil(int n) const
{
    return lua_isnoneornil(_state.get(), n);
}

void LuaMachine::_pushlstring(char const * s) const
{
    lua_pushlstring(_state.get(), s, (::strlen(s)/sizeof(char))-1);
}

void LuaMachine::_setglobal(char const * s) const
{
     lua_setglobal(_state.get(), s);
}

void LuaMachine::_getglobal(char const * s) const
{
    lua_getglobal(_state.get(), s);
}

char const * LuaMachine::_tostring(int i) const
{
    return lua_tostring(_state.get(), i);
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

