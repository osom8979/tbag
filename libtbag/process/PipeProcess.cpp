/**
 * @file   PipeProcess.hpp
 * @brief  PipeProcess class implementation.
 * @author zer0
 * @date   2016-05-17
 * @date   2016-11-27 (Rename: PipeProcess -> PipeProcess)
 */

#include <libtbag/process/PipeProcess.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <cstring>
#include <array>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

static int const STANDARD_INPUT_FD  = 0; ///< @c stdin
static int const STANDARD_OUTPUT_FD = 1; ///< @c stdout
static int const STANDARD_ERROR_FD  = 2; ///< @c stderr
static int const STANDARD_IO_SIZE   = 3;

/**
 * Pointer to implementation of @c uv_process_t.
 *
 * @author zer0
 * @date   2016-10-15
 *
 * @remarks
 *  Use the libuv.
 */
struct PipeProcess::ProcPimpl : public libtbag::loop::event::UvHandler
{
private:
    PipeProcess & _parent;
    uv_process_t _process;

private:
    uv_process_options_t _options;
    PipeProcess::Param       _param;
    std::vector<char*>   _args_ptr;
    std::vector<char*>   _envs_ptr;

private:
    using StdioArray = std::array<uv_stdio_container_t, STANDARD_IO_SIZE>;
    StdioArray _stdios;
    OutStream  _out;
    OutStream  _err;
    InStream   _in;

private:
    int64_t _exit_status;
    int _terminate_signal;

public:
    ProcPimpl(PipeProcess & parent)
            : UvHandler(&_process)
            , _parent(parent)
            , _exit_status(PipeProcess::getUnknownExitCode())
            , _terminate_signal(PipeProcess::getUnknownTerminateSignal())
    {
        ::memset((void*)&_process, 0x00, sizeof(_process));
    }

    ~ProcPimpl()
    {
        // EMPTY.
    }

public:
    inline int64_t getExitStatus() const TBAG_NOEXCEPT
    { return _exit_status; }
    inline int getTerminateSignal() const TBAG_NOEXCEPT
    { return _terminate_signal; }

    inline OutStream & getOut()
    { return _out; }
    inline OutStream & getErr()
    { return _err; }
    inline InStream  & getIn()
    { return _in; }

public:
    bool update(PipeProcess::Param const & param)
    {
        _param = param;

        std::size_t const ARGS_SIZE = _param.args.size();
        std::size_t const ENVS_SIZE = _param.envs.size();

        _args_ptr.clear();
        _envs_ptr.clear();
        _args_ptr.resize(ARGS_SIZE + 2);
        _envs_ptr.resize(ENVS_SIZE + 1);

        // First: exe file path.
        _args_ptr[0] = &_param.exe_path[0];

        // Arguments.
        for (std::size_t index = 0; index < ARGS_SIZE; ++index) {
            _args_ptr[index + 1] = &_param.args[index][0];
        }

        // Environment.
        for (std::size_t index = 0; index < ENVS_SIZE; ++index) {
            _envs_ptr[index] = &_param.envs[index][0];
        }

        // Last: NULL pointer.
        _args_ptr[ARGS_SIZE + 1] = nullptr;
        _envs_ptr[ENVS_SIZE + 0] = nullptr;

        // Update options.
        ::memset((void*)&_options, 0x00, sizeof(_options));
        _options.exit_cb = TBAG_UV_EVENT_CALLBACK_EXIT;
        _options.file    = _param.exe_path.c_str();
        _options.cwd     = _param.work_dir.c_str();
        _options.args    = &_args_ptr[0];
        _options.env     = &_envs_ptr[0];
        _options.flags   = _param.flags;

        // The ipc argument is a boolean to indicate
        // if this pipe will be used for handle passing between processes.
        int const ENABLE_IPC  = 1;
        int const DISABLE_IPC = 0;

        REMOVE_UNUSED_VARIABLE(ENABLE_IPC);
        REMOVE_UNUSED_VARIABLE(DISABLE_IPC);

        uv_loop_t * loop = static_cast<uv_loop_t*>(_parent._loop->getNative());
        uv_pipe_t * pipe_stdout = static_cast<uv_pipe_t*>(_out.getNative());
        uv_pipe_t * pipe_stderr = static_cast<uv_pipe_t*>(_err.getNative());
        uv_pipe_t * pipe_stdin  = static_cast<uv_pipe_t*>(_in.getNative());

        _out.setOnReadCallback(_param.out_callback);
        _err.setOnReadCallback(_param.err_callback);
        _in.setOnWriteCallback(_param.in_callback);

        uv_pipe_init(loop, pipe_stdin , DISABLE_IPC);
        uv_pipe_init(loop, pipe_stdout, DISABLE_IPC);
        uv_pipe_init(loop, pipe_stderr, DISABLE_IPC);

        uv_stdio_flags const READ_PIPE_FLAGS =
                static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_READABLE_PIPE);
        uv_stdio_flags const WRITE_PIPE_FLAGS =
                static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);

        _stdios[STANDARD_INPUT_FD].flags = READ_PIPE_FLAGS;
        _stdios[STANDARD_INPUT_FD].data.stream = (uv_stream_t*)pipe_stdin;

        _stdios[STANDARD_OUTPUT_FD].flags = WRITE_PIPE_FLAGS;
        _stdios[STANDARD_OUTPUT_FD].data.stream = (uv_stream_t*)pipe_stdout;

        _stdios[STANDARD_ERROR_FD ].flags = WRITE_PIPE_FLAGS;
        _stdios[STANDARD_ERROR_FD ].data.stream = (uv_stream_t*)pipe_stderr;

        _options.stdio = &_stdios[0];
        _options.stdio_count = STANDARD_IO_SIZE;

        if (uv_spawn(loop, &_process, &_options) != 0) {
            return false;
        }
        if (_param.input_buffer.empty() == false && _in.write(_param.input_buffer) == false) {
            return false;
        }
        if (_out.startRead() == false || _err.startRead() == false) {
            return false;
        }

        return true;
    }

    virtual void onExit(void * process, int64_t exit_status, int term_signal) override
    {
        _exit_status = exit_status;
        _terminate_signal = term_signal;
    }
};

// -----------------------
// PipeProcess implementation.
// -----------------------

PipeProcess::PipeProcess()
        : _loop(new loop::UvEventLoop())
        , _process(new ProcPimpl(*this))
{
    // EMPTY.
}

PipeProcess::~PipeProcess()
{
    // EMPTY.
}

bool PipeProcess::exe(Param const & param)
{
    if (_process->update(param)) {
        return _loop->runDefault();
    }
    return false;
}

bool PipeProcess::exe(Path const & exe_path, Path const & work_dir)
{
    return exe(Param().setExePath(exe_path).setWorkingDir(work_dir));
}

bool PipeProcess::exe(Path const & exe_path)
{
    return exe(exe_path, Path::getWorkDir());
}

bool PipeProcess::write(char const * buffer, std::size_t length)
{
    return _process->getIn().write(buffer, length);
}

int64_t PipeProcess::getExitStatus() const
{
    return _process->getExitStatus();
}

int PipeProcess::getTerminateSignal() const
{
    return _process->getTerminateSignal();
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

