/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/mgr/LoggerManager.hpp>
#include <libtbag/log/sink/CoutSink.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>

#include <mutex>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LoggerManager = ::libtbag::log::mgr::LoggerManager;
using MakeType      = ::libtbag::log::msg::PacketGenerator::MakeType;

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

Logger * createConsoleLogger(std::string const & name, bool auto_flush)
{
    using SinkType = sink::CoutSink<std::mutex>;
    return impl::createLogger<SinkType>(name, MakeType::DEFAULT, auto_flush);
}

Logger * createColorConsoleLogger(std::string const & name, bool auto_flush)
{
    using SinkType = sink::CoutSink<std::mutex>;
    return impl::createLogger<SinkType>(name, MakeType::DEFAULT_COLOR, auto_flush);
}

Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
    using SinkType = sink::FileSink<std::mutex>;
    return impl::createLogger<SinkType>(name, MakeType::DEFAULT, path, auto_flush);
}

Logger * createRotateFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
    using SinkType = sink::RotateFileSink<std::mutex>;
    return impl::createLogger<SinkType>(name, MakeType::DEFAULT, path, auto_flush);
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

