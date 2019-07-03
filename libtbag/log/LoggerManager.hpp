/**
 * @file   LoggerManager.hpp
 * @brief  LoggerManager class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGERMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGERMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/log/Logger.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/dom/tinyxml2/tinyxml2.h>

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

/**
 * LoggerManager class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 *
 * @remarks
 *  - Fix the C2280 error in MSVC14: Requires inheritance Noncopyable class.
 */
class TBAG_API LoggerManager : public libtbag::pattern::Singleton2<LoggerManager>
{
public:
    SINGLETON2_PROTOTYPE(LoggerManager);

public:
    using LoggerId = std::size_t;
    using UniqueLogger = std::unique_ptr<Logger>;
    using Loggers = std::unordered_map<LoggerId, UniqueLogger>;

private:
    Loggers _loggers;

public:
    static LoggerId getLoggerId(std::string const & name);

public:
    bool addLogger(LoggerId id, Logger * new_logger);
    bool addLogger(std::string const & name, Logger * new_logger);

public:
    template <typename ... Args>
    bool createLogger(LoggerId id, Args && ... args)
    {
        return addLogger(id, new Logger(std::forward<Args>(args) ...));
    }

    template <typename ... Args>
    bool createLogger(std::string const & name, Args && ... args)
    {
        return addLogger(name, new Logger(std::forward<Args>(args) ...));
    }

public:
    bool removeLogger(LoggerId id);
    bool removeLogger(std::string const & name);

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_loggers.size())
    { return _loggers.size(); }

    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_loggers.empty())
    { return _loggers.empty(); }

public:
    Logger * getLoggerPtr(LoggerId id);
    Logger * getLoggerPtr(std::string const & name);
};

// -----------------------
// Miscellaneous utilities
// -----------------------

/**
 * Create new logger.
 *
 * @return
 *  Returns a NULL pointer if it already exists or fails to create.
 */
TBAG_API Logger * createLogger(std::string const & name,
                               Logger::Sink * sink,
                               Logger::Generator * generator = nullptr,
                               int level = libtbag::log::INFO_LEVEL,
                               bool auto_flush = false);

TBAG_API Logger * createRawStdoutLogger(std::string const & name);
TBAG_API Logger * createColorStdoutLogger(std::string const & name);
TBAG_API Logger * createStdoutLogger(std::string const & name);
TBAG_API Logger * createFileLogger(std::string const & name, std::string const & path);

TBAG_CONSTEXPR char const * const SINK_CONSOLE = "console";
TBAG_CONSTEXPR char const * const SINK_FILE    = "file";
TBAG_CONSTEXPR char const * const SINK_NULL    = "null";

TBAG_CONSTEXPR char const * const GENERATOR_DEFAULT       = "default";
TBAG_CONSTEXPR char const * const GENERATOR_DEFAULT_COLOR = "default_color";
TBAG_CONSTEXPR char const * const GENERATOR_RAW           = "raw";

struct LoggerInitParams
{
    std::string name;
    std::string sink;
    std::string destination;
    std::string arguments;
    std::string generator;
    std::string line_feed;
    std::string severity;
    std::string auto_flush;
};

TBAG_API bool existsSinkName(std::string const & name);
TBAG_API bool existsGeneratorName(std::string const & name);

TBAG_API Logger::SharedSink newSink(std::string const & name, std::string const & dest, std::string const & args);
TBAG_API Logger::SharedGenerator newGenerator(std::string const & name, std::string const & line_feed);

TBAG_API Logger * createLogger(LoggerInitParams const & params, libtbag::string::Environments const & envs);
TBAG_API Logger * createLogger(LoggerInitParams const & params);

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LOGGERMANAGER_HPP__

