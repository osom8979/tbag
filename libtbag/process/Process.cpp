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

// -----------------------------
// Process class implementation.
// -----------------------------

TBAG_EVENT_IMPLEMENT_OPEN(__process_manager__, Process);
//{
TBAG_EVENT_EXIT(onExit);
//}
TBAG_EVENT_IMPLEMENT_CLOSE(__process_manager__);

Process::Process()
{
    // EMPTY.
}

Process::~Process()
{
    // EMPTY.
}

bool Process::exe(std::string const & exe_path, std::string const & work_dir)
{
    // TBAG_EVENT_CALLBACK_EXIT(__process_manager__, onExit);
    return false;
}

bool Process::exe(std::string const & exe_path)
{
    return false;
}

int64_t Process::getExitStatus() const
{
    return 0;
}

int Process::getTerminateSignal() const
{
    return 0;
}

void Process::onExit(void * handle, int64_t exit_status, int term_signal)
{
    _exit_status = exit_status;
    _terminate_signal = term_signal;
}

} // namespace process

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

