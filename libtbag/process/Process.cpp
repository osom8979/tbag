/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2016-11-27
 */

#include <libtbag/process/Process.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>

#include <array>
#include <uv.h>
#include <dep/uv/include/uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

// @formatter:off
inline TBAG_CONSTEXPR static bool isPrintOnCloseDebugMessage() TBAG_NOEXCEPT { return true; }
// @formatter:on

std::string getExecutableSuffix()
{
#if defined(__PLATFORM_WINDOWS__)
    return ".exe";
#else
    return "";
#endif
}

std::string getExecutableName(std::string const & name)
{
    return name + getExecutableSuffix();
}

// ------------------------------------
// Process::Pimpl class implementation.
// ------------------------------------

/**
 * Pointer to implementation of @c uv_process_t.
 *
 * @author zer0
 * @date   2016-11-28
 *
 * @remarks
 *  Use the libuv.
 */
struct Process::ProcPimpl : public Noncopyable
{
public:
    using StdioArray = std::array<uv_stdio_container_t, STANDARD_IO_SIZE>;

private:
    uv_process_t         _process;
    uv_process_options_t _options;

private:
    StdioArray _stdios;

private:
    int _pid; ///< spawned process id.

public:
    ProcPimpl() : _pid(UNKNOWN_PROCESS_ID)
    {
        ::memset((void*)&_process, 0x00, sizeof(_process));
        ::memset((void*)&_options, 0x00, sizeof(_options));
        ::memset((void*)_stdios.data(), 0x00, sizeof(_stdios.data()) * _stdios.size());
    }

    ~ProcPimpl()
    {
        // EMPTY.
    }

public:
    inline int getSpawnedPid() const TBAG_NOEXCEPT
    { return _pid; }

    inline uv_process_t * handle()
    { return &_process; }
    inline uv_process_t const * handle() const
    { return &_process; }

    inline uv_process_options_t * options()
    { return &_options; }
    inline uv_process_options_t const * options() const
    { return &_options; }

    inline StdioArray & atStdios()
    { return _stdios; }
    inline StdioArray const & atStdios() const
    { return _stdios; }

    inline void clearOptions()
    { ::memset((void*)&_options, 0x00, sizeof(_options)); }

    void setStdio(uv_loop_t * loop, uv_pipe_t * pipe, int index, Param::IoOption const & option)
    {
        // The ipc argument is a boolean to indicate
        // if this pipe will be used for handle passing between processes.
        int const ENABLE_IPC  = 1;
        int const DISABLE_IPC = 0;
        REMOVE_UNUSED_VARIABLE(ENABLE_IPC);
        REMOVE_UNUSED_VARIABLE(DISABLE_IPC);

        ::uv_pipe_init(loop, pipe, DISABLE_IPC);

        if (option.flag == Param::IoFlag::IGNORE_IO) {
            _stdios[index].flags = UV_IGNORE;
        } else if (option.flag == Param::IoFlag::INHERIT) {
            _stdios[index].flags = UV_INHERIT_FD;
            _stdios[index].data.fd = option.fd;
        } else if (option.flag == Param::IoFlag::PIPE) {
            if (index == STANDARD_INPUT_FD) {
                _stdios[index].flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_READABLE_PIPE);
            } else {
                _stdios[index].flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
            }
            _stdios[index].data.stream = (uv_stream_t*)pipe;
        }
    }

    inline ErrorCode spawn(EventLoop & loop)
    {
        int const CODE = ::uv_spawn(static_cast<uv_loop_t*>(loop.getNative()), &_process, &_options);
        if (CODE == 0) {
            _pid = _process.pid;
        } else {
            _pid = UNKNOWN_PROCESS_ID;
            // Error code case:
            // - EAGAIN: Resource temporarily unavailable (may be the same value as EWOULDBLOCK) (POSIX.1).
            __tbag_debug("Process::ProcPimpl::spawn() error[{}] {}", CODE, uv::getUvErrorName(CODE));
            return ErrorCode::FAILURE;
        }
        return ErrorCode::SUCCESS;
    }

    inline ErrorCode kill(int signal_number)
    {
        if (_pid == UNKNOWN_PROCESS_ID || _pid == 0) {
            __tbag_debug("Process::ProcPimpl::kill({}) {}",
                         signal_number,
                         debug::getErrorMessage(ErrorCode::UNKNOWN_PROCESS_ID));
            return ErrorCode::UNKNOWN_PROCESS_ID;
        }

        int const CODE = ::uv_kill(_pid, signal_number);
        if (CODE != 0) {
            __tbag_debug("Process::ProcPimpl::kill({}) error[{}] {}",
                         signal_number,
                         CODE,
                         uv::getUvErrorName(CODE));
            return ErrorCode::FAILURE;
        }
        return ErrorCode::SUCCESS;
    }

    bool isClosing() const TBAG_NOEXCEPT
    {
        return ::uv_is_closing((uv_handle_t*)&_process) != 0;
    }

    void close(uv_close_cb close_cb)
    {
        ::uv_close((uv_handle_t*)&_process, close_cb);
    }
};

