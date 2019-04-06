/**
 * @file   LuaPot.cpp
 * @brief  LuaPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#define luajit_c
#include <libtbag/script/LuaBypass.hpp>

#include <libtbag/tpot/apps/LuaPot.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cassert>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#if defined(TBAG_PLATFORM_UNIX_LIKE)
# include <unistd.h>
# define lua_stdin_is_tty() isatty(0)
#elif defined(TBAG_PLATFORM_WINDOWS)
# include <io.h>
# ifdef __BORLANDC__
#  define lua_stdin_is_tty() isatty(_fileno(stdin))
# else
#  define lua_stdin_is_tty() _isatty(_fileno(stdin))
# endif
#else
# define lua_stdin_is_tty() 1
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

/**
 * Backend of LuaPot class.
 *
 * @author zer0
 * @date   2019-01-30
 */
struct LuaPotBackend : private Noncopyable
{
public:
    TBAG_CONSTEXPR static char const * const PROGRAM_NAME = "LuaPot";

    TBAG_CONSTEXPR static int const FLAGS_INTERACTIVE =  1;
    TBAG_CONSTEXPR static int const FLAGS_VERSION     =  2;
    TBAG_CONSTEXPR static int const FLAGS_EXEC        =  4;
    TBAG_CONSTEXPR static int const FLAGS_OPTION      =  8;
    TBAG_CONSTEXPR static int const FLAGS_NOENV       = 16;

private:
    int         _argc;
    char **     _argv;
    lua_State * _state;
    int         _result;

public:
    LuaPotBackend(int argc, char ** argv, lua_State * L) : _argc(argc), _argv(argv), _state(L), _result(0)
    {
        assert(argc >= 1);
        assert(argv != nullptr);
        assert(L != nullptr);
    }

    ~LuaPotBackend()
    {
        // EMPTY.
    }

public:
    static int collectargs(char ** argv, int * flags)
    {
        for (int i = 1; argv[i] != nullptr; i++) {
            if (argv[i][0] != '-') {
                // Not an option?
                return i;
            }
            switch (argv[i][1]) {
            case '-':
                if (argv[i][2] != '\0') {
                    // check that argument has no extra characters at the end.
                    return -1;
                }
                return (argv[i+1] != nullptr ? i+1 : 0);
            case '\0':
                return i;
            case 'i':
                if (argv[i][2] != '\0') {
                    // check that argument has no extra characters at the end.
                    return -1;
                }
                *flags |= FLAGS_INTERACTIVE;
                TBAG_FALLTHROUGH
            case 'v':
                if (argv[i][2] != '\0') {
                    // check that argument has no extra characters at the end.
                    return -1;
                }
                *flags |= FLAGS_VERSION;
                break;
            case 'e':
                *flags |= FLAGS_EXEC;
                TBAG_FALLTHROUGH
            case 'j':
                // LuaJIT extension
                TBAG_FALLTHROUGH
            case 'l':
                *flags |= FLAGS_OPTION;
                if (argv[i][2] == '\0') {
                    i++;
                    if (argv[i] == nullptr) {
                        return -1;
                    }
                }
                break;
            case 'O':
                // LuaJIT extension
                break;
            case 'b':
                // LuaJIT extension
                if (*flags) {
                    return -1;
                }
                *flags |= FLAGS_EXEC;
                return 0;
            case 'E':
                *flags |= FLAGS_NOENV;
                break;
            default:
                // invalid option.
                return -1;
            }
        }
        return 0;
    }

    static std::string getUsageString()
    {
        return "usage: tpot lua [options]... [script [args]...].\n"
               "Available options are:\n"
               "  -e chunk  Execute string 'chunk'.\n"
               "  -l name   Require library 'name'.\n"
               "  -b ...    Save or list bytecode.\n"
               "  -j cmd    Perform LuaJIT control command.\n"
               "  -O[opt]   Control LuaJIT optimizations.\n"
               "  -i        Enter interactive mode after executing 'script'.\n"
               "  -v        Show version information.\n"
               "  -E        Ignore environment variables.\n"
               "  --        Stop handling options.\n"
               "  -         Execute stdin and stop handling options.\n";
    }

