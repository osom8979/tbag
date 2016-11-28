/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2016-11-27
 */

#include <libtbag/process/Process.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
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
private:
    uv_process_t         _process;
    uv_process_options_t _options;

public:
    ProcPimpl()
    {
        ::memset((void*)&_process, 0x00, sizeof(_process));
        ::memset((void*)&_options, 0x00, sizeof(_options));
    }

    ~ProcPimpl()
    {
        // EMPTY.
    }

public:
    inline uv_process_t * handle()
    { return &_process; }
    inline uv_process_t const * handle() const
    { return &_process; }

    inline uv_process_options_t * options()
    { return &_options; }
    inline uv_process_options_t const * options() const
    { return &_options; }

    inline void clearOptions()
    { ::memset((void*)&_options, 0x00, sizeof(_options)); }
};

// --------------------
// Process message map.
// --------------------

TBAG_EVENT_IMPLEMENT_OPEN(__process_manager__, Process);
//{
    TBAG_EVENT_EXIT(onExit);
//}
TBAG_EVENT_IMPLEMENT_CLOSE(__process_manager__);

// -----------------------------
// Process class implementation.
// -----------------------------

Process::Process() : _process(new ProcPimpl())
{
    TBAG_EVENT_REGISTER(__process_manager__, _process->handle(), this);
}

Process::~Process()
{
    TBAG_EVENT_UNREGISTER(__process_manager__, _process->handle());
}

bool Process::exe(Param const & param)
{
    setParam(param);
    return _loop.runDefault();
}

bool Process::exe(Path const & exe_path, Path const & work_dir)
{
    return exe(Param().setExePath(exe_path).setWorkingDir(work_dir));
}

bool Process::exe(Path const & exe_path)
{
    return exe(exe_path, Path::getWorkDir());
}

void Process::update()
{
    std::size_t const ARGS_SIZE = _param.args.size();
    std::size_t const ENVS_SIZE = _param.envs.size();

    _args_ptr.clear();
    _envs_ptr.clear();
    _args_ptr.resize(ARGS_SIZE + 2); // + EXE_PATH, NULL
    _envs_ptr.resize(ENVS_SIZE + 1); // + NULL

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
    _process->clearOptions();
    _process->options()->exit_cb = TBAG_EVENT_CALLBACK_EXIT(__process_manager__, onExit);
    _process->options()->file    = _param.exe_path.c_str();
    _process->options()->cwd     = _param.work_dir.c_str();
    _process->options()->args    = &_args_ptr[0];
    _process->options()->env     = &_envs_ptr[0];
    _process->options()->flags   = _param.flags;
}

bool Process::spawn()
{
    return ::uv_spawn(static_cast<uv_loop_t*>(_loop.getNative()), _process->handle(), _process->options()) != 0;
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

