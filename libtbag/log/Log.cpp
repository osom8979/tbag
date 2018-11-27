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

Logger * createRawStdoutLogger(std::string const & name, bool mutex, bool auto_flush)
{
    return createStdoutLogger(name, MakeType::RAW, mutex, auto_flush);
}

Logger * createColorStdoutLogger(std::string const & name, bool mutex, bool auto_flush)
{
    return createStdoutLogger(name, MakeType::DEFAULT_COLOR, mutex, auto_flush);
}

Logger * createStdoutLogger(std::string const & name, MakeType type, bool mutex, bool auto_flush)
{
    using     MutexCoutSink = sink::CoutSink<std::mutex>;
    using FakeMutexCoutSink = sink::CoutSink<lock::FakeLock>;

    if (mutex) {
        return impl::createLogger<MutexCoutSink>(name, type, auto_flush);
    } else {
        return impl::createLogger<FakeMutexCoutSink>(name, type, auto_flush);
    }
}

Logger * createFileLogger(std::string const & name, std::string const & path,
                          MakeType type, bool mutex, bool auto_flush)
{
    using     MutexFileSink = sink::FileSink<std::mutex>;
    using FakeMutexFileSink = sink::FileSink<lock::FakeLock>;

    if (mutex) {
        return impl::createLogger<MutexFileSink>(name, type, path, auto_flush);
    } else {
        return impl::createLogger<FakeMutexFileSink>(name, type, path, auto_flush);
    }
}

Logger * createRotateFileLogger(std::string const & name, std::string const & path,
                                std::size_t max_size, std::size_t max_file_count,
                                MakeType type, bool mutex, bool auto_flush)
{
    using     MutexRotateFileSink = sink::RotateFileSink<std::mutex>;
    using FakeMutexRotateFileSink = sink::RotateFileSink<lock::FakeLock>;

    if (mutex) {
        return impl::createLogger<MutexRotateFileSink>(name, type, path, max_size, max_file_count, auto_flush);
    } else {
        return impl::createLogger<FakeMutexRotateFileSink>(name, type, path, max_size, max_file_count, auto_flush);
    }
}

Logger * createDefaultRawStdoutLogger(bool mutex, bool auto_flush)
{
    return createRawStdoutLogger(TBAG_DEFAULT_LOGGER_NAME, mutex, auto_flush);
}

Logger * createDefaultColorStdoutLogger(bool mutex, bool auto_flush)
{
    return createColorStdoutLogger(TBAG_DEFAULT_LOGGER_NAME, mutex, auto_flush);
}

Logger * createDefaultStdoutLogger(bool mutex, bool auto_flush)
{
    return createStdoutLogger(TBAG_DEFAULT_LOGGER_NAME, MakeType::DEFAULT, mutex, auto_flush);
}

Logger * createDefaultFileLogger(std::string const & path, bool mutex, bool auto_flush)
{
    return createFileLogger(TBAG_DEFAULT_LOGGER_NAME, path, MakeType::DEFAULT, mutex, auto_flush);
}

Logger * createDefaultRotateFileLogger(std::string const & path, std::size_t max_size, std::size_t max_file_count,
                                       bool mutex, bool auto_flush)
{
    return createRotateFileLogger(TBAG_DEFAULT_LOGGER_NAME, path, max_size, max_file_count,
                                  MakeType::DEFAULT, mutex, auto_flush);
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

void setLevel(std::string const & name, int level)
{
    setSeverity(name, libtbag::log::level::getSeverityWithLevelStep(level));
}

void setDefaultLevel(int level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
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