    static void l_message(const char * pname, char const * msg)
    {
        if (pname) {
            fprintf(stderr, "%s: ", pname);
        }
        fprintf(stderr, "%s\n", msg);
        fflush(stderr);
    }

    static void lstop(lua_State * L, lua_Debug * UNUSED_PARAM(ar))
    {
        lua_sethook(L, nullptr, 0, 0);
        // Avoid luaL_error -- a C hook doesn't add an extra frame.
        luaL_where(L, 0);
        lua_pushfstring(L, "%sinterrupted!", lua_tostring(L, -1));
        lua_error(L);
    }

    void laction(int i)
    {
        ::signal(i, SIG_DFL); // if another SIGINT happens before lstop, terminate process (default action)
        lua_sethook(_state, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
    }

    static int report(lua_State * L, int status)
    {
        if (status && !lua_isnil(L, -1)) {
            char const * msg = lua_tostring(L, -1);
            if (msg == nullptr) {
                msg = "(error object is not a string)";
            }
            l_message(PROGRAM_NAME, msg);
            lua_pop(L, 1);
        }
        return status;
    }

    static int traceback(lua_State *L)
    {
        if (!lua_isstring(L, 1)) {
            // Non-string error object? Try metamethod.
            if (lua_isnoneornil(L, 1) || !luaL_callmeta(L, 1, "__tostring") || !lua_isstring(L, -1)) {
                // Return non-string error object.
                return 1;
            }

            // Replace object by result of __tostring metamethod.
            lua_remove(L, 1);
        }
        luaL_traceback(L, L, lua_tostring(L, 1), 1);
        return 1;
    }

    static int docall(lua_State * L, int narg, int clear)
    {
        int base = lua_gettop(L) - narg; // function index
        lua_pushcfunction(L, traceback); // push traceback function
        lua_insert(L, base); // put it under chunk and args
        int status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
        lua_remove(L, base); // remove traceback function

        // force a complete garbage collection in case of errors
        if (status != 0) {
            lua_gc(L, LUA_GCCOLLECT, 0);
        }
        return status;
    }

    static std::string getVersionString()
    {
        return LUAJIT_VERSION;
    }

    static std::string getJitStatusString(lua_State * L)
    {
        const char * s;
        lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
        lua_getfield(L, -1, "jit"); // Get jit.* module table.
        lua_remove(L, -2);
        lua_getfield(L, -1, "status");
        lua_remove(L, -2);
        int n = lua_gettop(L);
        lua_call(L, 0, LUA_MULTRET);

        std::stringstream ss;
        ss << (lua_toboolean(L, n) ? "JIT: ON" : "JIT: OFF");
        for (n++; (s = lua_tostring(L, n)); n++) {
            ss << ' ' << s;
        }
        return ss.str();
    }

    static int getargs(lua_State * L, char ** argv, int n)
    {
        int argc = 0;
        while (argv[argc]) {
            argc++; // count total number of arguments
        }

        // number of arguments to the script
        int const NARG = argc - (n + 1);
        luaL_checkstack(L, NARG + 3, "too many arguments to script");

        int i;
        for (i = n+1; i < argc; i++) {
            lua_pushstring(L, argv[i]);
        }
        lua_createtable(L, NARG, n + 1);
        for (i = 0; i < argc; i++) {
            lua_pushstring(L, argv[i]);
            lua_rawseti(L, -2, i - n);
        }
        return NARG;
    }

    static int dofile(lua_State * L, char const * name)
    {
        int const STATUS = luaL_loadfile(L, name) || docall(L, 0, 1);
        return report(L, STATUS);
    }

    static int dostring(lua_State * L, char const * s, char const * name)
    {
        int status = luaL_loadbuffer(L, s, strlen(s), name) || docall(L, 0, 1);
        return report(L, status);
    }

    static int dolibrary(lua_State * L, char const * name)
    {
        lua_getglobal(L, "require");
        lua_pushstring(L, name);
        return report(L, docall(L, 1, 1));
    }

    static void write_prompt(lua_State * L, int firstline)
    {
        lua_getfield(L, LUA_GLOBALSINDEX, firstline ? "_PROMPT" : "_PROMPT2");
        char const * p = lua_tostring(L, -1);
        if (p == nullptr) {
            p = firstline ? LUA_PROMPT : LUA_PROMPT2;
        }
        fputs(p, stdout);
        fflush(stdout);
        lua_pop(L, 1); // remove global.
    }

    static int incomplete(lua_State * L, int status)
    {
        if (status == LUA_ERRSYNTAX) {
            size_t lmsg;
            char const * msg = lua_tolstring(L, -1, &lmsg);
            char const * tp = msg + lmsg - (sizeof(LUA_QL("<eof>")) - 1);
            if (strstr(msg, LUA_QL("<eof>")) == tp) {
                lua_pop(L, 1);
                return 1;
            }
        }
        return 0;
    }

    static int pushline(lua_State * L, int firstline)
    {
        char buf[LUA_MAXINPUT];
        write_prompt(L, firstline);
        if (fgets(buf, LUA_MAXINPUT, stdin)) {
            size_t len = strlen(buf);
            if (len > 0 && buf[len-1] == '\n') {
                buf[len-1] = '\0';
            }
            if (firstline && buf[0] == '=') {
                lua_pushfstring(L, "return %s", buf+1);
            } else {
                lua_pushstring(L, buf);
            }
            return 1;
        }
        return 0;
    }

    static int loadline(lua_State * L)
    {
        lua_settop(L, 0);
        if (!pushline(L, 1)) {
            return -1; // no input
        }

        int status;
        for (;;) {
            // repeat until gets a complete line
            status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
            if (!incomplete(L, status)) {
                break; // cannot try to add lines?
            }
            if (!pushline(L, 0)) {
                // no more input?
                return -1;
            }
            lua_pushliteral(L, "\n"); // add a new line...
            lua_insert(L, -2); // ...between the two lines
            lua_concat(L, 3); // join them
        }

        lua_remove(L, 1); // remove line
        return status;
    }

    static void dotty(lua_State * L)
    {
        int status;
        while ((status = loadline(L)) != -1) {
            if (status == 0) {
                status = docall(L, 0, 0);
            }

            report(L, status);

            if (status == 0 && lua_gettop(L) > 0) {
                // any result to print?
                lua_getglobal(L, "print");
                lua_insert(L, 1);
                if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0) {
                    char const * msg = lua_pushfstring(L, "error calling 'print' (%s)", lua_tostring(L, -1));
                    l_message(nullptr, msg);
                }
            }
        }
        lua_settop(L, 0); // clear stack
        fputs("\n", stdout);
        fflush(stdout);
    }

