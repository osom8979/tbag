/**
 * @file   Logger.hpp
 * @brief  Logger class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/log/Severity.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/log/msg/Generator.hpp>

#include <libtbag/string/fmt/format.h>
#include <libtbag/string/fmt/ostream.h>

#include <atomic>
#include <memory>
#include <string>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

/**
 * Logger class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */
class TBAG_API Logger : private Noncopyable
{
public:
    using Sink       = libtbag::log::sink::Sink;
    using SharedSink = std::shared_ptr<Sink>;
    using WeakedSink = std::weak_ptr<Sink>;

    using Generator       = libtbag::log::msg::Generator;
    using SharedGenerator = std::shared_ptr<Generator>;
    using WeakedGenerator = std::weak_ptr<Generator>;

public:
    std::string const NAME;

private:
    SharedSink      _sink;
    SharedGenerator _generator;

private:
    std::atomic_int  _level;
    std::atomic_bool _auto_flush;

public:
    Logger(std::string const & name,
           SharedSink const & sink,
           SharedGenerator const & generator,
           int level = libtbag::log::INFO_LEVEL,
           bool auto_flush = false);
    ~Logger();

public:
    WeakedSink getWeakedSink() const TBAG_NOEXCEPT_SP_OP(WeakedSink(_sink))
    { return WeakedSink(_sink); }
    WeakedGenerator getWeakedGenerator() const TBAG_NOEXCEPT_SP_OP(WeakedGenerator(_generator))
    { return WeakedGenerator(_generator); }

public:
    inline int getLevel() const TBAG_NOEXCEPT_SP_OP(_level.load())
    { return _level.load(); }
    inline void setLevel(int level) TBAG_NOEXCEPT_SP_OP(_level.store(level))
    { _level.store(level); }

    inline Severity getSeverity() const TBAG_NOEXCEPT_SP_OP(_level.load())
    { return getSeverityWithLevelStep(_level.load()); }
    inline void setSeverity(Severity const & severity) TBAG_NOEXCEPT_SP_OP(_level.store(int(severity)))
    { _level.store(int(severity)); }

public:
    inline bool getAutoFlush() const TBAG_NOEXCEPT_SP_OP(_auto_flush.load())
    { return _auto_flush.load(); }
    inline void setAutoFlush(bool flag = true) TBAG_NOEXCEPT_SP_OP(_auto_flush.store(flag))
    { _auto_flush.store(flag); }

public:
    inline bool youShallNotPass(int level) const TBAG_NOEXCEPT_SP_OP(_level.load())
    {
        auto const threshold = _level.load();
        return threshold == OFF_LEVEL || !Severity::isContain(threshold, level);
    }

public:
    void flush() const;

public:
    bool write(int level, char const * level_name, char const * message, int size, bool generate) const;
    bool write(int level, char const * level_name, char const * message, int size) const;
    bool write(int level, char const * level_name, char const * message) const;
    bool write(int level, char const * level_name, std::string const & message) const;

public:
    bool write(int level, char const * message, int size) const;
    bool write(int level, char const * message) const;
    bool write(int level, std::string const & message) const;

public:
    bool write(Severity const & severity, char const * message, int size) const;
    bool write(Severity const & severity, char const * message) const;
    bool write(Severity const & severity, std::string const & message) const;

public:
    bool raw(Severity const & severity, char const * message, int size) const;
    bool raw(Severity const & severity, char const * message) const;
    bool raw(Severity const & severity, std::string const & message) const;

public:
    template <typename FormatT, typename ... Args>
    bool format(Severity const & severity, FormatT && format, Args && ... args) const
    {
        if (youShallNotPass(severity)) {
            return true;
        }
        return write(severity, ::fmt::format(std::forward<FormatT>(format), std::forward<Args>(args) ...));
    }

public:
    template <typename F, typename ... A> inline
    bool emergency(F && f, A && ... a) const
    {
        return format(EMERGENCY_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool alert(F && f, A && ... a) const
    {
        return format(ALERT_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool critical(F && f, A && ... a) const
    {
        return format(CRITICAL_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool error(F && f, A && ... a) const
    {
        return format(ERROR_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool warning(F && f, A && ... a) const
    {
        return format(WARNING_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool notice(F && f, A && ... a) const
    {
        return format(NOTICE_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool info(F && f, A && ... a) const
    {
        return format(INFO_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }

    template <typename F, typename ... A> inline
    bool debug(F && f, A && ... a) const
    {
        return format(DEBUG_SEVERITY, std::forward<F>(f), std::forward<A>(a) ...);
    }
};

template <typename FormatT, typename ... Args>
inline bool logging(Logger const * logger, Severity level, FormatT && format, Args && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->format(level, std::forward<FormatT>(format), std::forward<Args>(args) ...);
}

template <typename F, typename ... A>
inline bool emergency(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->emergency(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool alert(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->alert(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool critical(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->critical(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool error(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->error(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool warning(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->warning(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool notice(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->notice(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool info(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->info(std::forward<F>(format), std::forward<A>(args) ...);
}

template <typename F, typename ... A>
inline bool debug(Logger const * logger, F && format, A && ... args)
{
    if (!logger) {
        return false;
    }
    return logger->debug(std::forward<F>(format), std::forward<A>(args) ...);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGER_HPP__

