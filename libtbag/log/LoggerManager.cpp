/**
 * @file   LoggerManager.cpp
 * @brief  LoggerManager class implementation.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Move namespace: log/mgr -> log)
 */

#include <libtbag/log/LoggerManager.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <libtbag/log/msg/DefaultColorGenerator.hpp>
#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/log/msg/RawGenerator.hpp>

#include <libtbag/log/sink/ConsoleSink.hpp>
#include <libtbag/log/sink/FileSink.hpp>
#include <libtbag/log/sink/NullSink.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/log/sink/StringQueueSink.hpp>
#include <libtbag/log/sink/ThreadSink.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

namespace __static_assert_test__ {
using namespace libtbag::string;
using namespace libtbag::log::sink;
static_assert(string_equal(ConsoleSink::sink_name(), SINK_CONSOLE), "ConsoleSink name mismatch.");
static_assert(string_equal(FileSink::sink_name(), SINK_FILE), "FileSink name mismatch.");
static_assert(string_equal(NullSink::sink_name(), SINK_NULL), "NullSink name mismatch.");
static_assert(string_equal(RotateFileSink::sink_name(), SINK_ROTATE_FILE), "RotateFileSink name mismatch.");
} // namespace __static_assert_test__

LoggerManager::LoggerId LoggerManager::getLoggerId(std::string const & name)
{
    return std::hash<std::string>()(name);
}

bool LoggerManager::addLogger(LoggerId id, Logger * new_logger)
{
    return _loggers.insert(std::make_pair(id, UniqueLogger(new_logger))).second;
}

bool LoggerManager::addLogger(std::string const & name, Logger * new_logger)
{
    return addLogger(getLoggerId(name), new_logger);
}

bool LoggerManager::removeLogger(LoggerId id)
{
    return _loggers.erase(id) == 1;
}

bool LoggerManager::removeLogger(std::string const & name)
{
    return removeLogger(getLoggerId(name));
}

Logger * LoggerManager::getLoggerPtr(LoggerId id)
{
    // Don't use the _logs.at() method.
    // This is to avoid the std::out_of_range exception.

    auto itr = _loggers.find(id);
    if (itr == _loggers.end()) {
        return nullptr;
    }
    return itr->second.get();
}

Logger * LoggerManager::getLoggerPtr(std::string const & name)
{
    return getLoggerPtr(getLoggerId(name));
}

// -----------------------
// Miscellaneous utilities
// -----------------------

Logger * createLogger(std::string const & name,
                      Logger::SharedSink const & sink,
                      Logger::SharedGenerator const & generator,
                      int level,
                      bool auto_flush)
{
    auto * manager = LoggerManager::getInstance();
    assert(manager != nullptr);

    if (manager->getLoggerPtr(name) != nullptr) {
        manager->removeLogger(name);
    }
    assert(manager->getLoggerPtr(name) == nullptr);

    Logger * logger = nullptr;
    try {
        logger = new Logger(name, sink, generator, level, auto_flush);
    } catch (...) {
        return nullptr;
    }

    if (manager->addLogger(name, logger)) {
        return logger;
    } else {
        delete logger;
        return nullptr;
    }
}

using namespace libtbag::log::msg;
using namespace libtbag::log::sink;

using LineFeedStyle = libtbag::string::LineFeedStyle;

TBAG_CONSTEXPR static int const DEFAULT_LOG_LEVEL = INFO_LEVEL;
TBAG_CONSTEXPR static bool const DEFAULT_AUTO_FLUSH = false;

Logger * createRawStdoutLogger(std::string const & name)
{
    auto sink = std::make_shared<ConsoleSink>(ConsoleSink::ConsoleType::CT_STDOUT);
    auto gen = std::make_shared<RawGenerator>(LineFeedStyle::LFS_UNIX);
    assert(sink);
    assert(gen);
    return createLogger(name, sink, gen, DEFAULT_LOG_LEVEL, DEFAULT_AUTO_FLUSH);
}

Logger * createColorStdoutLogger(std::string const & name)
{
    auto sink = std::make_shared<ConsoleSink>(ConsoleSink::ConsoleType::CT_STDOUT);
    auto gen = std::make_shared<DefaultColorGenerator>(LineFeedStyle::LFS_UNIX);
    assert(sink);
    assert(gen);
    return createLogger(name, sink, gen, DEFAULT_LOG_LEVEL, DEFAULT_AUTO_FLUSH);
}

Logger * createStdoutLogger(std::string const & name)
{
    auto sink = std::make_shared<ConsoleSink>(ConsoleSink::ConsoleType::CT_STDOUT);
    auto gen = std::make_shared<DefaultGenerator>(LineFeedStyle::LFS_UNIX);
    assert(sink);
    assert(gen);
    return createLogger(name, sink, gen, DEFAULT_LOG_LEVEL, DEFAULT_AUTO_FLUSH);
}

Logger * createFileLogger(std::string const & name, std::string const & path)
{
    auto sink = std::make_shared<FileSink>(path.c_str());
    auto gen = std::make_shared<DefaultGenerator>(LineFeedStyle::LFS_UNIX);
    assert(sink);
    assert(gen);
    return createLogger(name, sink, gen, DEFAULT_LOG_LEVEL, DEFAULT_AUTO_FLUSH);
}