    static int handle_script(lua_State * L, char ** argv, int n)
    {
        int narg = getargs(L, argv, n); // collect arguments
        lua_setglobal(L, "arg");
        char const * fname = argv[n];
        if (strcmp(fname, "-") == 0 && strcmp(argv[n-1], "--") != 0) {
            fname = nullptr;
        }

        int status = luaL_loadfile(L, fname);
        lua_insert(L, -(narg+1));
        if (status == 0) {
            status = docall(L, narg, 0);
        } else {
            lua_pop(L, narg);
        }
        return report(L, status);
    }

    // Load add-on module.
    static int loadjitmodule(lua_State * L)
    {
        lua_getglobal(L, "require");
        lua_pushliteral(L, "jit.");
        lua_pushvalue(L, -3);
        lua_concat(L, 2);
        if (lua_pcall(L, 1, 1, 0)) {
            char const * msg = lua_tostring(L, -1);
            if (msg && !strncmp(msg, "module ", 7)) {
                l_message(PROGRAM_NAME, "unknown luaJIT command or jit.* modules not installed");
                return 1;
            }
            return report(L, 1);
        }
        lua_getfield(L, -1, "start");
        if (lua_isnil(L, -1)) {
            l_message(PROGRAM_NAME, "unknown luaJIT command or jit.* modules not installed");
            return 1;
        }
        lua_remove(L, -2); // Drop module table.
        return 0;
    }

    // Run command with options.
    static int runcmdopt(lua_State * L, char const * opt)
    {
        int narg = 0;
        if (opt && *opt) {
            for (;;) {  // Split arguments.
                char const * p = strchr(opt, ',');
                narg++;
                if (!p) {
                    break;
                }
                if (p == opt) {
                    lua_pushnil(L);
                } else {
                    lua_pushlstring(L, opt, (size_t)(p - opt));
                }
                opt = p + 1;
            }

            if (*opt) {
                lua_pushstring(L, opt);
            } else {
                lua_pushnil(L);
            }
        }
        return report(L, lua_pcall(L, narg, 0, 0));
    }

