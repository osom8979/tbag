/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2017-02-03
 */

#include <libtbag/uvpp/Process.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <uv.h>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

using CharPtrs = std::vector<char*>;
using Stdios = std::vector<uv_stdio_container_t>;

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_exit_cb__(uv_process_t * handle, int64_t exit_status, int term_signal)
{
    // Type definition for callback passed in uv_process_options_t
    // which will indicate the exit status and the signal that caused the process to terminate, if any.
    Process * p = static_cast<Process*>(handle->data);
    if (p == nullptr) {
        tDLogE("__global_uv_exit_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(p)) {
        tDLogE("__global_uv_exit_cb__() handle.data is deleted.");
    } else {
        p->onExit(exit_status, term_signal);
    }
}

/* inline */ namespace impl {

static bool updateOptions(Process::Options & options,
                          CharPtrs & args,
                          CharPtrs & envs,
                          Stdios & stdios,
                          uv_process_options_t & native)
{
    std::size_t const ARGS_SIZE = options.args.size();
    std::size_t const ENVS_SIZE = options.envs.size();

    args.clear();
    envs.clear();
    args.resize(ARGS_SIZE + 2); // [EXE_PATH], [...], [NULL]
    envs.resize(ENVS_SIZE + 1); // [...], [NULL]

    // First: exe file path.
    args[0] = &options.file[0];

    // Argument pointers.
    for (std::size_t index = 0; index < ARGS_SIZE; ++index) {
        args[index + 1] = &options.args[index][0];
    }

    // Environment pointers.
    for (std::size_t index = 0; index < ENVS_SIZE; ++index) {
        envs[index] = &options.envs[index][0];
    }

    // Last: NULL pointer.
    args[ARGS_SIZE + 1] = nullptr;
    envs[ENVS_SIZE + 0] = nullptr;

    native.exit_cb = __global_uv_exit_cb__;

    // Path pointing to the program to be executed.
    native.file = options.file.c_str();
    // If non-null this represents a directory the subprocess should execute
    // in. Stands for current working directory.
    native.cwd  = options.cwd.c_str();

    // Command line arguments. args[0] should be the path to the program. On
    // Windows this uses CreateProcess which concatenates the arguments into a
    // string this can cause some strange errors. See the
    // UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS flag on uv_process_flags.
    native.args = &args[0];
    // This will be set as the environ variable in the subprocess. If this is
    // NULL then the parents environ will be used.
    native.env  = &envs[0];

    static_assert(sizeof(native.uid) == sizeof(options.uid), "Not equal uid size type.");
    static_assert(sizeof(native.gid) == sizeof(options.gid), "Not equal gid size type.");

    // Libuv can change the child process user/group id. This happens only when
    // the appropriate bits are set in the flags fields.
    // Note: This is not supported on Windows, uv_spawn() will fail and set the error to UV_ENOTSUP.
    native.uid = options.uid;
    native.gid = options.gid;

    // Various flags that control how uv_spawn() behaves. See the definition of
    // 'enum uv_process_flags' below.
    native.flags = 0;
    native.flags |= (options.setuid ? UV_PROCESS_SETUID : 0);
    native.flags |= (options.setgid ? UV_PROCESS_SETGID : 0);
    // Do not wrap any arguments in quotes, or perform any other escaping, when
    // converting the argument list into a command line string. This option is
    // only meaningful on Windows systems. On Unix it is silently ignored.
    native.flags |= (options.verbatim_args ? UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS : 0);
    // Spawn the child process in a detached state - this will make it a process
    // group leader, and will effectively enable the child to keep running after
    // the parent exits. Note that the child process will still keep the
    // parent's event loop alive unless the parent process calls uv_unref() on
    // the child's process handle.
    native.flags |= (options.detached ? UV_PROCESS_DETACHED : 0);
    // Hide the subprocess console window that would normally be created. This
    // option is only meaningful on Windows systems. On Unix it is silently
    // ignored.
    native.flags |= (options.hide ? UV_PROCESS_WINDOWS_HIDE : 0);

    std::size_t const STDIOS_SIZE = options.stdios.size();
    native.stdio_count = static_cast<int>(STDIOS_SIZE);

    stdios.clear();
    if (STDIOS_SIZE > 0) {
        stdios.resize(STDIOS_SIZE);
        native.stdio = &stdios[0];
    } else {
        native.stdio = nullptr;
    }

    // The 'stdio' field points to an array of uv_stdio_container_t structs that
    // describe the file descriptors that will be made available to the child
    // process. The convention is that stdio[0] points to stdin, fd 1 is used for
    // stdout, and fd 2 is stderr.
    //
    // Note: that on windows file descriptors greater than 2 are available to the
    // child process only if the child processes uses the MSVCRT runtime.
    for (std::size_t index = 0; index < STDIOS_SIZE; ++index) {
        Process::StdioContainer & stdio = options.stdios[index];

        if (stdio.ignore) {
            stdios[index].flags = static_cast<uv_stdio_flags>(UV_IGNORE);
        } else {
            stdios[index].flags = static_cast<uv_stdio_flags>(0);
            stdios[index].flags = static_cast<uv_stdio_flags>(stdios[index].flags | (stdio.create_pipe    ? UV_CREATE_PIPE    : 0));
            stdios[index].flags = static_cast<uv_stdio_flags>(stdios[index].flags | (stdio.inherit_fd     ? UV_INHERIT_FD     : 0));
            stdios[index].flags = static_cast<uv_stdio_flags>(stdios[index].flags | (stdio.inherit_stream ? UV_INHERIT_STREAM : 0));
            stdios[index].flags = static_cast<uv_stdio_flags>(stdios[index].flags | (stdio.readable_pipe  ? UV_READABLE_PIPE  : 0));
            stdios[index].flags = static_cast<uv_stdio_flags>(stdios[index].flags | (stdio.writable_pipe  ? UV_WRITABLE_PIPE  : 0));

            if (stdio.type == Process::StdioContainer::Type::STDIO_CONTAINER_STREAM && stdio.stream != nullptr) {
                stdios[index].data.stream = stdio.stream->cast<uv_stream_t>();
            } else /*if (stdio.type == Process::StdioContainer::Type::STDIO_CONTAINER_FD)*/ {
                stdios[index].data.fd = stdio.fd;
            }
        }
    }

    return true;
}

} // namespace impl

// ---------------------------------------
// Process::StdioContainer implementation.
// ---------------------------------------

Process::StdioContainer::StdioContainer(bool ignore_flag)
        : type(Type::STDIO_CONTAINER_FD), stream(nullptr), fd(0),
          ignore(ignore_flag), create_pipe(false), inherit_fd(false), inherit_stream(false),
          readable_pipe(false), writable_pipe(false)
{
    // EMPTY.
}

Process::StdioContainer::StdioContainer(Stream * s, bool inherit) : StdioContainer()
{
    type = Type::STDIO_CONTAINER_STREAM;
    stream = s;
    inherit_stream = inherit;
    ignore = false;
}

Process::StdioContainer::StdioContainer(int f, bool inherit) : StdioContainer()
{
    type = Type::STDIO_CONTAINER_FD;
    fd = f;
    inherit_fd = inherit;
    ignore = false;
}

Process::StdioContainer::~StdioContainer()
{
    // EMPTY.
}

Process::StdioContainer & Process::StdioContainer::clear()
{
    type = Type::STDIO_CONTAINER_FD;
    stream = nullptr;
    fd = 0;
    ignore         = false;
    create_pipe    = false;
    inherit_fd     = false;
    inherit_stream = false;
    readable_pipe  = false;
    writable_pipe  = false;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setStream(Stream * s)
{
    type = Type::STDIO_CONTAINER_STREAM;
    stream = s;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setFd(int f)
{
    type = Type::STDIO_CONTAINER_FD;
    fd = f;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setIgnore()
{
    ignore = true;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setCreatePipe(bool flag)
{
    create_pipe = flag;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setInheritFd(bool flag)
{
    inherit_fd = flag;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setInheritStream(bool flag)
{
    inherit_stream = flag;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setReadablePipe(bool flag)
{
    readable_pipe = flag;
    return *this;
}

Process::StdioContainer & Process::StdioContainer::setWritablePipe(bool flag)
{
    writable_pipe = flag;
    return *this;
}

// --------------------------------
// Process::Options implementation.
// --------------------------------

Process::Options::Options() : file(), cwd(), args(), envs(), stdios(),
                              uid(0), gid(0), setuid(false), setgid(false),
                              detached(false), verbatim_args(false), hide(false)
{
    appendDefaultEnvironment();
}

Process::Options::~Options()
{
    // EMPTY.
}

Process::Options & Process::Options::clear()
{
    file.clear();
    cwd.clear();
    args.clear();
    envs.clear();
    stdios.clear();
    uid = 0;
    gid = 0;
    setuid = false;
    setgid = false;
    detached = false;
    verbatim_args = false;
    hide = false;
    return *this;
}

Process::Options & Process::Options::setFile(std::string const & path)
{
    file = path;
    return *this;
}

Process::Options & Process::Options::setWorking(std::string const & dir)
{
    cwd = dir;
    return *this;
}

Process::Options & Process::Options::setCurrentWorking()
{
    cwd = filesystem::Path::getWorkDir().getString();
    return *this;
}

Process::Options & Process::Options::appendArgument(std::string const & arg)
{
    args.push_back(arg);
    return *this;
}

Process::Options & Process::Options::appendEnvironment(std::string const & env)
{
    envs.push_back(env);
    return *this;
}

Process::Options & Process::Options::appendEnvironment(std::string const & key, std::string const & val)
{
    envs.push_back(key + "=" + val);
    return *this;
}

Process::Options & Process::Options::appendDefaultEnvironment()
{
    if (isWindowsPlatform()) {
        // https://github.com/libuv/libuv/commit/3409c9b3836e28f8fb3ca22d48ab521950fd3a78
        //
        // The way windows takes environment variables is different than what C does;
        // Windows wants a contiguous block of null-terminated strings, terminated
        // with an additional null.
        //
        // Windows has a few "essential" environment variables. winsock will fail
        // to initialize if SYSTEMROOT is not defined; some APIs make reference to
        // TEMP. SYSTEMDRIVE is probably also important. We therefore ensure that
        // these get defined if the input environment block does not contain any
        // values for them.
        //
        // Also add variables known to Cygwin to be required for correct
        // subprocess operation in many cases:
        // https://github.com/Alexpux/Cygwin/blob/b266b04fbbd3a595f02ea149e4306d3ab9b1fe3d/winsup/cygwin/environ.cc#L955

        std::string value;
        if (isSuccess(libtbag::uvpp::getEnv(ESSENTIAL_ENV_SYSTEMDRIVE, value))) {
            appendEnvironment(ESSENTIAL_ENV_SYSTEMDRIVE, value);
        }
        if (isSuccess(libtbag::uvpp::getEnv(ESSENTIAL_ENV_SYSTEMROOT, value))) {
            appendEnvironment(ESSENTIAL_ENV_SYSTEMROOT, value);
        }
        if (isSuccess(libtbag::uvpp::getEnv(ESSENTIAL_ENV_TEMP, value))) {
            appendEnvironment(ESSENTIAL_ENV_TEMP, value);
        }
    }
    return *this;
}

Process::Options & Process::Options::appendStdio(StdioContainer const & io)
{
    stdios.push_back(io);
    return *this;
}

Process::Options & Process::Options::appendIgnoreStdio()
{
    stdios.emplace_back(true);
    return *this;
}

Process::Options & Process::Options::setUserId(uuser id, bool enable)
{
    uid = id;
    setuid = enable;
    return *this;
}

Process::Options & Process::Options::setGroupId(ugroup id, bool enable)
{
    gid = id;
    setgid = enable;
    return *this;
}

Process::Options & Process::Options::setDetached(bool flag)
{
    detached = flag;
    return *this;
}

Process::Options & Process::Options::setVerbatimArguments(bool flag)
{
    verbatim_args = flag;
    return *this;
}

Process::Options & Process::Options::setHide(bool flag)
{
    hide = flag;
    return *this;
}

std::string Process::Options::getAllArguments() const
{
    std::stringstream ss;
    std::size_t const SIZE = args.size();
    for (std::size_t i = 0; i < SIZE; ++i) {
        ss << args[i];
        if (i + 1 < SIZE) {
            ss << ' ';
        }
    }
    return ss.str();
}

// -----------------------
// Process implementation.
// -----------------------

Process::Process() : Handle(uhandle::PROCESS)
{
    // EMPTY.
}

Process::Process(Loop & loop, Options const & options) : Process()
{
    auto const CODE = spawn(loop, options);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

Process::~Process()
{
    // EMPTY.
}

int Process::getPid() const
{
    return Parent::cast<uv_process_t>()->pid;
}

Err Process::spawn(Loop & loop, Options const & options)
{
    _options = options; // IMPORTANT!!

    uv_process_options_t uv_options = {0,};
    CharPtrs args;
    CharPtrs envs;
    Stdios stdios;

    if (impl::updateOptions(_options, args, envs, stdios, uv_options) == false) {
        tDLogE("Process::spawn() options error.");
        return E_ILLARGS;
    }

    // If the process is successfully spawned, this function will return 0.
    // Otherwise, the negative error code corresponding to the reason it couldn’t spawn is returned.
    //
    // Possible reasons for failing to spawn would include
    // (but not be limited to) the file to execute not existing,
    // not having permissions to use the setuid or setgid specified,
    // or not having enough memory to allocate for the new process.
    tDLogN("Process::spawn() {} {}", _options.file, _options.getAllArguments());
    int const CODE = ::uv_spawn(loop.cast<uv_loop_t>(), Parent::cast<uv_process_t>(), &uv_options);
    return convertUvErrorToErrWithLogging("Process::spawn()", CODE);
}

Err Process::processKill(int signum)
{
    // uv_signal_t - Signal handle for signal support, specially on Windows.
    int const CODE = ::uv_process_kill(Parent::cast<uv_process_t>(), signum);
    return convertUvErrorToErrWithLogging("Process::processKill()", CODE);
}

// --------------
// Event methods.
// --------------

void Process::onExit(int64_t exit_status, int term_signal)
{
    tDLogD("Process::onExit({}, {}) called.", exit_status, term_signal);
}

// ---------------
// Static methods.
// ---------------

void Process::disableStdioInheritance()
{
    // The effect is that child processes spawned by this process don’t accidentally inherit these handles.
    //
    // It is recommended to call this function as early in your program as possible,
    // before the inherited file descriptors can be closed or duplicated.
    //
    // Note:
    // This function works on a best-effort basis:
    // there is no guarantee that libuv can discover all file descriptors that were inherited.
    // In general it does a better job on Windows than it does on Unix.
    ::uv_disable_stdio_inheritance();
}

Err Process::kill(int pid, int signum)
{
    // uv_signal_t - Signal handle for signal support, specially on Windows.
    int const CODE = ::uv_kill(pid, signum);
    return convertUvErrorToErrWithLogging("Process::kill()", CODE);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

