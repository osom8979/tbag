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

bool parseAutoFlush(std::string const & flush_name)
{
    // @formatter:off
    std::string const VALUE = string::lower(flush_name);
    if (VALUE == TBAG_LOGGER_AUTO_FLUSH_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool parseMultiThread(std::string const & multithread_name)
{
    // @formatter:off
    std::string const VALUE = string::lower(multithread_name);
    if (VALUE == TBAG_LOGGER_MULTITHREAD_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool parseMutexThread(std::string const & mutex_name)
{
    // @formatter:off
    std::string const VALUE = string::lower(mutex_name);
    if (VALUE == TBAG_LOGGER_MUTEX_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

Severity parseSeverity(std::string const & severity_name)
{
    // @formatter:off
    std::string const NAME = string::upper(severity_name);
    if (NAME == EMERGENCY_SEVERITY.text) { return EMERGENCY_SEVERITY; }
    if (NAME ==     ALERT_SEVERITY.text) { return     ALERT_SEVERITY; }
    if (NAME ==  CRITICAL_SEVERITY.text) { return  CRITICAL_SEVERITY; }
    if (NAME ==     ERROR_SEVERITY.text) { return     ERROR_SEVERITY; }
    if (NAME ==   WARNING_SEVERITY.text) { return   WARNING_SEVERITY; }
    if (NAME ==    NOTICE_SEVERITY.text) { return    NOTICE_SEVERITY; }
    if (NAME ==      INFO_SEVERITY.text) { return      INFO_SEVERITY; }
    if (NAME ==     DEBUG_SEVERITY.text) { return     DEBUG_SEVERITY; }
    return OFF_SEVERITY;
    // @formatter:on
}

MakeType parseGeneratorType(std::string const & generator_name)
{
    // @formatter:off
    std::string const NAME = string::lower(generator_name);
    if (NAME == TBAG_LOGGER_GENERATOR_DEFAULT_COLOR) {
        return MakeType::DEFAULT_COLOR;
    }
    return MakeType::DEFAULT;
    // @formatter:on
}

Logger * createLogger(std::string const & name,
                      std::string const & lower_sink,
                      std::string const & destination,
                      std::string const & multithread,
                      std::string const & mutex,
                      std::string const & lower_generator,
                      std::string const & lower_severity,
                      std::string const & lower_flush)
{
    if (name.empty()) {
        return nullptr;
    }

    bool auto_flush     = parseAutoFlush(lower_flush);
    bool is_multithread = parseMultiThread(multithread);
    bool is_mutex       = parseMutexThread(mutex);

    Severity severity   = parseSeverity(lower_severity);
    MakeType generator  = parseGeneratorType(lower_generator);

    sink::SinkInterface * sink = nullptr;
    mgr::Logger * logger = nullptr;

    try {
        if (lower_sink == TBAG_LOGGER_SINK_COUT) {
            sink = new sink::CoutSink<std::mutex>(auto_flush);
        } else if (lower_sink == TBAG_LOGGER_SINK_FILE) {
            sink = new sink::FileSink<std::mutex>(destination, auto_flush);
        } else if (lower_sink == TBAG_LOGGER_SINK_ROTATE_FILE) {
            sink = new sink::RotateFileSink<std::mutex>(destination, auto_flush);
        } else {
            return nullptr;
        }
        logger = new mgr::Logger(sink, generator);
    } catch (...) {
        // @formatter:off
        if (  sink) { delete   sink; }
        if (logger) { delete logger; }
        return nullptr;
        // @formatter:on
    }

    return nullptr;
}

int createLoggerWithXmlConfigPath(std::string const & path)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR) {
        return false;
    }
    return createLoggerWithXmlElement(doc.FirstChildElement(TBAG_LOGGER_XML_NODE_NAME));
}

int createLoggerWithXmlString(std::string const & xml)
{
    tinyxml2::XMLDocument doc;
    if (doc.Parse(xml.c_str(), xml.size()) != tinyxml2::XML_NO_ERROR) {
        return false;
    }
    return createLoggerWithXmlElement(doc.FirstChildElement(TBAG_LOGGER_XML_NODE_NAME));
}

int createLoggerWithXmlElement(tinyxml2::XMLElement * element)
{
    if (element == nullptr || element->Name() != TBAG_LOGGER_XML_NODE_NAME) {
        return 0;
    }

    // Create logger
    auto * name_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_NAME);

    auto toString = [](tinyxml2::XMLElement * element) -> std::string {
        if (element) {
            return std::string(element->GetText());
        }
        return std::string();
    };

    bool is_create = false;
    std::string name = toString(name_element);
    if (name.empty() == false) {
        auto *        sink_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_SINK);
        auto * destination_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_DESTINATION);
        auto * multithread_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_MULTITHREAD);
        auto *       mutex_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_MUTEX);
        auto *   generator_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_GENERATOR);
        auto *    severity_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_SEVERITY);
        auto *  auto_flush_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_AUTO_FLUSH);

        is_create = createLogger(name,
                                 toString(sink_element),
                                 toString(destination_element),
                                 toString(multithread_element),
                                 toString(mutex_element),
                                 toString(generator_element),
                                 toString(severity_element),
                                 toString(auto_flush_element)) != nullptr;
    }

    // Next STEP.
    auto * next = element->NextSiblingElement();
    if (next) {
        return createLoggerWithXmlElement(next) + (is_create ? 1 : 0);
    }
    return (is_create ? 1 : 0);
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

