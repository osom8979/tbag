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
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/log/Severity.hpp>
#include <libtbag/log/Logger.hpp>
#include <libtbag/log/LoggerManager.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

TBAG_CONSTEXPR char const * const TBAG_DEFAULT_LOGGER_NAME = "tbag";

// clang-format off
TBAG_API Logger * createDefaultRawStdoutLogger();
TBAG_API Logger * createDefaultColorStdoutLogger();
TBAG_API Logger * createDefaultStdoutLogger();
TBAG_API Logger * createDefaultFileLogger(std::string const & path);
TBAG_API bool     removeDefaultLogger();
TBAG_API bool     existsDefaultLogger();
TBAG_API Logger * getDefaultLogger();
TBAG_API void     setDefaultSeverity(Severity const & level);
TBAG_API void     setDefaultLevel(int level);
TBAG_API Severity getDefaultSeverity();
TBAG_API void     setDefaultAutoFlush(bool flag = true);
TBAG_API bool     getDefaultAutoFlush();
// clang-format on

/**
 * SeverityGuard class.
 *
 * @author zer0
 * @date   2017-05-09
 */
class SeverityGuard : private Noncopyable
{
public:
    LoggerId const ID;
    Severity const SAVE;

public:
    explicit SeverityGuard(LoggerId const & id, Severity const & save, Severity const & current) : ID(id), SAVE(save)
    { setSeverity(id, current); }

    SeverityGuard(LoggerId const & id, Severity const & severity) : SeverityGuard(id, getSeverity(id), severity)
    { /* EMPTY. */ }
    SeverityGuard(LoggerId const & id) : SeverityGuard(id, OFF_SEVERITY)
    { /* EMPTY. */ }

    SeverityGuard(std::string const & name, Severity const & severity) : SeverityGuard(getLoggerId(name), severity)
    { /* EMPTY. */ }
    SeverityGuard(std::string const & name) : SeverityGuard(name, OFF_SEVERITY)
    { /* EMPTY. */ }

    SeverityGuard() : SeverityGuard(TBAG_DEFAULT_LOGGER_NAME)
    { /* EMPTY. */ }

public:
    ~SeverityGuard()
    { setSeverity(ID, SAVE); }
};

