/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/mgr/LoggerManager.hpp>
#include <libtbag/log/sink/CoutSink.hpp>
#include <libtbag/log/sink/FileSink.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <mutex>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LoggerManager = ::libtbag::log::mgr::LoggerManager;

namespace impl {

template <typename SinkType, typename ... Args>
Logger * createLogger(std::string const & name, MakeType type, Args && ... args)
{
    try {
        auto * sink = new SinkType(std::forward<Args>(args) ...);
        auto * log  = new mgr::Logger(sink, type);
        if (LoggerManager::getInstance()->addLogger(name, log)) {
            return log;
        }
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

} // namespace impl {

Logger * createColorConsoleLogger(std::string const & name, bool auto_flush)
{
    return createConsoleLogger(name, MakeType::DEFAULT_COLOR, false, auto_flush);
}

Logger * createConsoleLogger(std::string const & name, MakeType type, bool mutex, bool auto_flush)
{
    if (mutex) {
        return impl::createLogger<sink::CoutSink<std::mutex> >(name, type, auto_flush);
    } else {
        return impl::createLogger<sink::CoutSink<lock::FakeLock> >(name, type, auto_flush);
    }
}

Logger * createFileLogger(std::string const & name,
                          std::string const & path,
                          MakeType type,
                          bool mutex,
                          bool auto_flush)
{
    if (mutex) {
        return impl::createLogger<sink::FileSink<std::mutex> >(name, type, path, auto_flush);
    } else {
        return impl::createLogger<sink::FileSink<lock::FakeLock> >(name, type, path, auto_flush);
    }
}

Logger * createRotateFileLogger(std::string const & name,
                                std::string const & path,
                                std::size_t max_size,
                                MakeType type,
                                bool mutex,
                                bool auto_flush)
{
    if (mutex) {
        return impl::createLogger<sink::RotateFileSink<std::mutex> >(name, type, path, max_size, auto_flush);
    } else {
        return impl::createLogger<sink::RotateFileSink<lock::FakeLock> >(name, type, path, max_size, auto_flush);
    }
}

Logger * createDefaultConsoleLogger(bool auto_flush)
{
    return createConsoleLogger(TBAG_DEFAULT_LOGGER_NAME, MakeType::DEFAULT, true, auto_flush);
}

Logger * createDefaultColorConsoleLogger(bool auto_flush)
{
    return createColorConsoleLogger(TBAG_DEFAULT_LOGGER_NAME, auto_flush);
}

Logger * createDefaultFileLogger(std::string const & path, bool auto_flush)
{
    return createFileLogger(TBAG_DEFAULT_LOGGER_NAME, path, MakeType::DEFAULT, true, auto_flush);
}

Logger * createDefaultRotateFileLogger(std::string const & path, std::size_t max_size, bool auto_flush)
{
    return createRotateFileLogger(TBAG_DEFAULT_LOGGER_NAME, path, max_size, MakeType::DEFAULT, true, auto_flush);
}

bool removeLogger(std::string const & name)
{
    return LoggerManager::getInstance()->removeLogger(name);
}

bool removeDefaultLogger()
{
    return removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * getLogger(std::string const & name)
{
    return LoggerManager::getInstance()->getLoggerPtr(name);
}

Logger * getDefaultLogger()
{
    return getLogger(TBAG_DEFAULT_LOGGER_NAME);
}

bool existsLogger(std::string const & name)
{
    return getLogger(name) != nullptr;
}

void setSeverity(std::string const & name, Severity level)
{
    Logger * logger = getLogger(name);
    if (logger != nullptr) {
        logger->setSeverity(level);
    }
}

void setDefaultSeverity(Severity level)
{
    setSeverity(TBAG_DEFAULT_LOGGER_NAME, level);
}

Severity getSeverity(std::string const & name)
{
    Logger * logger = getLogger(name);
    if (logger != nullptr) {
        return logger->getSeverity();
    }
    return OFF_SEVERITY;
}

Severity getDefaultSeverity()
{
    return getSeverity(TBAG_DEFAULT_LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

