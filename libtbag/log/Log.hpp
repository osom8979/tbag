/**
 * @file   Log.hpp
 * @brief  Log class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton.hpp>

#include <atomic>

#define SPDLOG_TRACE_ON // [DEV-ONLY] TRACE LOGGING.
#define SPDLOG_DEBUG_ON // [DEV-ONLY] DEBUG LOGGING.
#include <libtbag/log/spdlog/spdlog.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

constexpr char const * const DEFAULT_LOGGER_OF_CONSOLE = "_C";
constexpr char const * const DEFAULT_LOGGER_OF_FILE = "_F";
constexpr char const * const DEFAULT_LOGGER_FORMAT = "%L [%Y-%m-%dT%H:%M:%S.%e] [%t] %v";

constexpr bool isAsynchronousLogging() noexcept
{
    return true;
}

/**
 * Log class prototype.
 *
 * @author zer0
 * @date   2016-04-04
 */
class Log : SINGLETON_INHERITANCE(Log)
{
public:
    SINGLETON_RESTRICT(Log);

public:
    /**
     * @remarks
     *  Conflict issue: DEBUG(stdlib), ERR(NCurses) symbol.
     */
    enum LogLevel
    {
        LOG_LEVEL_TRACE     = spdlog::level::level_enum::trace    ,
        LOG_LEVEL_DEBUG     = spdlog::level::level_enum::debug    ,
        LOG_LEVEL_INFO      = spdlog::level::level_enum::info     ,
        LOG_LEVEL_NOTICE    = spdlog::level::level_enum::notice   ,
        LOG_LEVEL_WARNING   = spdlog::level::level_enum::warn     ,
        LOG_LEVEL_ERROR     = spdlog::level::level_enum::err      ,
        LOG_LEVEL_CRITICAL  = spdlog::level::level_enum::critical ,
        LOG_LEVEL_ALERT     = spdlog::level::level_enum::alert    ,
        LOG_LEVEL_EMERGENCY = spdlog::level::level_enum::emerg    ,
        LOG_LEVEL_OFF       = spdlog::level::level_enum::off      ,
    };

public:
    using Logger = std::shared_ptr<spdlog::logger>;

private:
    std::atomic_bool _enable_console;
    std::atomic_bool _enable_file;

    Logger _console;
    Logger _file;

public:
    inline Logger getConsole() const noexcept {
        return this->_console;
    }

    inline Logger getFile() const noexcept {
        return this->_file;
    }

public:
    inline void setEnableConsole(bool enable = true) noexcept {
        this->_enable_console.store(enable);
    }

    inline void setEnableFile(bool enable = true) noexcept {
        this->_enable_file.store(enable);
    }

public:
    inline void setLevel(LogLevel level) noexcept {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    }

    inline void setDefaultLevel() noexcept {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(LOG_LEVEL_INFO));
    }

    inline void setFormat(std::string const & format) noexcept {
        spdlog::set_pattern(format);
    }

    inline void setDefaultFormat() noexcept {
        spdlog::set_pattern(DEFAULT_LOGGER_FORMAT);
    }

    void initConsole() {
        if (isAsynchronousLogging()) {
            this->_console = spdlog::stdout_logger_mt(DEFAULT_LOGGER_OF_CONSOLE);
        } else {
            this->_console = spdlog::stdout_logger_st(DEFAULT_LOGGER_OF_CONSOLE);
        }
    }

    void initFile(std::string const & path) {
        if (isAsynchronousLogging()) {
            this->_file = spdlog::daily_logger_mt(DEFAULT_LOGGER_OF_FILE, path, 0, 0);
        } else {
            this->_file = spdlog::daily_logger_st(DEFAULT_LOGGER_OF_FILE, path, 0, 0);
        }
    }

public:
    inline bool isInitConsole() const noexcept {
        return (this->_console.get() != nullptr ? true : false);
    }

    inline bool isInitFile() const noexcept {
        return (this->_file.get() != nullptr ? true : false);
    }

public:
#ifndef __SPDLOG_LOGGING_IMPLEMENT
#define __SPDLOG_LOGGING_IMPLEMENT(level)               \
    template <typename... Args>                         \
    void level(const char* fmt, const Args&... args) {  \
        if (this->_enable_console.load()) {             \
            this->_console->level(fmt, args...);        \
        }                                               \
        if (this->_enable_file.load()) {                \
            this->_file->level(fmt, args...);           \
        }                                               \
    }
#endif

public:
    __SPDLOG_LOGGING_IMPLEMENT(trace   );
    __SPDLOG_LOGGING_IMPLEMENT(debug   );
    __SPDLOG_LOGGING_IMPLEMENT(info    );
    __SPDLOG_LOGGING_IMPLEMENT(notice  );
    __SPDLOG_LOGGING_IMPLEMENT(warn    );
    __SPDLOG_LOGGING_IMPLEMENT(error   );
    __SPDLOG_LOGGING_IMPLEMENT(critical);
    __SPDLOG_LOGGING_IMPLEMENT(alert   );
    __SPDLOG_LOGGING_IMPLEMENT(emerg   );
};

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(NDEBUG)
# define _TLOG(...)
# define _DLOG(...)
#else
# define _TLOG(...)  \
    SPDLOG_TRACE(::libtbag::Log::getInstance()->getConsole(), __VA_ARGS__);  \
    SPDLOG_TRACE(::libtbag::Log::getInstance()->getFile(),    __VA_ARGS__);
# define _DLOG(...)  \
    SPDLOG_DEBUG(::libtbag::Log::getInstance()->getConsole(), __VA_ARGS__);  \
    SPDLOG_DEBUG(::libtbag::Log::getInstance()->getFile(),    __VA_ARGS__);
#endif

#if defined(NDEBUG)
# define _DIRECT_CONSOLE_LOG(format, ...)
#else
# include <cstdio>
# define _DIRECT_CONSOLE_LOG(format, ...) \
    std::fprintf(stdout, "[%s] " format, __LOCATION__, __VA_ARGS__)
#endif

#include <libtbag/predef.hpp>
#if !defined(NDEBUG) && defined(__OS_WINDOWS__)
# include <windows.h>
# define _DIRECT_DEBUG_VIEW_LOG(msg) OutputDebugStringA(msg)
#else
# define _DIRECT_DEBUG_VIEW_LOG(msg)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

