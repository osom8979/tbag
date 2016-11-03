/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2016-10-15
 */

#include <libtbag/process/Process.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/predef.hpp>

#include <cstring>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace process {

std::string getExecutableSuffix()
{
#if defined(__OS_WINDOWS__)
    return ".exe";
#else
    return "";
#endif
}

std::string getExecutableName(std::string const & name)
{
    return name + getExecutableSuffix();
}

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
struct Process::ProcPimpl : public libtbag::loop::event::UvHandler
{
private:
    uv_process_t _process;

private:
    uv_process_options_t _options;
    Process::Param       _param;
    std::vector<char*>   _args_ptr;
    std::vector<char*>   _envs_ptr;

private:
    int64_t _exit_status;
    int _terminate_signal;

public:
    ProcPimpl() : UvHandler(&_process)
                , _exit_status(Process::getUnknownExitCode())
                , _terminate_signal(Process::getUnknownTerminateSignal())
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

public:
    bool update(Process::Param const & param)
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

        //_options.stdio = &this->_stdios[0];
        //_options.stdio_count = STDIO_SIZE;

        return true;
    }

    bool spawn(uv_loop_t * loop)
    {
        return ::uv_spawn(loop, &_process, &_options) == 0;
    }

    virtual void onExit(void * process, int64_t exit_status, int term_signal) override
    {
        _exit_status = exit_status;
        _terminate_signal = term_signal;
    }
};

// -----------------------
// Process implementation.
// -----------------------

Process::Process()
        : _loop(new loop::UvEventLoop())
        , _process(new ProcPimpl())
{
    // EMPTY.
}

Process::~Process()
{
    // EMPTY.
}

bool Process::exe(Param const & param)
{
    uv_loop_t * loop = static_cast<uv_loop_t*>(_loop->getNative());
    if (_process->update(param) && _process->spawn(loop)) {
        return _loop->runDefault();
    }
    return false;
}

bool Process::exe(Path const & exe_path, Path const & work_dir)
{
    return exe(Param().setExePath(exe_path).setWorkingDir(work_dir));
}

bool Process::exe(Path const & exe_path)
{
    return exe(exe_path, Path::getWorkDir());
}

int64_t Process::getExitStatus()
{
    return _process->getExitStatus();
}

int Process::getTerminateSignal()
{
    return _process->getTerminateSignal();
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

