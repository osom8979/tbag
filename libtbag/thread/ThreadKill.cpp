/**
 * @file   ThreadKill.cpp
 * @brief  ThreadKill class implementation.
 * @author zer0
 * @date   2019-10-17
 */

#include <libtbag/thread/ThreadKill.hpp>
#include <libtbag/config-ex.h>
#include <libtbag/signal/SignalHandler.hpp>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <Windows.h> // or <processthreadsapi.h>
#else
# include <libtbag/dummy/Win32.hpp>
using namespace libtbag::dummy::win32;
#endif

#if defined(TBAG_USE_PTHREADS)
#include <pthread.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

Err __kill_thread_win32(libtbag::uvpp::uthread id)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return TerminateThread((HANDLE)id, EXIT_FAILURE) == TRUE ? E_SUCCESS : E_UNKNOWN;
#else
    return E_ENOSYS;
#endif
}

Err __kill_thread_pthread(libtbag::uvpp::uthread id)
{
#if defined(TBAG_USE_PTHREADS)
    return ::pthread_kill((pthread_t)id, libtbag::signal::TBAG_SIGNAL_KILL) == 0 ? E_SUCCESS : E_UNKNOWN;
#else
    return E_ENOSYS;
#endif
}

Err killThread(libtbag::uvpp::uthread id)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return __kill_thread_win32(id);
#elif defined(TBAG_USE_PTHREADS)
    return __kill_thread_pthread(id);
#else
    return E_ENOSYS;
#endif
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

