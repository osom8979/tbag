/**
 * @file   dlog.cpp
 * @brief  dlog class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/log/details/dlog.hpp>
#include <libtbag/lock/UvLock.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstdio>
#include <cassert>

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 1
#  define _DEFAULT_DEBUG_LOG_LEVEL ENABLE_TBAG_LIBRARY_DEBUGGING_LOG
# else
#  define _DEFAULT_DEBUG_LOG_LEVEL 0
# endif
#else
# define _DEFAULT_DEBUG_LOG_LEVEL 5
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

static int volatile g_debug_log_level = _DEFAULT_DEBUG_LOG_LEVEL;

int getDebugLogLevel() TBAG_NOEXCEPT
{
    return g_debug_log_level;
}

void setDebugLogLevel(int level) TBAG_NOEXCEPT
{
    g_debug_log_level = level;
}

void setDebugLogLevelFromEnvs()
{
    std::string level_text;
    if (isFailure(libtbag::uvpp::getEnv(DEBUG_LOG_LEVEL_ENV_NAME, level_text))) {
        return;
    }

    int level;
    if (!libtbag::string::toVal(level_text, level)) {
        return;
    }

    g_debug_log_level = level;
}

void dlog(int level, char const * TBAG_RESTRICT text)
{
    if (level > g_debug_log_level) {
        return;
    }

    assert(text != nullptr);

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    static libtbag::lock::UvLock __lock__;
    __lock__.lock();
#endif

    ::fputs(text, stderr);

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    __lock__.unlock();
#endif
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

