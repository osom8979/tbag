/**
 * @file   LuaMain.cpp
 * @brief  LuaMain class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#define luajit_c
#include <libtbag/script/LuaMain.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/script/LuaMachine.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cassert>

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

namespace script {

TBAG_CONSTEXPR static unsigned const FLAG_INTERACTIVE =  1;
TBAG_CONSTEXPR static unsigned const FLAG_VERSION =  2;
TBAG_CONSTEXPR static unsigned const FLAG_EXEC =  4;
TBAG_CONSTEXPR static unsigned const FLAG_OPTION =  8;
TBAG_CONSTEXPR static unsigned const FLAG_NO_ENV = 16;

TBAG_CONSTEXPR static char const * const USAGE = R"(usage: lua [options]... [script [args]...].
Available options are:
  -e chunk  Execute string 'chunk'.
  -l name   Require library 'name'.
  -b ...    Save or list bytecode.
  -j cmd    Perform LuaJIT control command.
  -O[opt]   Control LuaJIT optimizations.
  -i        Enter interactive mode after executing 'script'.
  -v        Show version information.
  -E        Ignore environment variables.
  --        Stop handling options.
  -         Execute stdin and stop handling options.)";

static int collect_args(char ** argv, unsigned * flags)
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
            *flags |= FLAG_INTERACTIVE;
            TBAG_FALLTHROUGH
        case 'v':
            if (argv[i][2] != '\0') {
                return -1; // check that argument has no extra characters at the end.
            }
            *flags |= FLAG_VERSION;
            break;

        case 'e':
            *flags |= FLAG_EXEC;
            TBAG_FALLTHROUGH
        case 'j':
            // LuaJIT extension
            TBAG_FALLTHROUGH
        case 'l':
            *flags |= FLAG_OPTION;
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
            *flags |= FLAG_EXEC;
            return 0;

        case 'E':
            *flags |= FLAG_NO_ENV;
            break;

        default:
            // invalid option.
            return -1;
        }
    }
    return 0;
}

static void message(char const * msg)
{
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
}

static void stop_hook(lua_State * L, lua_Debug * UNUSED_PARAM(ar))
{
    lua_sethook(L, nullptr, 0, 0);
    // Avoid luaL_error -- a C hook doesn't add an extra frame.
    luaL_where(L, 0);
    lua_pushfstring(L, "%s interrupted!", lua_tostring(L, -1));
    lua_error(L);
}

static void signal_action(lua_State * L, int i)
{
    // if another SIGINT happens before stop_hook, terminate process (default action)
    ::signal(i, SIG_DFL);
    lua_sethook(L, stop_hook, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);  // NOLINT
}

static int report(lua_State * L, int status)
{
    if (status && !lua_isnil(L, -1)) {
        char const * msg = lua_tostring(L, -1);
        if (msg == nullptr) {
            msg = "(error object is not a string)";
        }
        message(msg);
        lua_pop(L, 1);
    }
    return status;
}

static int traceback(lua_State * L)
{
    if (!lua_isstring(L, 1)) {
        // Non-string error object? Try meta method.
        if (lua_isnoneornil(L, 1) || !luaL_callmeta(L, 1, "__tostring") || !lua_isstring(L, -1)) {
            // Return non-string error object.
            return 1;
        }
        // Replace object by result of __tostring meta method.
        lua_remove(L, 1);
    }
    luaL_traceback(L, L, lua_tostring(L, 1), 1);
    return 1;
}

static int do_call(lua_State * L, int narg, int clear)
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

static void print_jit_status(lua_State * L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
    lua_getfield(L, -1, "jit"); // Get jit.* module table.
    lua_remove(L, -2);
    lua_getfield(L, -1, "status");
    lua_remove(L, -2);
    int n = lua_gettop(L);
    lua_call(L, 0, LUA_MULTRET);

    char const * s;
    fprintf(stdout, "%s", (lua_toboolean(L, n) ? "JIT: ON" : "JIT: OFF"));
    for (n++; (s = lua_tostring(L, n)); n++) {
        fprintf(stdout, " %s", s);
    }
    fputs("\n", stdout);
    fflush(stdout);
}

static int getargs(lua_State * L, char ** argv, int n)
{
    int argc = 0;
    while (argv[argc]) {
        // count total number of arguments
        argc++;
    }

    // number of arguments to the script
    int const narg = argc - (n + 1);
    luaL_checkstack(L, narg + 3, "too many arguments to script");

    int i;
    for (i = n+1; i < argc; i++) {
        lua_pushstring(L, argv[i]);
    }
    lua_createtable(L, narg, n + 1);
    for (i = 0; i < argc; i++) {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i - n);
    }
    return narg;
}

static int do_file(lua_State * L, char const * name)
{
    int const STATUS = luaL_loadfile(L, name) || do_call(L, 0, 1);
    return report(L, STATUS);
}

static int do_string(lua_State * L, char const * s, char const * name)
{
    int status = luaL_loadbuffer(L, s, strlen(s), name) || do_call(L, 0, 1);
    return report(L, status);
}

static int do_library(lua_State * L, char const * name)
{
    lua_getglobal(L, "require");
    lua_pushstring(L, name);
    return report(L, do_call(L, 1, 1));
}

static void write_prompt(lua_State * L, int first_line)
{
    lua_getfield(L, LUA_GLOBALSINDEX, first_line ? "_PROMPT" : "_PROMPT2");
    char const * p = lua_tostring(L, -1);
    if (p == nullptr) {
        p = first_line ? LUA_PROMPT : LUA_PROMPT2;
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

static int push_line(lua_State * L, int first_line)
{
    char buf[LUA_MAXINPUT];
    write_prompt(L, first_line);
    if (fgets(buf, LUA_MAXINPUT, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') {
            buf[len-1] = '\0';
        }
        if (first_line && buf[0] == '=') {
            lua_pushfstring(L, "return %s", buf+1);
        } else {
            lua_pushstring(L, buf);
        }
        return 1;
    }
    return 0;
}

static int load_line(lua_State * L)
{
    lua_settop(L, 0);
    if (!push_line(L, 1)) {
        return -1; // no input
    }

    int status;
    for (;;) {
        // repeat until gets a complete line
        status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
        if (!incomplete(L, status)) {
            break; // cannot try to add lines?
        }
        if (!push_line(L, 0)) {
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

static void do_tty(lua_State * L)
{
    int status;
    while ((status = load_line(L)) != -1) {
        if (status == 0) {
            status = do_call(L, 0, 0);
        }

        report(L, status);

        if (status == 0 && lua_gettop(L) > 0) {
            // any result to print?
            lua_getglobal(L, "print");
            lua_insert(L, 1);
            if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0) {
                char const * msg = lua_pushfstring(L, "error calling 'print' (%s)", lua_tostring(L, -1));
                message(msg);
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
        status = do_call(L, narg, 0);
    } else {
        lua_pop(L, narg);
    }
    return report(L, status);
}

// Load add-on module.
static int load_jit_module(lua_State * L)
{
    lua_getglobal(L, "require");
    lua_pushliteral(L, "jit.");
    lua_pushvalue(L, -3);
    lua_concat(L, 2);
    if (lua_pcall(L, 1, 1, 0)) {
        char const * msg = lua_tostring(L, -1);
        if (msg && !strncmp(msg, "module ", 7)) {
            message("unknown luaJIT command or jit.* modules not installed");
            return 1;
        }
        return report(L, 1);
    }
    lua_getfield(L, -1, "start");
    if (lua_isnil(L, -1)) {
        message("unknown luaJIT command or jit.* modules not installed");
        return 1;
    }
    lua_remove(L, -2); // Drop module table.
    return 0;
}

// Run command with options.
static int run_command_with_options(lua_State * L, char const * opt)
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
static int do_jit_cmd(lua_State * L, char const * cmd)
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
        if (load_jit_module(L)) {
            return 1;
        }
    } else {
        lua_remove(L, -2); // Drop jit.* table.
    }
    lua_remove(L, -2); // Drop module name.
    return run_command_with_options(L, opt ? opt+1 : opt);
}

// Optimization flags.
static int do_jit_opt(lua_State * L, char const * opt)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
    lua_getfield(L, -1, "jit.opt"); // Get jit.opt.* module table.
    lua_remove(L, -2);
    lua_getfield(L, -1, "start");
    lua_remove(L, -2);
    return run_command_with_options(L, opt);
}

// Save or list bytecode.
static int do_byte_code(lua_State * L, char ** argv)
{
    int narg = 0;
    lua_pushliteral(L, "bcsave");
    if (load_jit_module(L)) {
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

static int run_args(lua_State * L, char ** argv, int n)
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
            if (do_string(L, chunk, "=(command line)") != 0) {
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
            if (do_library(L, filename)) {
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
            if (do_jit_cmd(L, cmd)) {
                return 1;
            }
            break;
        }

        case 'O': {
            // LuaJIT extension
            if (do_jit_opt(L, argv[i] + 2)) {
                return 1;
            }
            break;
        }

        case 'b': {
            // LuaJIT extension
            return do_byte_code(L, argv+i);
        }

        default:
            break;
        }
    }
    return 0;
}

static int handle_lua_init(lua_State * L)
{
    auto const * init = ::getenv(LUA_INIT);
    if (init == nullptr) {
        return 0;  /* status OK */
    } else if (init[0] == '@') {
        return do_file(L, init+1);
    } else {
        return do_string(L, init, "=" LUA_INIT);
    }
}

