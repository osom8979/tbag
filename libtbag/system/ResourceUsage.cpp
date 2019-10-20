/**
 * @file   ResourceUsage.cpp
 * @brief  ResourceUsage class implementation.
 * @author zer0
 * @date   2019-10-20
 */

#include <libtbag/system/ResourceUsage.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

ErrResourceUsage getResourceUsage()
{
    uv_rusage_t rusage = {0,};
    int const uv_error = ::uv_getrusage(&rusage);
    if (uv_error != 0) {
        return libtbag::convertUvErrorToErr(uv_error);
    }

    ResourceUsage result = {};
    result.utime.sec  = rusage.ru_utime.tv_sec;
    result.utime.usec = rusage.ru_utime.tv_usec;
    result.stime.sec  = rusage.ru_stime.tv_sec;
    result.stime.usec = rusage.ru_stime.tv_usec;
    result.maxrss     = rusage.ru_maxrss;
    result.ixrss      = rusage.ru_ixrss;
    result.idrss      = rusage.ru_idrss;
    result.isrss      = rusage.ru_isrss;
    result.minflt     = rusage.ru_minflt;
    result.majflt     = rusage.ru_majflt;
    result.nswap      = rusage.ru_nswap;
    result.inblock    = rusage.ru_inblock;
    result.oublock    = rusage.ru_oublock;
    result.msgsnd     = rusage.ru_msgsnd;
    result.msgrcv     = rusage.ru_msgrcv;
    result.nsignals   = rusage.ru_nsignals;
    result.nvcsw      = rusage.ru_nvcsw;
    result.nivcsw     = rusage.ru_nivcsw;
    return { E_SUCCESS, result };
}

static ErrResourceUsage __get_resource_usage_windows(int pid)
{
    return E_ENOSYS;
}

static ErrResourceUsage __get_resource_usage_macos(int pid)
{
    return E_ENOSYS;
}

static ErrResourceUsage __get_resource_usage_linux(int pid)
{
    return E_ENOSYS;
}

ErrResourceUsage getResourceUsage(int pid)
{
    ResourceUsage result = {};
#if defined(TBAG_PLATFORM_WINDOWS)
    return __get_resource_usage_windows(pid);
#elif defined(TBAG_PLATFORM_MACOS)
    return __get_resource_usage_macos(pid);
#elif defined(TBAG_PLATFORM_LINUX)
    return __get_resource_usage_linux(pid);
#else
    return E_ENOSYS;
#endif
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