bool removeLogger(LoggerId id)
{
    return LoggerManager::getInstance()->removeLogger(id);
}

bool removeLogger(std::string const & name)
{
    return removeLogger(getLoggerId(name));
}

bool removeLogger(Logger const * logger)
{
    assert(logger != nullptr);
    return removeLogger(logger->NAME);
}

Logger * getLogger(LoggerId id)
{
    return LoggerManager::getInstance()->getLoggerPtr(id);
}

Logger * getLogger(std::string const & name)
{
    return getLogger(getLoggerId(name));
}

bool existsLogger(LoggerId id)
{
    return getLogger(id) != nullptr;
}

bool existsLogger(std::string const & name)
{
    return getLogger(name) != nullptr;
}

void setSeverity(LoggerId id, Severity const & level)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setSeverity(level);
    }
}

void setSeverity(std::string const & name, Severity const & level)
{
    setSeverity(getLoggerId(name), level);
}

void setLevel(LoggerId id, int level)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setLevel(level);
    }
}

void setLevel(std::string const & name, int level)
{
    setLevel(getLoggerId(name), level);
}

Severity getSeverity(LoggerId id)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        return logger->getSeverity();
    }
    return OFF_SEVERITY;
}

Severity getSeverity(std::string const & name)
{
    return getSeverity(getLoggerId(name));
}

void setAutoFlush(LoggerId id, bool flag)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setAutoFlush(flag);
    }
}

void setAutoFlush(std::string const & name, bool flag)
{
    setAutoFlush(getLoggerId(name), flag);
}

bool getAutoFlush(LoggerId id)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        return logger->getAutoFlush();
    }
    return false;
}

bool getAutoFlush(std::string const & name)
{
    return getAutoFlush(getLoggerId(name));
}

bool existsSinkName(std::string const & name)
{
    using namespace libtbag::log::sink;
    auto const lower = libtbag::string::lower(libtbag::string::trim(name));
    if (lower == ConsoleSink::sink_name()) {
        return true;
    } else if (lower == FileSink::sink_name()) {
        return true;
    } else if (lower == NullSink::sink_name()) {
        return true;
    } else if (lower == RotateFileSink::sink_name()) {
        return true;
    } else if (lower == StringQueueSink::sink_name()) {
        return true;
    }
    return false;
}

bool existsGeneratorName(std::string const & name)
{
    auto const lower = libtbag::string::lower(libtbag::string::trim(name));
    if (lower == GENERATOR_DEFAULT) {
        return true;
    } else if (lower == GENERATOR_DEFAULT_COLOR) {
        return true;
    } else if (lower == GENERATOR_RAW) {
        return true;
    }
    return false;
}

Logger::SharedSink newSink(std::string const & name, std::string const & args)
{
    auto const lower = libtbag::string::lower(libtbag::string::trim(name));
    if (lower == ConsoleSink::sink_name()) {
        return std::make_shared<ConsoleSink>(args);
    } else if (lower == FileSink::sink_name()) {
        return std::make_shared<FileSink>(args);
    } else if (lower == NullSink::sink_name()) {
        return std::make_shared<NullSink>();
    } else if (lower == RotateFileSink::sink_name()) {
        return std::make_shared<RotateFileSink>(args);
    } else if (lower == StringQueueSink::sink_name()) {
        return std::make_shared<StringQueueSink>(args);
    } else {
        return Logger::SharedSink(nullptr);
    }
}

Logger::SharedSink newThreadSink(std::string const & name, std::string const & args)
{
    return std::make_shared<ThreadSink>(newSink(name, args));
}

Logger::SharedGenerator newGenerator(std::string const & name, std::string const & line_feed)
{
    auto const lower = libtbag::string::lower(libtbag::string::trim(name));
    if (lower == GENERATOR_DEFAULT) {
        return std::make_shared<DefaultGenerator>(line_feed);
    } else if (lower == GENERATOR_DEFAULT_COLOR) {
        return std::make_shared<DefaultColorGenerator>(line_feed);
    } else if (lower == GENERATOR_RAW) {
        return std::make_shared<RawGenerator>(line_feed);
    } else {
        return Logger::SharedGenerator(nullptr);
    }
}

Logger * createLogger(LoggerInitParams const & params, libtbag::string::Environments const & envs)
{
    if (params.name.empty()) {
        return nullptr;
    }
    if (!existsSinkName(params.sink)) {
        return nullptr;
    }
    if (!existsGeneratorName(params.generator)) {
        return nullptr;
    }

    bool enable_thread_logging = false;
    libtbag::string::toVal(params.thread, enable_thread_logging);

    auto const converted_arguments = envs.convert(params.arguments);
    Logger::SharedSink sink;
    if (enable_thread_logging) {
        sink = newThreadSink(params.sink, converted_arguments);
    } else {
        sink = newSink(params.sink, converted_arguments);
    }
    if (!sink) {
        return nullptr;
    }

    auto gen = newGenerator(params.generator, params.line_feed);
    auto const name = libtbag::string::trim(params.name);
    auto const severity = findSeverity(params.severity);
    auto const auto_flush = libtbag::string::toValue<bool>(params.auto_flush, false);
    return createLogger(name, sink, gen, int(severity), auto_flush);
}

Logger * createLogger(LoggerInitParams const & params)
{
    return createLogger(params, libtbag::string::Environments::createDefaultEnvironments());
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

