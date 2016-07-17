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

using LogMgr = ::libtbag::log::details::LoggerManager;
using Logger = ::libtbag::log::details::Logger;

Logger * createDefaultConsoleLogger()
{
    try {
        Logger * logger = new Logger(new ::libtbag::log::sink::CoutSink<std::mutex>(true));
        LogMgr::getInstance()->addLogger(TBAG_DEFAULT_LOGGER_NAME, logger);
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
        return LogMgr::getInstance()->getLoggerPtr(TBAG_DEFAULT_LOGGER_NAME);
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

void setDefaultLevel(LogLevel level)
{
}

void removeDefaultLogger()
{
    LogMgr::getInstance()->removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

