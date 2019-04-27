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

#include <sstream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

LuaMachine::LuaMachine() : _state(nullptr)
{
    auto * lua = luaL_newstate();
    assert(lua != nullptr);

    _state.reset(lua, [](lua_State * l){
        lua_close(l);
    });
    assert(static_cast<bool>(_state));

    initDefaultOpenLibraries();
    initDefaultLuaPath();
    initDefaultLuaCPath();
    initDefaultTbagTable();

    if (!libtbag::script::lua::bindRayLua(lua)) {
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
    auto * lua = _state.get();
    assert(lua != nullptr);

    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, -1);
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
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_newtable(lua);
    lua_pushstring(lua, libtbag::util::getTbagVersion().toString().c_str());
    lua_setfield(lua, -2, "version");
    lua_setglobal(lua, lua_tbag_name());
}

void LuaMachine::reset()
{
    _state.reset();
}

std::string LuaMachine::getLuaPath() const
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_getfield(lua, -1, "path");
    std::string const PACKAGE_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaPaths() const
{
    return libtbag::string::splitTokens(getLuaPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaPath(std::string const & path)
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_pushstring(lua, path.c_str());
    lua_setfield(lua, -2, "path");
    lua_pop(lua, 1);
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
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_getfield(lua, -1, "cpath");
    std::string const PACKAGE_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaCPaths() const
{
    return libtbag::string::splitTokens(getLuaCPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaCPath(std::string const & path)
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_pushstring(lua, path.c_str());
    lua_setfield(lua, -2, "cpath");
    lua_pop(lua, 1);
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
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, lua_tbag_name());
    lua_getfield(lua, -1, "version");
    std::string const VERSION_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return VERSION_PATH;
}

bool LuaMachine::runScriptFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    if (!Path(path).isRegularFile()) {
        return false;
    }
    auto * lua = _state.get();
    assert(lua != nullptr);
    return luaL_dofile(lua, path.c_str()) == 0;
}

bool LuaMachine::runScript(std::string const & code)
{
    if (code.empty()) {
        return false;
    }
    auto * lua = _state.get();
    assert(lua != nullptr);
    return luaL_dostring(lua, code.c_str()) == 0;
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

int LuaMachine::getTop() const
{
    return lua_gettop(_state.get());
}

void LuaMachine::setTop(int idx) const
{
    lua_settop(_state.get(), idx);
}

void LuaMachine::pushValue(int idx) const
{
    lua_pushvalue(_state.get(), idx);
}

void LuaMachine::remove(int idx) const
{
    lua_remove(_state.get(), idx);
}

void LuaMachine::insert(int idx) const
{
    lua_insert(_state.get(), idx);
}

void LuaMachine::replace(int idx) const
{
    lua_replace(_state.get(), idx);
}

int LuaMachine::checkStack(int sz) const
{
    return lua_checkstack(_state.get(), sz);
}

int LuaMachine::isNumber(int idx) const
{
    return lua_isnumber(_state.get(), idx);
}

int LuaMachine::isString(int idx) const
{
    return lua_isstring(_state.get(), idx);
}

int LuaMachine::isCFunction(int idx) const
{
    return lua_iscfunction(_state.get(), idx);
}

int LuaMachine::isUserdata(int idx) const
{
    return lua_isuserdata(_state.get(), idx);
}

int LuaMachine::getType(int idx) const
{
    return lua_type(_state.get(), idx);
}

char const * LuaMachine::getTypename(int tp) const
{
    return lua_typename(_state.get(), tp);
}

int LuaMachine::equal(int idx1, int idx2) const
{
    return lua_equal(_state.get(), idx1, idx2);
}

int LuaMachine::rawEqual(int idx1, int idx2) const
{
    return lua_rawequal(_state.get(), idx1, idx2);
}

int LuaMachine::lessThan(int idx1, int idx2) const
{
    return lua_lessthan(_state.get(), idx1, idx2);
}

lua_Number LuaMachine::toNumber(int idx) const
{
    return lua_tonumber(_state.get(), idx);
}

lua_Integer LuaMachine::toInteger(int idx) const
{
    return lua_tointeger(_state.get(), idx);
}

int LuaMachine::toBoolean(int idx) const
{
    return lua_toboolean(_state.get(), idx);
}

char const * LuaMachine::toLString(int idx, size_t * len) const
{
    return lua_tolstring(_state.get(), idx, len);
}

size_t LuaMachine::objLen(int idx) const
{
    return lua_objlen(_state.get(), idx);
}

lua_CFunction LuaMachine::toCFunction(int idx) const
{
    return lua_tocfunction(_state.get(), idx);
}

void * LuaMachine::toUserdata(int idx) const
{
    return lua_touserdata(_state.get(), idx);
}

lua_State * LuaMachine::toThread(int idx) const
{
    return lua_tothread(_state.get(), idx);
}

void const * LuaMachine::toPointer(int idx) const
{
    return lua_topointer(_state.get(), idx);
}

void LuaMachine::pushNil() const
{
    lua_pushnil(_state.get());
}

void LuaMachine::pushNumber(lua_Number n) const
{
    lua_pushnumber(_state.get(), n);
}

void LuaMachine::pushInteger(lua_Integer n) const
{
    lua_pushinteger(_state.get(), n);
}

void LuaMachine::pushLString(char const * s, size_t l) const
{
    lua_pushlstring(_state.get(), s, l);
}

void LuaMachine::pushString(char const * s) const
{
    lua_pushstring(_state.get(), s);
}

char const * LuaMachine::pushVFString(char const * fmt, va_list argp) const
{
    return lua_pushvfstring(_state.get(), fmt, argp);
}

char const * LuaMachine::pushFString(char const * fmt, ...) const
{
    va_list args;
    va_start(args, fmt);
    char const * result = pushVFString(fmt, args);
    va_end(args);
    return result;
}

void LuaMachine::pushCClosure(lua_CFunction fn, int n) const
{
    lua_pushcclosure(_state.get(), fn, n);
}

void LuaMachine::pushBoolean(int b) const
{
    lua_pushboolean(_state.get(), b);
}

void LuaMachine::pushLightUserdata(void * p) const
{
    lua_pushlightuserdata(_state.get(), p);
}

int LuaMachine::pushThread() const
{
    return lua_pushthread(_state.get());
}

void LuaMachine::getTable(int idx) const
{
    lua_gettable(_state.get(), idx);
}

void LuaMachine::getField(int idx, char const * k) const
{
    lua_getfield(_state.get(), idx, k);
}

void LuaMachine::rawGet(int idx) const
{
    lua_rawget(_state.get(), idx);
}

void LuaMachine::rawGeti(int idx, int n) const
{
    lua_rawgeti(_state.get(), idx, n);
}

void LuaMachine::createTable(int narr, int nrec) const
{
    lua_createtable(_state.get(), narr, nrec);
}

void * LuaMachine::newUserdata(size_t sz) const
{
    return lua_newuserdata(_state.get(), sz);
}

int LuaMachine::getMetatable(int objindex) const
{
    return lua_getmetatable(_state.get(), objindex);
}

void LuaMachine::getFenv(int idx) const
{
    lua_getfenv(_state.get(), idx);
}

void LuaMachine::setTable(int idx) const
{
    lua_settable(_state.get(), idx);
}

void LuaMachine::setField(int idx, char const * k) const
{
    lua_setfield(_state.get(), idx, k);
}

void LuaMachine::rawSet(int idx) const
{
    lua_rawset(_state.get(), idx);
}

void LuaMachine::rawSeti(int idx, int n) const
{
    lua_rawseti(_state.get(), idx, n);
}

int LuaMachine::setMetatable(int objindex) const
{
    return lua_setmetatable(_state.get(), objindex);
}

int LuaMachine::setFenv(int idx) const
{
    return lua_setfenv(_state.get(), idx);
}

void LuaMachine::call(int nargs, int nresults) const
{
    lua_call(_state.get(), nargs, nresults);
}

int LuaMachine::pcall(int nargs, int nresults, int errfunc) const
{
    return lua_pcall(_state.get(), nargs, nresults, errfunc);
}

int LuaMachine::cpcall(lua_CFunction func, void * ud) const
{
    return lua_cpcall(_state.get(), func, ud);
}

int LuaMachine::load(lua_Reader reader, void * dt, char const * chunkname) const
{
    return lua_load(_state.get(), reader, dt, chunkname);
}

int LuaMachine::dump(lua_Writer writer, void * data) const
{
    return lua_dump(_state.get(), writer, data);
}

int LuaMachine::yield(int nresults) const
{
    return lua_yield(_state.get(), nresults);
}

int LuaMachine::resume(int narg) const
{
    return lua_resume(_state.get(), narg);
}

int LuaMachine::status() const
{
    return lua_status(_state.get());
}

int LuaMachine::gc(int what, int data) const
{
    return lua_gc(_state.get(), what, data);
}

int LuaMachine::error() const
{
    return lua_error(_state.get());
}

int LuaMachine::next(int idx) const
{
    return lua_next(_state.get(), idx);
}

void LuaMachine::concat(int n) const
{
    lua_concat(_state.get(), n);
}

lua_Alloc LuaMachine::getAllocf(void ** ud) const
{
    return lua_getallocf(_state.get(), ud);
}

void LuaMachine::setAllocf(lua_Alloc f, void * ud) const
{
    lua_setallocf(_state.get(), f, ud);
}

int LuaMachine::getStack(int level, lua_Debug * ar) const
{
    return lua_getstack(_state.get(), level, ar);
}

int LuaMachine::getInfo(char const * what, lua_Debug * ar) const
{
    return lua_getinfo(_state.get(), what, ar);
}

char const * LuaMachine::getLocal(lua_Debug const * ar, int n) const
{
    return lua_getlocal(_state.get(), ar, n);
}

char const * LuaMachine::setLocal(lua_Debug const * ar, int n) const
{
    return lua_setlocal(_state.get(), ar, n);
}

char const * LuaMachine::getUpValue(int funcindex, int n) const
{
    return lua_getupvalue(_state.get(), funcindex, n);
}

char const * LuaMachine::setUpValue(int funcindex, int n) const
{
    return lua_setupvalue(_state.get(), funcindex, n);
}

int LuaMachine::setHook(lua_Hook func, int mask, int count) const
{
    return lua_sethook(_state.get(), func, mask, count);
}

lua_Hook LuaMachine::getHook() const
{
    return lua_gethook(_state.get());
}

int LuaMachine::getHookMask() const
{
    return lua_gethookmask(_state.get());
}

int LuaMachine::getHookCount() const
{
    return lua_gethookcount(_state.get());
}

void LuaMachine::pop(int n) const
{
    lua_pop(_state.get(), n);
}

void LuaMachine::newTable() const
{
    lua_newtable(_state.get());
}

void LuaMachine::registerFunction(char const * n, lua_CFunction f) const
{
    lua_register(_state.get(), n, f);
}

void LuaMachine::pushCFunction(lua_CFunction f) const
{
    lua_pushcfunction(_state.get(), f);
}

size_t LuaMachine::strlen(int i) const
{
    return lua_strlen(_state.get(), i);
}

bool LuaMachine::isFunction(int n) const
{
    return lua_isfunction(_state.get(), n);
}

bool LuaMachine::isTable(int n) const
{
    return lua_istable(_state.get(), n);
}

bool LuaMachine::isLightUserdata(int n) const
{
    return lua_islightuserdata(_state.get(), n);
}

bool LuaMachine::isNil(int n) const
{
    return lua_isnil(_state.get(), n);
}

bool LuaMachine::isBoolean(int n) const
{
    return lua_isboolean(_state.get(), n);
}

bool LuaMachine::isThread(int n) const
{
    return lua_isthread(_state.get(), n);
}

bool LuaMachine::isNone(int n) const
{
    return lua_isnone(_state.get(), n);
}

bool LuaMachine::isNoneOrNil(int n) const
{
    return lua_isnoneornil(_state.get(), n);
}

void LuaMachine::pushLiteral(char const * s) const
{
    lua_pushlstring(_state.get(), s, (::strlen(s)/sizeof(char))-1);
}

void LuaMachine::setGlobal(char const * s) const
{
     lua_setglobal(_state.get(), s);
}

void LuaMachine::getGlobal(char const * s) const
{
    lua_getglobal(_state.get(), s);
}

char const * LuaMachine::toString(int i) const
{
    return lua_tostring(_state.get(), i);
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

