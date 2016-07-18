/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/details/LoggerManager.hpp>
#include <libtbag/log/sink/CoutSink.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LoggerManager = ::libtbag::log::details::LoggerManager;

Logger * createDefaultConsoleLogger(bool auto_flush)
{
    try {
        using namespace ::libtbag::log::sink;
        Logger * logger = new Logger(new CoutSink<std::mutex>(auto_flush));
        LoggerManager::getInstance()->addLogger(TBAG_DEFAULT_LOGGER_NAME, logger);
        return logger;
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * createDefaultFileLogger(std::string const & path, bool auto_flush)
{
    try {
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
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

void setDefaultLevel(LogLevel level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
}

void removeDefaultLogger()
{
    LoggerManager::getInstance()->removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

