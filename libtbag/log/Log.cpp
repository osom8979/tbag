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

Logger * createDefaultConsoleLogger()
{
    try {
        using namespace ::libtbag::log::sink;
        Logger * logger = new Logger(new CoutSink<std::mutex>(true));
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

Logger * getDefaultLogger()
{
    try {
        return LoggerManager::getInstance()->getLoggerPtr(TBAG_DEFAULT_LOGGER_NAME);
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

void setDefaultLevel(LogLevel level)
{
    try {
        Logger * logger = LoggerManager::getInstance()->getLoggerPtr(TBAG_DEFAULT_LOGGER_NAME);
        if (logger != nullptr) {
            logger->setLogLevel(level);
        }
    } catch (...) {
        // EMPTY.
    }
}

void removeDefaultLogger()
{
    LoggerManager::getInstance()->removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

