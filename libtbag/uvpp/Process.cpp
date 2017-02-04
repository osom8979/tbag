/**
 * @file   Process.cpp
 * @brief  Process class implementation.
 * @author zer0
 * @date   2017-02-03
 */

#include <libtbag/uvpp/Process.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_exit_cb__(uv_process_t * handle, int64_t exit_status, int term_signal)
{
    // Type definition for callback passed in uv_process_options_t
    // which will indicate the exit status and the signal that caused the process to terminate, if any.
}

/* inline */ namespace impl {

static bool convertOptionsToNative(Process::Options const & options, uv_process_options_t & native)
{
    return false;
}

} // namespace impl

// --------------------------------
// Process::Options implementation.
// --------------------------------

Process::Options::Options()
{
}

Process::Options::~Options()
{
}

Process::Options::Options(Options const & obj)
{
}

Process::Options::Options(Options && obj)
{
}

Process::Options & Process::Options::operator =(Options const & obj)
{
    if (this != & obj) {
    }
    return *this;
}

Process::Options & Process::Options::operator =(Options && obj)
{
    if (this != & obj) {
    }
    return *this;
}

void swap(Process::Options & obj1, Process::Options & obj2)
{
    Process::Options temp = obj1;
    obj1 = obj2;
    obj2 = temp;
}

// -----------------------
// Process implementation.
// -----------------------

Process::Process() : Handle(uhandle::IDLE)
{
    // EMPTY.
}

Process::Process(Loop & loop, Options const & options) : Process()
{
    if (spawn(loop, options) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Process::~Process()
{
    // EMPTY.
}

uerr Process::spawn(Loop & loop, Options const & options)
{
    uv_process_options_t uv_options = {0,};
    if (impl::convertOptionsToNative(options, uv_options) == false) {
        __tbag_error("Process::spawn() options error.");
        return uerr::UVPP_ILLARGS;
    }

    // If the process is successfully spawned, this function will return 0.
    // Otherwise, the negative error code corresponding to the reason it couldn’t spawn is returned.
    //
    // Possible reasons for failing to spawn would include
    // (but not be limited to) the file to execute not existing,
    // not having permissions to use the setuid or setgid specified,
    // or not having enough memory to allocate for the new process.
    int const CODE = ::uv_spawn(loop.cast<uv_loop_t>(), Parent::cast<uv_process_t>(), &uv_options);
    return getUerr2("Process::spawn()", CODE);
}

uerr Process::processKill(int signum)
{
    // uv_signal_t - Signal handle for signal support, specially on Windows.
    int const CODE = ::uv_process_kill(Parent::cast<uv_process_t>(), signum);
    return getUerr2("Process::processKill()", CODE);
}

// --------------
// Event methods.
// --------------

void Process::onExit(int64_t exit_status, int term_signal)
{
    __tbag_debug("Process::onExit({}, {}) called.", exit_status, term_signal);
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

uerr Process::kill(int pid, int signum)
{
    // uv_signal_t - Signal handle for signal support, specially on Windows.
    int const CODE = ::uv_kill(pid, signum);
    return getUerr2("Process::kill()", CODE);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

