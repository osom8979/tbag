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

#include <libtbag/log/sink/CallbackSink.hpp>
#include <libtbag/log/sink/ConsoleSink.hpp>
#include <libtbag/log/sink/FileSink.hpp>
#include <libtbag/log/sink/NullSink.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

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

bool existsSinkName(std::string const & name)
{
    auto const LOWER = libtbag::string::lower(libtbag::string::trim(name));
    if (LOWER == SINK_CONSOLE) {
        return true;
    } else if (LOWER == SINK_FILE) {
        return true;
    } else if (LOWER == SINK_NULL) {
        return true;
    }
    return false;
}

bool existsGeneratorName(std::string const & name)
{
    auto const LOWER = libtbag::string::lower(libtbag::string::trim(name));
    if (LOWER == GENERATOR_DEFAULT) {
        return true;
    } else if (LOWER == GENERATOR_DEFAULT_COLOR) {
        return true;
    } else if (LOWER == GENERATOR_RAW) {
        return true;
    }
    return false;
}

Logger::SharedSink newSink(std::string const & name, std::string const & dest, std::string const & args)
{
    auto const LOWER = libtbag::string::lower(libtbag::string::trim(name));
    if (LOWER == SINK_CONSOLE) {
        return std::make_shared<ConsoleSink>(dest);
    } else if (LOWER == SINK_FILE) {
        return std::make_shared<FileSink>(dest);
    } else if (LOWER == SINK_NULL) {
        return std::make_shared<NullSink>();
    } else {
        return Logger::SharedSink(nullptr);
    }
}

Logger::SharedGenerator newGenerator(std::string const & name, std::string const & line_feed)
{
    auto const LOWER = libtbag::string::lower(libtbag::string::trim(name));
    if (LOWER == GENERATOR_DEFAULT) {
        return std::make_shared<DefaultGenerator>(line_feed);
    } else if (LOWER == GENERATOR_DEFAULT_COLOR) {
        return std::make_shared<DefaultColorGenerator>(line_feed);
    } else if (LOWER == GENERATOR_RAW) {
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

    auto sink = newSink(params.sink, envs.convert(params.destination), envs.convert(params.arguments));
    assert(sink);
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

