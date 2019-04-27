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

// basic stack manipulation
public:
    int getTop() const;
    void setTop(int idx) const;
    void pushValue(int idx) const;
    void remove(int idx) const;
    void insert(int idx) const;
    void replace(int idx) const;
    int checkStack(int sz) const;

// access functions (stack -> C)
public:
    int isNumber(int idx) const;
    int isString(int idx) const;
    int isCFunction(int idx) const;
    int isUserdata(int idx) const;
    int getType(int idx) const;
    char const * getTypename(int tp) const;

    int equal(int idx1, int idx2) const;
    int rawEqual(int idx1, int idx2) const;
    int lessThan(int idx1, int idx2) const;

    lua_Number toNumber(int idx) const;
    lua_Integer toInteger(int idx) const;
    int toBoolean(int idx) const;
    char const * toLString(int idx, size_t * len) const;
    size_t objLen(int idx) const;
    lua_CFunction toCFunction(int idx) const;
    void * toUserdata(int idx) const;
    lua_State * toThread(int idx) const;
    void const * toPointer(int idx) const;

// push functions (C -> stack)
public:
    void pushNil() const;
    void pushNumber(lua_Number n) const;
    void pushInteger(lua_Integer n) const;
    void pushLString(char const * s, size_t l) const;
    void pushString(char const * s) const;
    char const * pushVFString(char const * fmt, va_list argp) const;
    char const * pushFString(char const * fmt, ...) const;
    void pushCClosure(lua_CFunction fn, int n) const;
    void pushBoolean(int b) const;
    void pushLightUserdata(void * p) const;
    int pushThread() const;

// get functions (Lua -> stack)
public:
    void getTable(int idx) const;
    void getField(int idx, char const * k) const;
    void rawGet(int idx) const;
    void rawGeti(int idx, int n) const;
    void createTable(int narr, int nrec) const;
    void * newUserdata(size_t sz) const;
    int getMetatable(int objindex) const;
    void getFenv(int idx) const;

// set functions (stack -> Lua)
public:
    void setTable(int idx) const;
    void setField(int idx, char const * k) const;
    void rawSet(int idx) const;
    void rawSeti(int idx, int n) const;
    int setMetatable(int objindex) const;
    int setFenv(int idx) const;

// load and run Lua code
public:
    void call(int nargs, int nresults) const;
    int pcall(int nargs, int nresults, int errfunc) const;
    int cpcall(lua_CFunction func, void * ud) const;
    int load(lua_Reader reader, void * dt, char const * chunkname) const;
    int dump(lua_Writer writer, void * data) const;

// coroutine functions
public:
    int yield(int nresults) const;
    int resume(int narg) const;
    int status() const;

// garbage-collection function
public:
    int gc(int what, int data) const;

// miscellaneous functions
public:
    int error() const;
    int next(int idx) const;
    void concat(int n) const;
    lua_Alloc getAllocf(void ** ud) const;
    void setAllocf(lua_Alloc f, void * ud) const;

// debug api
public:
    int getStack(int level, lua_Debug * ar) const;
    int getInfo(char const * what, lua_Debug * ar) const;
    char const * getLocal(lua_Debug const * ar, int n) const;
    char const * setLocal(lua_Debug const * ar, int n) const;
    char const * getUpValue(int funcindex, int n) const;
    char const * setUpValue(int funcindex, int n) const;
    int setHook(lua_Hook func, int mask, int count) const;
    lua_Hook getHook() const;
    int getHookMask() const;
    int getHookCount() const;

// some useful macros
public:
    void pop(int n) const;
    void newTable() const;
    void registerFunction(char const * n, lua_CFunction f) const;
    void pushCFunction(lua_CFunction f) const;
    size_t strlen(int i) const;

    bool isFunction(int n) const;
    bool isTable(int n) const;
    bool isLightUserdata(int n) const;
    bool isNil(int n) const;
    bool isBoolean(int n) const;
    bool isThread(int n) const;
    bool isNone(int n) const;
    bool isNoneOrNil(int n) const;

    void pushLiteral(char const * s) const;
    void setGlobal(char const * s) const;
    void getGlobal(char const * s) const;
    char const * toString(int i) const;
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

