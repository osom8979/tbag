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

/**
 * Pointer to implementation of @c uv_process_t.
 *
 * @author zer0
 * @date   2016-10-15
 *
 * @remarks
 *  Use the libuv.
 */
struct Process::ProcPimpl
{
private:
    Process & _parent;
    uv_process_t _process;

public:
    ProcPimpl(Process & process) : _parent(process)
    {
        ::memset((void*)&_process, 0x00, sizeof(_process));
        _parent.add(&_process);
    }

    ~ProcPimpl()
    {
        _parent.remove(&_process);
    }

    inline uv_process_t * getNative()
    { return &_process; }
    inline uv_process_t const * getNative() const
    { return &_process; }
};

/**
 * Pointer to implementation of @c uv_process_options_t.
 *
 * @author zer0
 * @date   2016-10-15
 *
 * @remarks
 *  Use the libuv.
 */
struct Process::ProcOptionPimpl
{
private:
    uv_process_options_t _options;
    Process::Param       _param;
    std::vector<char*>   _args_ptr;
    std::vector<char*>   _envs_ptr;

public:
    ProcOptionPimpl() = default;
    ~ProcOptionPimpl() = default;

    inline uv_process_options_t * getNative()
    { return &_options; }
    inline uv_process_options_t const * getNative() const
    { return &_options; }

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
        _options.exit_cb = (uv_exit_cb) &libtbag::loop::event::uv::onExit;
        _options.file    = _param.exe_path.c_str();
        _options.cwd     = _param.work_dir.c_str();
        _options.args    = &_args_ptr[0];
        _options.env     = &_envs_ptr[0];
        _options.flags   = _param.flags;

        return true;
    }
};

// -----------------------
// Process implementation.
// -----------------------

Process::Process() throw(InitializeException)
        : _loop(new loop::UvEventLoop())
        , _process(new ProcPimpl(*this))
        , _options(new ProcOptionPimpl())
        , _exit_status(Process::getUnknownExitCode())
        , _terminate_signal(Process::getUnknownTerminateSignal())
{
    // EMPTY.
}

Process::~Process()
{
    // EMPTY.
}

bool Process::spawn()
{
    return ::uv_spawn(static_cast<uv_loop_t*>(_loop->getNative())
                    , _process->getNative()
                    , _options->getNative()) == 0;
}

bool Process::exe(Param const & param)
{
    if (_options->update(param) && spawn()) {
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

void Process::onExit(void * process, int64_t exit_status, int term_signal)
{
    _exit_status = exit_status;
    _terminate_signal = term_signal;
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