// --------------------
// Process message map.
// --------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(Process);
//{
    TBAG_UV_EVENT_EXIT(onExit);
    TBAG_UV_EVENT_CLOSE(onClose);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE(Process);

// -----------------------------
// Process class implementation.
// -----------------------------

Process::Process() : _process(new ProcPimpl()),
                     _in(uv::UvHandleType::PIPE),
                     _out(uv::UvHandleType::PIPE),
                     _err(uv::UvHandleType::PIPE)
{
    TBAG_UV_EVENT_DEFAULT_REGISTER(_process->handle(), this);
}

Process::~Process()
{
    close();
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_process->handle());
}

void Process::clear()
{
    _process->clearOptions();
}

void Process::update()
{
    std::size_t const ARGS_SIZE = _param.arguments.size();
    std::size_t const ENVS_SIZE = _param.environments.size();

    _args_ptr.clear();
    _envs_ptr.clear();
    _args_ptr.resize(ARGS_SIZE + 2); // + EXE_PATH, NULL
    _envs_ptr.resize(ENVS_SIZE + 1); // + NULL

    // First: exe file path.
    _args_ptr[0] = &_param.exe_path[0];

    // Arguments.
    for (std::size_t index = 0; index < ARGS_SIZE; ++index) {
        _args_ptr[index + 1] = &_param.arguments[index][0];
    }

    // Environment.
    for (std::size_t index = 0; index < ENVS_SIZE; ++index) {
        _envs_ptr[index] = &_param.environments[index][0];
    }

    // Last: NULL pointer.
    _args_ptr[ARGS_SIZE + 1] = nullptr;
    _envs_ptr[ENVS_SIZE + 0] = nullptr;

    // Update options.
    _process->options()->exit_cb = TBAG_UV_EVENT_DEFAULT_CALLBACK_EXIT(onExit);
    _process->options()->file    = _param.exe_path.c_str();
    _process->options()->cwd     = _param.work_dir.c_str();
    _process->options()->args    = &_args_ptr[0];
    _process->options()->env     = &_envs_ptr[0];
}

void Process::updateWithFlags()
{
    static_assert(sizeof(_param.uid) == sizeof(_process->options()->uid), "Not equal uid size type.");
    static_assert(sizeof(_param.gid) == sizeof(_process->options()->gid), "Not equal gid size type.");

    _process->options()->flags = 0;

    // Changing the UID/GID is only supported on Unix,
    // uv_spawn will fail on Windows with UV_ENOTSUP.
    if (_param.uid != 0) {
        // sets the child’s execution user ID to uv_process_options_t.uid.
        _process->options()->flags |= UV_PROCESS_SETUID;
        _process->options()->uid = _param.uid;
    }

    if (_param.gid != 0) {
        // sets the child’s execution group ID to uv_process_options_t.gid.
        _process->options()->flags |= UV_PROCESS_SETGID;
        _process->options()->gid = _param.gid;
    }

    if (_param.verbatim_arg) {
        // No quoting or escaping of uv_process_options_t.args is done on Windows. Ignored on Unix.
        _process->options()->flags |= UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS;
    }

    if (_param.detached) {
        // Starts the child process in a new session,
        // which will keep running after the parent process exits.
        _process->options()->flags |= UV_PROCESS_DETACHED;
    }
}

