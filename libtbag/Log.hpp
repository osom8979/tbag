/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton.hpp>

#include <memory>

//#define SPDLOG_TRACE_ON // [DEV-ONLY] TRACE LOGGING.
//#define SPDLOG_DEBUG_ON // [DEV-ONLY] DEBUG LOGGING.
#include <spdlog/spdlog.h>

#ifndef __LOG_CONFIG__MULTI_THREAD__
#define __LOG_CONFIG__MULTI_THREAD__ 0
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

constexpr char const * const GetConsoleLogger() noexcept(true)
{
    return "CONSOLE";
}

/**
 * Log class prototype.
 *
 * @author zer0
 * @date   2016-04-04
 */
class Log : public Noncopyable
{
public:
    SINGLETON_PROTOTYPE(Log);

public:
    using Logger = std::shared_ptr<spdlog::logger>;

private:
    Logger _console;

public:
    static Logger getConsole() {
        return getInstance()->_console;
    }
};

SINGLETON_IMPLEMENT(Log);

Log::Log()
{
    _console = spdlog::stdout_logger_st(GetConsoleLogger());
}

Log::~Log()
{
    // Empty.
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_HPP__

