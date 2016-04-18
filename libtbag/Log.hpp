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

#include <atomic>

#define SPDLOG_TRACE_ON // [DEV-ONLY] TRACE LOGGING.
#define SPDLOG_DEBUG_ON // [DEV-ONLY] DEBUG LOGGING.
#include <spdlog/spdlog.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

constexpr char const * const DEFAULT_LOGGER_OF_CONSOLE = "_C";
constexpr char const * const DEFAULT_LOGGER_OF_FILE = "_F";
constexpr char const * const DEFAULT_LOGGER_FORMAT = "%L [%Y-%m-%dT%H:%M:%S.%e] [%t] %v";

constexpr bool IsAsynchronousLogging() noexcept
{
    return true;
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
    enum class Level : int
    {
        TRACE    = spdlog::level::level_enum::trace    ,
        DBG      = spdlog::level::level_enum::debug    ,
        INFO     = spdlog::level::level_enum::info     ,
        NOTICE   = spdlog::level::level_enum::notice   ,
        WARN     = spdlog::level::level_enum::warn     ,
        ERR      = spdlog::level::level_enum::err      ,
        CRITICAL = spdlog::level::level_enum::critical ,
        ALERT    = spdlog::level::level_enum::alert    ,
        EMERG    = spdlog::level::level_enum::emerg    ,
        OFF      = spdlog::level::level_enum::off      ,
    };

public:
    using Logger = std::shared_ptr<spdlog::logger>;

private:
    std::atomic_bool _enable_console;
    std::atomic_bool _enable_file;

    Logger _console;
    Logger _file;

public:
    inline Logger get_console() const noexcept {
        return this->_console;
    }

    inline Logger get_file() const noexcept {
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
    inline void setLevel(Level level) noexcept {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    }

    inline void setDefaultLevel() noexcept {
        spdlog::set_level(static_cast<spdlog::level::level_enum>(Level::INFO));
    }

    inline void setFormat(std::string const & format) noexcept {
        spdlog::set_pattern(format);
    }

    inline void setDefaultFormat() noexcept {
        spdlog::set_pattern(DEFAULT_LOGGER_FORMAT);
    }

    void initConsole() {
        if (IsAsynchronousLogging()) {
            this->_console = spdlog::stdout_logger_mt(DEFAULT_LOGGER_OF_CONSOLE);
        } else {
            this->_console = spdlog::stdout_logger_st(DEFAULT_LOGGER_OF_CONSOLE);
        }
    }

    void initFile(std::string const & path) {
        if (IsAsynchronousLogging()) {
            this->_file = spdlog::daily_logger_mt(DEFAULT_LOGGER_OF_FILE, path, 0, 0);
        } else {
            this->_file = spdlog::daily_logger_st(DEFAULT_LOGGER_OF_FILE, path, 0, 0);
        }
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
    __SPDLOG_LOGGING_IMPLEMENT(trace);
    __SPDLOG_LOGGING_IMPLEMENT(debug);
    __SPDLOG_LOGGING_IMPLEMENT(info);
    __SPDLOG_LOGGING_IMPLEMENT(notice);
    __SPDLOG_LOGGING_IMPLEMENT(warn);
    __SPDLOG_LOGGING_IMPLEMENT(error);
    __SPDLOG_LOGGING_IMPLEMENT(critical);
    __SPDLOG_LOGGING_IMPLEMENT(alert);
    __SPDLOG_LOGGING_IMPLEMENT(emerg);
};

SINGLETON_IMPLEMENT(Log);

Log::Log()
{
    __EMPTY_BLOCK__
}

Log::~Log()
{
    __EMPTY_BLOCK__
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef NDEBUG
#define _TLOG(...)  \
    SPDLOG_TRACE(::libtbag::Log::getInstance()->get_console(), __VA_ARGS__);  \
    SPDLOG_TRACE(::libtbag::Log::getInstance()->get_file(),    __VA_ARGS__);
#define _DLOG(...)  \
    SPDLOG_DEBUG(::libtbag::Log::getInstance()->get_console(), __VA_ARGS__);  \
    SPDLOG_DEBUG(::libtbag::Log::getInstance()->get_file(),    __VA_ARGS__);
#else
#define _TLOG(...)
#define _DLOG(...)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_HPP__