    // JIT engine control command: try jit library first or load add-on module.
    static int dojitcmd(lua_State * L, char const * cmd)
    {
        char const * opt = strchr(cmd, '=');
        lua_pushlstring(L, cmd, opt ? (size_t)(opt - cmd) : strlen(cmd));
        lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
        lua_getfield(L, -1, "jit"); // Get jit.* module table.
        lua_remove(L, -2);
        lua_pushvalue(L, -2);
        lua_gettable(L, -2); // Lookup library function.
        if (!lua_isfunction(L, -1)) {
            lua_pop(L, 2); // Drop non-function and jit.* table, keep module name.
            if (loadjitmodule(L)) {
                return 1;
            }
        } else {
            lua_remove(L, -2); // Drop jit.* table.
        }
        lua_remove(L, -2); // Drop module name.
        return runcmdopt(L, opt ? opt+1 : opt);
    }

    // Optimization flags.
    static int dojitopt(lua_State * L, char const * opt)
    {
        lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
        lua_getfield(L, -1, "jit.opt"); // Get jit.opt.* module table.
        lua_remove(L, -2);
        lua_getfield(L, -1, "start");
        lua_remove(L, -2);
        return runcmdopt(L, opt);
    }

    // Save or list bytecode.
    static int dobytecode(lua_State * L, char ** argv)
    {
        int narg = 0;
        lua_pushliteral(L, "bcsave");
        if (loadjitmodule(L)) {
            return 1;
        }
        if (argv[0][2]) {
            narg++;
            argv[0][1] = '-';
            lua_pushstring(L, argv[0]+1);
        }
        for (argv++; *argv != nullptr; narg++, argv++) {
            lua_pushstring(L, *argv);
        }
        return report(L, lua_pcall(L, narg, 0, 0));
    }

    static int runargs(lua_State * L, char ** argv, int n)
    {
        for (int i = 1; i < n; i++) {
            if (argv[i] == nullptr) {
                continue;
            }

            lua_assert(argv[i][0] == '-');
            switch (argv[i][1]) {
            case 'e': {
                char const * chunk = argv[i] + 2;
                if (*chunk == '\0') {
                    chunk = argv[++i];
                }
                lua_assert(chunk != nullptr);
                if (dostring(L, chunk, "=(command line)") != 0) {
                    return 1;
                }
                break;
            }
            case 'l': {
                char const * filename = argv[i] + 2;
                if (*filename == '\0') {
                    filename = argv[++i];
                }
                lua_assert(filename != nullptr);
                if (dolibrary(L, filename)) {
                    return 1; // stop if file fails
                }
                break;
            }
            case 'j': {
                // LuaJIT extension
                char const * cmd = argv[i] + 2;
                if (*cmd == '\0') {
                    cmd = argv[++i];
                }
                lua_assert(cmd != nullptr);
                if (dojitcmd(L, cmd)) {
                    return 1;
                }
                break;
            }
            case 'O': {
                // LuaJIT extension
                if (dojitopt(L, argv[i] + 2)) {
                    return 1;
                }
                break;
            }
            case 'b': {
                // LuaJIT extension
                return dobytecode(L, argv+i);
            }
            default:
                break;
            }
        }
        return 0;
    }

