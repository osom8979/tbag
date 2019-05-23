/**
 * @file   ProcessId.cpp
 * @brief  ProcessId class implementation.
 * @author zer0
 * @date   2019-05-23
 */

#include <libtbag/system/ProcessId.hpp>
#include <libtbag/config-ex.h>

#if defined(TBAG_PLATFORM_WINDOWS)
#include <Windows.h>
#endif
#if defined(HAVE_SYS_TYPES_H)
#include <sys/types.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {

long getProcessId()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    return (long)GetCurrentProcessId();
#else
    return (long)getpid();
#endif
}

} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

