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
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/tty/Tces.hpp>

#include <cstdio>
#include <cassert>

#include <sstream>

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

    setDebugLogLevel(level);
}

void dlog(int level, char const * file, int line, std::string const & text)
{
    if (_te_level <= COMPARE_AND(level) <= _tp_level) {
        // OK.
    } else {
        return;
    }
    if (level > g_debug_log_level) {
        return;
    }

    std::stringstream ss;
    if (isWindowsPlatform()) {
        // clang-format off
        switch (level) {
        case _tp_level: ss << "[P] "; break;
        case _td_level: ss << "[D] "; break;
        case _ti_level: ss << "[I] "; break;
        case _tw_level: ss << "[W] "; break;
        case _te_level: ss << "[E] "; break;
        default: break;
        }
        // clang-format on
    } else {
        // clang-format off
        switch (level) {
        case _tp_level: ss << libtbag::tty::DISPLAY_ATTRIBUTE_FG_MAGENTA << "[P] "; break;
        case _td_level: ss << libtbag::tty::DISPLAY_ATTRIBUTE_FG_BLUE    << "[D] "; break;
        case _ti_level: ss << libtbag::tty::DISPLAY_ATTRIBUTE_FG_GREEN   << "[I] "; break;
        case _tw_level: ss << libtbag::tty::DISPLAY_ATTRIBUTE_FG_YELLOW  << "[W] "; break;
        case _te_level: ss << libtbag::tty::DISPLAY_ATTRIBUTE_FG_RED     << "[E] "; break;
        default: break;
        }
        // clang-format on
    }

    ss << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << " " << file << ":" << line << " "
       << text;

    if (isWindowsPlatform()) {
        ss << libtbag::string::WINDOWS_NEW_LINE;
    } else {
        ss << libtbag::tty::DISPLAY_ATTRIBUTE_RESET << libtbag::string::UNIX_NEW_LINE;
    }

    auto const log_message = ss.str();

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    static libtbag::lock::UvLock __lock__;
    __lock__.lock();
#endif

    ::fputs(log_message.c_str(), stderr);

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    __lock__.unlock();
#endif
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