    static int handle_luainit(lua_State * L)
    {
        auto const * init = ::getenv(LUA_INIT);
        if (init == nullptr) {
            return 0;  /* status OK */
        } else if (init[0] == '@') {
            return dofile(L, init+1);
        } else {
            return dostring(L, init, "=" LUA_INIT);
        }
    }

public:
    int run()
    {
        if (_state == nullptr) {
            l_message(_argv[0], "cannot create state: not enough memory");
            return EXIT_FAILURE;
        }

        int const STATUS = lua_cpcall(_state, LuaPotBackend::luac_pmain_cb, this);
        report(_state, STATUS);

        // lua_close(_state); // It is managed in the Sol2 class.

        return (STATUS || _result) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    static int luac_pmain_cb(lua_State * L)
    {
        assert(lua_isuserdata(L, -1));
        auto * backend = static_cast<LuaPotBackend*>(lua_touserdata(L, -1));
        assert(backend != nullptr);
        return backend->luac_pmain(L);
    }

    int luac_pmain(lua_State * L)
    {
        int flags = 0;
        int script = collectargs(_argv, &flags);
        if (script < 0) {
            // invalid args?
            std::cerr << getUsageString() << std::endl;
            _result = 1;
            return 0;
        }

        if ((flags & FLAGS_NOENV)) {
            lua_pushboolean(L, 1);
            lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
        }

        // lua_gc(L, LUA_GCSTOP, 0); // stop collector during initialization.
        // luaL_openlibs(L); // open libraries
        // lua_gc(L, LUA_GCRESTART, -1);

        if (!(flags & FLAGS_NOENV)) {
            _result = handle_luainit(L);
            if (_result != 0) {
                return 0;
            }
        }

        if ((flags & FLAGS_VERSION)) {
            std::cout << getVersionString() << std::endl;
        }

        _result = runargs(L, _argv, (script > 0) ? script : _argc);
        if (_result != 0) {
            return 0;
        }

        if (script) {
            _result = handle_script(L, _argv, script);
            if (_result != 0) {
                return 0;
            }
        }

        if ((flags & FLAGS_INTERACTIVE)) {
            std::cout << getJitStatusString(L) << std::endl;
            dotty(L);
        } else if (script == 0 && !(flags & (FLAGS_EXEC|FLAGS_VERSION))) {
            if (lua_stdin_is_tty()) {
                std::cout << getVersionString() << std::endl
                          << getJitStatusString(L) << std::endl;
                dotty(L);
            } else {
                dofile(L, nullptr);  // executes stdin as a file.
            }
        }
        return 0;
    }
};

// ---------------------
// LuaPot implementation
// ---------------------

LuaPot::LuaPot()
{
    // EMPTY.
}

LuaPot::~LuaPot()
{
    // EMPTY.
}

char const * const LuaPot::name() TBAG_NOEXCEPT
{
    return "lua";
}

char const * const LuaPot::remarks() TBAG_NOEXCEPT
{
    return "Lua Interpreter";
}

char const * const LuaPot::help() TBAG_NOEXCEPT
{
    return "usage: %s lua [options]... [script [args]...].\n"
           "Available options are:\n"
           "  -e chunk  Execute string 'chunk' .\n"
           "  -l name   Require library 'name' .\n"
           "  -b ...    Save or list bytecode.\n"
           "  -j cmd    Perform LuaJIT control command.\n"
           "  -O[opt]   Control LuaJIT optimizations.\n"
           "  -i        Enter interactive mode after executing 'script'.\n"
           "  -v        Show version information.\n"
           "  -E        Ignore environment variables.\n"
           "  --        Stop handling options.\n"
           "  -         Execute stdin and stop handling options.\n";
}

char const * const LuaPot::getName() const TBAG_NOEXCEPT
{
    return name();
}

char const * const LuaPot::getRemarks() const TBAG_NOEXCEPT
{
    return remarks();
}

char const * const LuaPot::getHelp() const TBAG_NOEXCEPT
{
    return help();
}

int LuaPot::onMain()
{
    auto arg0 = libtbag::filesystem::Path::getExePath().toString();
    assert(!arg0.empty());

    auto original_args = args();
    if (!original_args.empty() && original_args[0] == getName()) {
        // If the 'lua' mode name argument is exists, erase it.
        original_args.erase(original_args.begin());
    }
    auto const ARGS_SIZE = original_args.size();

    std::vector<char*> argv;
    argv.push_back(&(arg0[0]));
    for (std::size_t i = 0; i < ARGS_SIZE; ++i) {
        argv.push_back(&(original_args[i][0]));
    }
    argv.push_back(nullptr);
    assert(!argv.empty());

    tDLogIfD(params().verbose && !original_args.empty(), "LuaPot::onMain() Passed arguments: {}",
             libtbag::string::mergeTokens(original_args));
    return LuaPotBackend(static_cast<int>(argv.size()), argv.data(), params().storage->lua->lua_state()).run();
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

