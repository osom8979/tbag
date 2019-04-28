/**
 * @file   LuaMachine.hpp
 * @brief  LuaMachine class prototype.
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/util/Version.hpp>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * LuaMachine class prototype.
 *
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */
class TBAG_API LuaMachine
{
public:
    using State = lua_State;
    using SharedState = std::shared_ptr<State>;
    using Version = libtbag::util::Version;

public:
    TBAG_CONSTEXPR static char const * const TABLE_SPLIT_STRING = ".";

public:
    TBAG_CONSTEXPR static char const * const lua_tbag_name() TBAG_NOEXCEPT
    { return "tbag"; }

private:
    SharedState _state;

public:
    LuaMachine();
    LuaMachine(std::nullptr_t) TBAG_NOEXCEPT;
    LuaMachine(LuaMachine const & obj) TBAG_NOEXCEPT;
    LuaMachine(LuaMachine && obj) TBAG_NOEXCEPT;
    ~LuaMachine();

public:
    LuaMachine & operator =(LuaMachine const & obj) TBAG_NOEXCEPT;
    LuaMachine & operator =(LuaMachine && obj) TBAG_NOEXCEPT;

public:
    void copy(LuaMachine const & obj) TBAG_NOEXCEPT;
    void swap(LuaMachine & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(LuaMachine & lh, LuaMachine & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_state); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline lua_State * state() TBAG_NOEXCEPT
    { return _state.get(); }
    inline lua_State const * state() const TBAG_NOEXCEPT
    { return _state.get(); }

public:
    static std::string getDefaultLuaPath();
    static std::string getDefaultLuaCPath();

private:
    void initDefaultOpenLibraries();
    void initDefaultLuaPath();
    void initDefaultLuaCPath();
    void initDefaultTbagTable();

public:
    void reset();

public:
    std::string getLuaPath() const;
    std::vector<std::string> getLuaPaths() const;
    void setLuaPath(std::string const & path);
    bool appendLuaPath(std::string const & path);

public:
    std::string getLuaCPath() const;
    std::vector<std::string> getLuaCPaths() const;
    void setLuaCPath(std::string const & path);
    bool appendLuaCPath(std::string const & path);

public:
    std::string getTbagVersion() const;

public:
    bool runScriptFile(std::string const & path);
    bool runScript(std::string const & code);

public:
    std::string findScriptPath(std::string const & filename, bool include_working = true) const;

public:
    Version getLuaVersion() const;
    Version getLuaJITVersion() const;

public:
    // clang-format off
    lua_CFunction _atpanic          (lua_CFunction panicf) const;
    int           _gettop           () const;
    void          _settop           (int idx) const;
    void          _pushvalue        (int idx) const;
    void          _remove           (int idx) const;
    void          _insert           (int idx) const;
    void          _replace          (int idx) const;
    int           _checkstack       (int sz) const;
    void          _xmove            (lua_State * to, int n) const;
    int           _isnumber         (int idx) const;
    int           _isstring         (int idx) const;
    int           _iscfunction      (int idx) const;
    int           _isuserdata       (int idx) const;
    int           _type             (int idx) const;
    char const *  _typename         (int tp) const;
    int           _equal            (int idx1, int idx2) const;
    int           _rawequal         (int idx1, int idx2) const;
    int           _lessthan         (int idx1, int idx2) const;
    lua_Number    _tonumber         (int idx) const;
    lua_Integer   _tointeger        (int idx) const;
    int           _toboolean        (int idx) const;
    char const *  _tolstring        (int idx, size_t *len) const;
    size_t        _objlen           (int idx) const;
    lua_CFunction _tocfunction      (int idx) const;
    void *        _touserdata       (int idx) const;
    lua_State *   _tothread         (int idx) const;
    void const *  _topointer        (int idx) const;
    void          _pushnil          () const;
    void          _pushnumber       (lua_Number n) const;
    void          _pushinteger      (lua_Integer n) const;
    void          _pushlstring      (char const * s, size_t l) const;
    void          _pushstring       (char const * s) const;
    char const *  _pushvfstring     (char const * fmt, va_list argp) const;
    char const *  _pushfstring      (char const * fmt, ...) const;
    void          _pushcclosure     (lua_CFunction fn, int n) const;
    void          _pushboolean      (int b) const;
    void          _pushlightuserdata(void * p) const;
    int           _pushthread       () const;
    void          _gettable         (int idx) const;
    void          _getfield         (int idx, char const * k) const;
    void          _rawget           (int idx) const;
    void          _rawgeti          (int idx, int n) const;
    void          _createtable      (int narr, int nrec) const;
    void *        _newuserdata      (size_t sz) const;
    int           _getmetatable     (int objindex) const;
    void          _getfenv          (int idx) const;
    void          _settable         (int idx) const;
    void          _setfield         (int idx, char const * k) const;
    void          _rawset           (int idx) const;
    void          _rawseti          (int idx, int n) const;
    int           _setmetatable     (int objindex) const;
    int           _setfenv          (int idx) const;
    void          _call             (int nargs, int nresults) const;
    int           _pcall            (int nargs, int nresults, int errfunc) const;
    int           _cpcall           (lua_CFunction func, void * ud) const;
    int           _load             (lua_Reader reader, void * dt, char const * chunkname) const;
    int           _dump             (lua_Writer writer, void * data) const;
    int           _yield            (int nresults) const;
    int           _resume           (int narg) const;
    int           _status           () const;
    int           _gc               (int what, int data) const;
    int           _error            () const;
    int           _next             (int idx) const;
    void          _concat           (int n) const;
    lua_Alloc     _getallocf        (void ** ud) const;
    void          _setallocf        (lua_Alloc f, void * ud) const;
    void          _setlevel         (lua_State * to) const;
    int           _getstack         (int level, lua_Debug * ar) const;
    int           _getinfo          (char const * what, lua_Debug * ar) const;
    char const *  _getlocal         (lua_Debug const * ar, int n) const;
    char const *  _setlocal         (lua_Debug const * ar, int n) const;
    char const *  _getupvalue       (int funcindex, int n) const;
    char const *  _setupvalue       (int funcindex, int n) const;
    int           _sethook          (lua_Hook func, int mask, int count) const;
    lua_Hook      _gethook          () const;
    int           _gethookmask      () const;
    int           _gethookcount     () const;
    void *        _upvalueid        (int idx, int n) const;
    void          _upvaluejoin      (int idx1, int n1, int idx2, int n2) const;
    int           _loadx            (lua_Reader reader, void * dt, char const * chunkname, char const * mode) const;
    int           _setmode          (int idx, int mode) const;
    void          _openlib          (char const * libname, luaL_Reg const * l, int nup) const;
    void          _register         (char const * libname, luaL_Reg const * l) const;
    int           _getmetafield     (int obj, char const * e) const;
    int           _callmeta         (int obj, char const * e) const;
    int           _typerror         (int narg, char const * tname) const;
    int           _argerror         (int numarg, char const * extramsg) const;
    char const *  _checklstring     (int numArg, size_t * l) const;
    char const *  _optlstring       (int numArg, char const * def, size_t * l) const;
    lua_Number    _checknumber      (int numArg) const;
    lua_Number    _optnumber        (int nArg, lua_Number def) const;
    lua_Integer   _checkinteger     (int numArg) const;
    lua_Integer   _optinteger       (int nArg, lua_Integer def) const;
    void          _checkstack       (int sz, char const * msg) const;
    void          _checktype        (int narg, int t) const;
    void          _checkany         (int narg) const;
    int           _newmetatable     (char const * tname) const;
    void *        _checkudata       (int ud, char const * tname) const;
    void          _where            (int lvl) const;
    int           _error            (char const * fmt, ...) const;
    int           _checkoption      (int narg, char const * def, char const * const lst[]) const;
    int           _ref              (int t) const;
    void          _unref            (int t, int ref) const;
    int           _loadfile         (char const * filename) const;
    int           _loadbuffer       (char const * buff, size_t sz, char const * name) const;
    int           _loadstring       (char const * s) const;
    char const *  _gsub             (char const * s, char const * p, char const * r) const;
    char const *  _findtable        (int idx, char const * fname, int szhint) const;
    int           _fileresult       (int stat, char const * fname) const;
    int           _execresult       (int stat) const;
    int           _loadfilex        (char const * filename, char const * mode) const;
    int           _loadbufferx      (char const * buff, size_t sz, char const * name, char const * mode) const;
    void          _traceback        (lua_State * L1, char const * msg, int level) const;
    int           _open_base        () const;
    int           _open_math        () const;
    int           _open_string      () const;
    int           _open_table       () const;
    int           _open_io          () const;
    int           _open_os          () const;
    int           _open_package     () const;
    int           _open_debug       () const;
    int           _open_bit         () const;
    int           _open_jit         () const;
    int           _open_ffi         () const;
    void          _openlibs         () const;
    int           _absindex         (int i) const;
    void          _pop              (int n) const;
    void          _newtable         () const;
    void          _register         (char const * n, lua_CFunction f) const;
    void          _pushcfunction    (lua_CFunction f) const;
    size_t        _strlen           (int i) const;
    bool          _isfunction       (int n) const;
    bool          _istable          (int n) const;
    bool          _islightuserdata  (int n) const;
    bool          _isnil            (int n) const;
    bool          _isboolean        (int n) const;
    bool          _isthread         (int n) const;
    bool          _isnone           (int n) const;
    bool          _isnoneornil      (int n) const;
    void          _pushlstring      (char const * s) const;
    void          _setglobal        (char const * s) const;
    void          _getglobal        (char const * s) const;
    char const *  _tostring         (int i) const;
    // clang-format on
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