// clang-format off
template <typename ... Args> inline void emergency(char const * f, Args && ... args) { emergency(getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void alert    (char const * f, Args && ... args) { alert    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void critical (char const * f, Args && ... args) { critical (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void error    (char const * f, Args && ... args) { error    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void warning  (char const * f, Args && ... args) { warning  (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void notice   (char const * f, Args && ... args) { notice   (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void info     (char const * f, Args && ... args) { info     (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void debug    (char const * f, Args && ... args) { debug    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
// clang-format on

// clang-format off
template <typename ... Args> inline void emergency(std::string const & f, Args && ... args) { emergency(getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void alert    (std::string const & f, Args && ... args) { alert    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void critical (std::string const & f, Args && ... args) { critical (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void error    (std::string const & f, Args && ... args) { error    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void warning  (std::string const & f, Args && ... args) { warning  (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void notice   (std::string const & f, Args && ... args) { notice   (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void info     (std::string const & f, Args && ... args) { info     (getDefaultLogger(), f, std::forward<Args>(args) ...); }
template <typename ... Args> inline void debug    (std::string const & f, Args && ... args) { debug    (getDefaultLogger(), f, std::forward<Args>(args) ...); }
// clang-format on

TBAG_API void __debug_message(char const * TBAG_RESTRICT text);

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#if defined(DISABLE_TBAG_LOG)
# define tLog(name, level, ...)
#else
# define tLog(name, level, ...)  ::libtbag::log::logging(::libtbag::log::getLogger(name), level, __VA_ARGS__)
#endif

#define tLogM(name, ...)  tLog(name, ::libtbag::log::EMERGENCY_SEVERITY, __VA_ARGS__)
#define tLogA(name, ...)  tLog(name, ::libtbag::log::ALERT_SEVERITY    , __VA_ARGS__)
#define tLogC(name, ...)  tLog(name, ::libtbag::log::CRITICAL_SEVERITY , __VA_ARGS__)
#define tLogE(name, ...)  tLog(name, ::libtbag::log::ERROR_SEVERITY    , __VA_ARGS__)
#define tLogW(name, ...)  tLog(name, ::libtbag::log::WARNING_SEVERITY  , __VA_ARGS__)
#define tLogN(name, ...)  tLog(name, ::libtbag::log::NOTICE_SEVERITY   , __VA_ARGS__)
#define tLogI(name, ...)  tLog(name, ::libtbag::log::INFO_SEVERITY     , __VA_ARGS__)
#define tLogD(name, ...)  tLog(name, ::libtbag::log::DEBUG_SEVERITY    , __VA_ARGS__)

#define tDLogM(...)  tLogM(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogA(...)  tLogA(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogC(...)  tLogC(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogE(...)  tLogE(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogW(...)  tLogW(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogN(...)  tLogN(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogI(...)  tLogI(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogD(...)  tLogD(::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

#if defined(DISABLE_TBAG_LOG)
# define tLogIf(condition, name, level, ...)
#else
# define tLogIf(condition, name, level, ...) do { if (condition) { tLog(name, level, __VA_ARGS__); } } while (false)
#endif

#define tLogIfM(c, name, ...)  tLogIf(c, name, ::libtbag::log::EMERGENCY_SEVERITY, __VA_ARGS__)
#define tLogIfA(c, name, ...)  tLogIf(c, name, ::libtbag::log::ALERT_SEVERITY    , __VA_ARGS__)
#define tLogIfC(c, name, ...)  tLogIf(c, name, ::libtbag::log::CRITICAL_SEVERITY , __VA_ARGS__)
#define tLogIfE(c, name, ...)  tLogIf(c, name, ::libtbag::log::ERROR_SEVERITY    , __VA_ARGS__)
#define tLogIfW(c, name, ...)  tLogIf(c, name, ::libtbag::log::WARNING_SEVERITY  , __VA_ARGS__)
#define tLogIfN(c, name, ...)  tLogIf(c, name, ::libtbag::log::NOTICE_SEVERITY   , __VA_ARGS__)
#define tLogIfI(c, name, ...)  tLogIf(c, name, ::libtbag::log::INFO_SEVERITY     , __VA_ARGS__)
#define tLogIfD(c, name, ...)  tLogIf(c, name, ::libtbag::log::DEBUG_SEVERITY    , __VA_ARGS__)

#define tDLogIfM(c, ...)  tLogIfM(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfA(c, ...)  tLogIfA(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfC(c, ...)  tLogIfC(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfE(c, ...)  tLogIfE(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfW(c, ...)  tLogIfW(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfN(c, ...)  tLogIfN(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfI(c, ...)  tLogIfI(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tDLogIfD(c, ...)  tLogIfD(c, ::libtbag::log::TBAG_DEFAULT_LOGGER_NAME, __VA_ARGS__)

/**
 * @def ENABLE_TBAG_LIBRARY_DEBUGGING_LOG
 *
 * @warning
 *  Don't use this macros from user level developers.
 */
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
# include <libtbag/string/fmt/format.h>
# include <libtbag/tty/Tces.hpp>
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 5
#  define __tbag_profile(...) ::libtbag::log::__debug_message(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_WHITE "P/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_profile(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 4
#  define __tbag_debug(...) ::libtbag::log::__debug_message(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_BLUE "D/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_debug(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 3
#  define __tbag_info(...) ::libtbag::log::__debug_message(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_GREEN "I/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_info(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 2
#  define __tbag_warning(...) ::libtbag::log::__debug_message(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_YELLOW "W/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_warning(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 1
#  define __tbag_error(...) ::libtbag::log::__debug_message(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_RED "E/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_error(...)
# endif
#else
# define __tbag_profile(...)
# define __tbag_debug(...)
# define __tbag_info(...)
# define __tbag_warning(...)
# define __tbag_error(...)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOG_HPP__