/**
 * Backend of LuaMain class.
 *
 * @author zer0
 * @date   2019-01-30
 */
struct LuaMain TBAG_FINAL : private Noncopyable
{
public:
    using LuaMachine = libtbag::script::LuaMachine;

private:
    LuaMachine _lua;

private:
    int _argc;
    char ** _argv;

private:
    int _result;

public:
    LuaMain(int argc, char ** argv)
            : _argc(argc), _argv(argv), _result(0)
    {
        assert(argc >= 1);
        assert(argv != nullptr);
        _lua.initDefault();
    }

    ~LuaMain()
    {
        // EMPTY.
    }

private:
    static int luac_pmain_cb(lua_State * L)
    {
        assert(lua_isuserdata(L, -1));
        auto * backend = static_cast<LuaMain*>(lua_touserdata(L, -1));
        assert(backend != nullptr);
        return backend->luac_pmain(L);
    }

public:
    int run()
    {
        if (_lua.state() == nullptr) {
            message("cannot create state: not enough memory");
            return EXIT_FAILURE;
        }

        int const STATUS = lua_cpcall(_lua.state(), LuaMain::luac_pmain_cb, this);
        report(_lua.state(), STATUS);

        // lua_close(_lua.state());

        return (STATUS || _result) ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    int luac_pmain(lua_State * L)
    {
        unsigned flags = 0;
        int script = collect_args(_argv, &flags);
        if (script < 0) {
            fprintf(stderr, "%s\n", USAGE);
            fflush(stderr);
            _result = 1;
            return 0;
        }

        if ((flags & FLAG_NO_ENV)) {
            lua_pushboolean(L, 1);
            lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
        }

        // lua_gc(L, LUA_GCSTOP, 0); // stop collector during initialization.
        // luaL_openlibs(L); // open libraries
        // lua_gc(L, LUA_GCRESTART, -1);

        if (!(flags & FLAG_NO_ENV)) {
            _result = handle_lua_init(L);
            if (_result != 0) {
                return 0;
            }
        }

        if ((flags & FLAG_VERSION)) {
            fprintf(stdout, "%s\n", LUAJIT_VERSION);
            fflush(stdout);
        }

        _result = run_args(L, _argv, (script > 0) ? script : _argc);
        if (_result != 0) {
            return 0;
        }

        if (script) {
            _result = handle_script(L, _argv, script);
            if (_result != 0) {
                return 0;
            }
        }

        if ((flags & FLAG_INTERACTIVE)) {
            print_jit_status(L);
            do_tty(L);
        } else if (script == 0 && !(flags & (FLAG_EXEC|FLAG_VERSION))) {
            if (lua_stdin_is_tty()) {
                fprintf(stdout, "%s\n", LUAJIT_VERSION);
                print_jit_status(L);
                do_tty(L);
            } else {
                // executes stdin as a file.
                do_file(L, nullptr);
            }
        }

        return 0;
    }
};

int runLuaJit(int argc, char ** argv)
{
    return LuaMain(argc, argv).run();
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