void Process::updateWithStdios()
{
    if (/**/_param.in.flag  == Param::IoFlag::IGNORE_IO &&
            _param.out.flag == Param::IoFlag::IGNORE_IO &&
            _param.err.flag == Param::IoFlag::IGNORE_IO) {
        _process->options()->stdio_count = 0;
        _process->atStdios()[STANDARD_INPUT_FD ].flags = UV_IGNORE;
        _process->atStdios()[STANDARD_OUTPUT_FD].flags = UV_IGNORE;
        _process->atStdios()[STANDARD_ERROR_FD ].flags = UV_IGNORE;
        return;
    }

    // The ipc argument is a boolean to indicate
    // if this pipe will be used for handle passing between processes.
    int const ENABLE_IPC  = 1;
    int const DISABLE_IPC = 0;

    REMOVE_UNUSED_VARIABLE(ENABLE_IPC);
    REMOVE_UNUSED_VARIABLE(DISABLE_IPC);

    _process->options()->stdio = _process->atStdios().data();
    _process->options()->stdio_count = STANDARD_IO_SIZE;
    if (_param.in_buffer.empty()) {
        _process->atStdios()[STANDARD_INPUT_FD].flags = UV_IGNORE;
    } else {
        _process->setStdio(_loop.castNative<uv_loop_t>(), _in.castNative<uv_pipe_t>(), STANDARD_INPUT_FD, _param.in);
    }
    _process->setStdio(_loop.castNative<uv_loop_t>(), _out.castNative<uv_pipe_t>(), STANDARD_OUTPUT_FD, _param.out);
    _process->setStdio(_loop.castNative<uv_loop_t>(), _err.castNative<uv_pipe_t>(), STANDARD_ERROR_FD, _param.err);
}

ErrorCode Process::spawn()
{
    return _process->spawn(_loop);
}

bool Process::exe()
{
    clear();
    update();
    updateWithFlags();
    updateWithStdios();

    // @formatter:off
    if (_process->atStdios()[STANDARD_INPUT_FD ].flags & UV_READABLE_PIPE) { _in.write(_param.in_buffer); }
    if (_process->atStdios()[STANDARD_OUTPUT_FD].flags & UV_WRITABLE_PIPE) { _out.read(); }
    if (_process->atStdios()[STANDARD_ERROR_FD ].flags & UV_WRITABLE_PIPE) { _out.read(); }
    // @formatter:on

    spawn();

    if ((_process->options()->flags & UV_PROCESS_DETACHED) == UV_PROCESS_DETACHED) {
        // Just remember that the handle is still monitoring the child, so your program won’t exit.
        // Use uv_unref() if you want to be more fire-and-forget.
        ::uv_unref((uv_handle_t*)_process->handle());
    }

    return _loop.runDefault();
}

void Process::close()
{
    close(_in);
    close(_out);
    close(_err);
    if (_process->isClosing() == false) {
        _process->close(TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onClose));
    }
    _loop.runDefault(); // CLOSE RUNNER.
}

void Process::close(uv::Handle & handle)
{
    if (handle.isInit() && handle.isClosing() == false) {
        ::uv_close(handle.castNative<uv_handle_t>(), TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onClose));
    }
}

bool Process::exe(Param const & param)
{
    setParam(param);
    return exe();
}

bool Process::exe(Path const & exe_path, Path const & work_dir, Strings const & args, Strings const & envs)
{
    return exe(Param().setExePath(exe_path).setWorkingDir(work_dir).setArguments(args).setEnvironments(envs));
}

bool Process::exe(Path const & exe_path, Path const & work_dir)
{
    return exe(exe_path, work_dir, Strings(), Strings());
}

bool Process::exe(Path const & exe_path)
{
    return exe(exe_path, Path::getWorkDir());
}

int Process::getProcessId() const
{
    return _process->getSpawnedPid();
}

ErrorCode Process::kill(int signal_number)
{
    return _process->kill(signal_number);
}

void Process::onExit(void * process, int64_t exit_status, int term_signal)
{
    //static_cast<uv_process_t*>(process);
    _exit_status = exit_status;
    _terminate_signal = term_signal;
}

void Process::onClose(void * handle)
{
    if (isPrintOnCloseDebugMessage()) {
        __tbag_debug("Process::onClose({})", uv::getUvNativeHandleName(_process->handle()));
    }
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

