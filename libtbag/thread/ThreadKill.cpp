/**
 * @file   ThreadKill.cpp
 * @brief  ThreadKill class implementation.
 * @author zer0
 * @date   2019-10-17
 */

#include <libtbag/thread/ThreadKill.hpp>
#include <libtbag/config-ex.h>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/Err.hpp>

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

Err __kill_thread_win32(libtbag::uvpp::uthread id, int signum)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return TerminateThread(id, signum) == TRUE ? E_SUCCESS : E_UNKNOWN;
#else
    return E_ENOSYS;
#endif
}

Err __kill_thread_pthread(libtbag::uvpp::uthread id, int signum)
{
#if defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_kill(id, signum);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

Err killThread(libtbag::uvpp::uthread id, int signum)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return __kill_thread_win32(id, signum);
#elif defined(TBAG_USE_PTHREADS)
    return __kill_thread_pthread(id, signum);
#else
    return E_ENOSYS;
#endif
}

Err cancelThread(libtbag::uvpp::uthread id)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return TerminateThread(id, EXIT_FAILURE) == TRUE ? E_SUCCESS : E_UNKNOWN;
#elif defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_cancel(id);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

Err setEnableCancelState()
{
#if defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

Err setDisableCancelState()
{
#if defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

Err setAsynchronousCancelType()
{
#if defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

Err setDeferredCancelType()
{
#if defined(TBAG_USE_PTHREADS)
    auto const result = ::pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
    if (result == 0) {
        return E_SUCCESS;
    }
    return libtbag::convertSystemErrorToErr(result);
#else
    return E_ENOSYS;
#endif
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

