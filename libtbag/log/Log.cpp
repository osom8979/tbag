/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/mgr/LoggerManager.hpp>
#include <libtbag/log/sink/CoutSink.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LoggerManager = ::libtbag::log::mgr::LoggerManager;

Logger * createConsoleLogger(std::string const & name, bool auto_flush)
{
//    try {
//        using namespace ::libtbag::log::sink;
//        Logger * logger = new Logger(new CoutSink<std::mutex>(auto_flush));
//        LoggerManager::getInstance()->addLogger(name, logger);
//        return logger;
//    } catch (...) {
//        // EMPTY.
//    }
    return nullptr;
}

Logger * createColorConsoleLogger(std::string const & name, bool auto_flush)
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

Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
//    try {
//        using namespace ::libtbag::log::sink;
//        Logger * logger = new Logger(new RotateOfstreamSink<std::mutex>(path, auto_flush));
//        LoggerManager::getInstance()->addLogger(name, logger);
//        return logger;
//    } catch (...) {
//        // EMPTY.
//    }
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

void setLevel(std::string const & name, Severity level)
{
    Logger * logger = getLogger(name);
    if (logger != nullptr) {
        logger->setSeverity(level);
    }
}

void setDefaultLevel(Severity level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

