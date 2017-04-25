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

Logger * createColorConsoleLogger(std::string const & name, bool auto_flush)
{
    return createConsoleLogger(name, MakeType::DEFAULT_COLOR, false, auto_flush);
}

Logger * createConsoleLogger(std::string const & name, bool auto_flush)
{
    return createConsoleLogger(name, MakeType::DEFAULT, false, auto_flush);
}

Logger * createConsoleLogger(std::string const & name, MakeType type, bool mutex, bool auto_flush)
{
    if (mutex) {
        return impl::createLogger<sink::CoutSink<std::mutex> >(name, type, auto_flush);
    } else {
        return impl::createLogger<sink::CoutSink<lock::FakeLock> >(name, type, auto_flush);
    }
}

Logger * createFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
    return createFileLogger(name, path, MakeType::DEFAULT, false, auto_flush);
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

Logger * createRotateFileLogger(std::string const & name, std::string const & path, bool auto_flush)
{
    return createRotateFileLogger(name, path, MakeType::DEFAULT, false, auto_flush);
}

Logger * createRotateFileLogger(std::string const & name,
                                std::string const & path,
                                MakeType type,
                                bool mutex,
                                bool auto_flush)
{
    if (mutex) {
        return impl::createLogger<sink::RotateFileSink<std::mutex> >(name, type, path, auto_flush);
    } else {
        return impl::createLogger<sink::RotateFileSink<lock::FakeLock> >(name, type, path, auto_flush);
    }
}

bool parseAutoFlush(std::string const & flush_value)
{
    // @formatter:off
    std::string const VALUE = string::lower(flush_value);
    if (VALUE == TBAG_LOGGER_AUTO_FLUSH_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool parseMultiThread(std::string const & multithread_value)
{
    // @formatter:off
    std::string const VALUE = string::lower(multithread_value);
    if (VALUE == TBAG_LOGGER_MULTITHREAD_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool parseMutexThread(std::string const & mutex_value)
{
    // @formatter:off
    std::string const VALUE = string::lower(mutex_value);
    if (VALUE == TBAG_LOGGER_MUTEX_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

Severity parseSeverity(std::string const & severity_value)
{
    // @formatter:off
    std::string const VALUE = string::upper(severity_value);
    if (VALUE == EMERGENCY_SEVERITY.text) { return EMERGENCY_SEVERITY; }
    if (VALUE ==     ALERT_SEVERITY.text) { return     ALERT_SEVERITY; }
    if (VALUE ==  CRITICAL_SEVERITY.text) { return  CRITICAL_SEVERITY; }
    if (VALUE ==     ERROR_SEVERITY.text) { return     ERROR_SEVERITY; }
    if (VALUE ==   WARNING_SEVERITY.text) { return   WARNING_SEVERITY; }
    if (VALUE ==    NOTICE_SEVERITY.text) { return    NOTICE_SEVERITY; }
    if (VALUE ==      INFO_SEVERITY.text) { return      INFO_SEVERITY; }
    if (VALUE ==     DEBUG_SEVERITY.text) { return     DEBUG_SEVERITY; }
    return OFF_SEVERITY;
    // @formatter:on
}

MakeType parseGeneratorType(std::string const & generator_value)
{
    // @formatter:off
    std::string const VALUE = string::lower(generator_value);
    if (VALUE == TBAG_LOGGER_GENERATOR_DEFAULT_COLOR) {
        return MakeType::DEFAULT_COLOR;
    }
    return MakeType::DEFAULT;
    // @formatter:on
}

Logger * createLogger(std::string const & name,
                      std::string const & sink_value,
                      std::string const & destination_value,
                      std::string const & multithread_value,
                      std::string const & mutex_value,
                      std::string const & generator_value,
                      std::string const & severity_value,
                      std::string const & flush_value)
{
    if (name.empty()) {
        return nullptr;
    }

    bool auto_flush     = parseAutoFlush(flush_value);
    bool is_multithread = parseMultiThread(multithread_value);
    bool is_mutex       = parseMutexThread(mutex_value);

    Severity severity   = parseSeverity(severity_value);
    MakeType generator  = parseGeneratorType(generator_value);

    if (sink_value == TBAG_LOGGER_SINK_COUT) {
        return createConsoleLogger(name, generator, is_mutex, auto_flush);
    } else if (sink_value == TBAG_LOGGER_SINK_FILE) {
        return createFileLogger(name, destination_value, generator, is_mutex, auto_flush);
    } else if (sink_value == TBAG_LOGGER_SINK_ROTATE_FILE) {
        return createRotateFileLogger(name, destination_value, generator, is_mutex, auto_flush);
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

int createLoggerWithParentXmlElement(tinyxml2::XMLElement const * parent)
{
    return createLoggerWithXmlElement(parent->FirstChildElement(TBAG_LOGGER_XML_NODE_NAME));
}

int createLoggerWithXmlElement(tinyxml2::XMLElement const * element)
{
    if (element == nullptr || string::lower(element->Name()) != TBAG_LOGGER_XML_NODE_NAME) {
        return 0;
    }

    // Create logger
    auto * name_element = element->FirstChildElement(TBAG_LOGGER_XML_ELEMENT_NAME);

    auto toString = [](tinyxml2::XMLElement const * element) -> std::string {
        if (element != nullptr && element->GetText() != nullptr) {
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

bool saveLoggerWithXmlElement(tinyxml2::XMLElement * parent,
                              std::string const & name,
                              std::string const & sink_value,
                              std::string const & destination_value,
                              std::string const & multithread_value,
                              std::string const & mutex_value,
                              std::string const & generator_value,
                              std::string const & severity_value,
                              std::string const & flush_value)
{
    auto * doc = parent->GetDocument();
    auto *    tbag_log_element = doc->NewElement(TBAG_LOGGER_XML_NODE_NAME);
    auto *        name_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_NAME);
    auto *        sink_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_SINK);
    auto * destination_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_DESTINATION);
    auto * multithread_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_MULTITHREAD);
    auto *       mutex_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_MUTEX);
    auto *   generator_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_GENERATOR);
    auto *    severity_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_SEVERITY);
    auto *  auto_flush_element = doc->NewElement(TBAG_LOGGER_XML_ELEMENT_AUTO_FLUSH);

           name_element->SetText(name.c_str());
           sink_element->SetText(sink_value.c_str());
    destination_element->SetText(destination_value.c_str());
    multithread_element->SetText(multithread_value.c_str());
          mutex_element->SetText(mutex_value.c_str());
      generator_element->SetText(generator_value.c_str());
       severity_element->SetText(severity_value.c_str());
     auto_flush_element->SetText(flush_value.c_str());

    tbag_log_element->InsertEndChild(name_element);
    tbag_log_element->InsertEndChild(sink_element);
    tbag_log_element->InsertEndChild(destination_element);
    tbag_log_element->InsertEndChild(multithread_element);
    tbag_log_element->InsertEndChild(mutex_element);
    tbag_log_element->InsertEndChild(generator_element);
    tbag_log_element->InsertEndChild(severity_element);
    tbag_log_element->InsertEndChild(auto_flush_element);
    parent->InsertEndChild(tbag_log_element);

    return true;
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

