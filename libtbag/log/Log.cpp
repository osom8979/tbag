/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/details/LoggerManager.hpp>
#include <libtbag/log/sink/CoutSink.hpp>
#include <libtbag/log/sink/ColorCoutSink.hpp>
#include <libtbag/log/sink/RotateOfstreamSink.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LoggerManager = ::libtbag::log::details::LoggerManager;

Logger * createConsoleLogger(std::string const & name, bool auto_flush)
{
    try {
        using namespace ::libtbag::log::sink;
        Logger * logger = new Logger(new CoutSink<std::mutex>(auto_flush));
        LoggerManager::getInstance()->addLogger(name, logger);
        return logger;
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * createColorConsoleLogger(std::string const & name, bool auto_flush)
{
    try {
        using namespace ::libtbag::log::sink;
        Logger * logger = new Logger(new ColorCoutSink<std::mutex>(auto_flush));
        LoggerManager::getInstance()->addLogger(name, logger);
        return logger;
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
    try {
        using namespace ::libtbag::log::sink;
        Logger * logger = new Logger(new RotateOfstreamSink<std::mutex>(path, auto_flush));
        LoggerManager::getInstance()->addLogger(name, logger);
        return logger;
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * createDefaultConsoleLogger(bool auto_flush)
{
    return createConsoleLogger(TBAG_DEFAULT_LOGGER_NAME, auto_flush);
}

Logger * createDefaultColorConsoleLogger(bool auto_flush)
{
    return createColorConsoleLogger(TBAG_DEFAULT_LOGGER_NAME, auto_flush);
}

Logger * createDefaultFileLogger(std::string const & path, bool auto_flush)
{
    return createFileLogger(TBAG_DEFAULT_LOGGER_NAME, path, auto_flush);
}

void removeLogger(std::string const & name)
{
    LoggerManager::getInstance()->removeLogger(name);
}

void removeDefaultLogger()
{
    removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * getLogger(std::string const & name)
{
    try {
        return LoggerManager::getInstance()->getLoggerPtr(name);
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * getDefaultLogger()
{
    return getLogger(TBAG_DEFAULT_LOGGER_NAME);
}

void setLevel(std::string const & name, LogLevel level)
{
    Logger * logger = getLogger(name);
    if (logger != nullptr) {
        logger->setLogLevel(level);
    }
}

void setLevel(std::string const & name, int level)
{
    setLevel(name, static_cast<LogLevel>(level));
}

void setDefaultLevel(LogLevel level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
}

// ------------------------------------------
// DebuggingLoggerInitializer implementation.
// ------------------------------------------

DebuggingLoggerInitializer::DebuggingLoggerInitializer(LogLevel level, bool auto_flush)
        : LOGGER_NAME(TBAG_DEBUGGING_LOGGER_NAME)
{
    createColorConsoleLogger(LOGGER_NAME, auto_flush);
    setLevel(LOGGER_NAME, level);
}

DebuggingLoggerInitializer::~DebuggingLoggerInitializer()
{
    removeLogger(LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

