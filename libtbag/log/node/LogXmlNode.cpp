/**
 * @file   LogXmlNode.cpp
 * @brief  LogXmlNode class implementation.
 * @author zer0
 * @date   2017-04-25
 */

#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

// --------------------------
// LogXmlNode implementation.
// --------------------------

LogXmlNode::LogXmlNode() : _envs(createDefaultEnvironments())
{
    // EMPTY.
}

LogXmlNode::~LogXmlNode()
{
    // EMPTY.
}

std::string LogXmlNode::name() const
{
    return XML_ELEMENT_TLOG_NAME;
}

void LogXmlNode::setup()
{
    _infos.clear();
}

void LogXmlNode::teardown()
{
    _infos.clear();
}

void LogXmlNode::load(Element const & element)
{
    _infos = loadLogInfo(element);
}

void LogXmlNode::save(Element & element) const
{
    saveLogInfo(element, _infos);
}

int LogXmlNode::createLoggers()
{
    int count = 0;
    for (auto & info : _infos) {
        if (LogXmlNode::createLogger(info, _envs) != nullptr) {
            ++count;
        }
    }
    return count;
}

int LogXmlNode::createLoggers(std::string & preview_message)
{
    int const RESULT = createLoggers();
    if (RESULT >= 1) {
        auto const NAMES = getNames();
        std::size_t const NAMES_SIZE = NAMES.size();
        assert(NAMES_SIZE >= 1);

        std::stringstream ss;
        ss << "Create loggers: " << NAMES[0];
        for (std::size_t i = 1; i < NAMES_SIZE; ++i) {
            ss << "," << NAMES[i];
        }
        preview_message = ss.str();
    } else {
        preview_message = "Not found logger";
    }
    return RESULT;
}

int LogXmlNode::removeLoggers()
{
    int count = 0;
    for (auto & info : _infos) {
        if (info.name.empty() == false && removeLogger(info.name)) {
            ++count;
        }
    }
    return count;
}

LogXmlNode::StringVector LogXmlNode::getNames() const
{
    StringVector names;
    for (auto & info : _infos) {
        names.push_back(info.name);
    }
    return names;
}

// ---------------
// Static methods.
// ---------------

std::size_t LogXmlNode::parseMaxSize(std::string const & value)
{
    try {
        return static_cast<std::size_t>(std::stoi(value));
    } catch (...) {
        // EMPTY.
    }
    return 0;
}

bool LogXmlNode::parseAutoFlush(std::string const & value)
{
    // @formatter:off
    std::string const LOWER_VALUE = string::lower(value);
    if (LOWER_VALUE == AUTO_FLUSH_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool LogXmlNode::parseMultiThread(std::string const & value)
{
    // @formatter:off
    std::string const LOWER_VALUE = string::lower(value);
    if (LOWER_VALUE == MULTITHREAD_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

bool LogXmlNode::parseMutex(std::string const & value)
{
    // @formatter:off
    std::string const LOWER_VALUE = string::lower(value);
    if (LOWER_VALUE == MUTEX_ON) {
        return true;
    }
    return false;
    // @formatter:on
}

LogXmlNode::Severity LogXmlNode::parseSeverity(std::string const & value)
{
    // @formatter:off
    static Severity const S_EMERGENCY = libtbag::log::level::    EMERGENCY_SEVERITY;
    static Severity const S_ALERT     = libtbag::log::level::        ALERT_SEVERITY;
    static Severity const S_CRITICAL  = libtbag::log::level::     CRITICAL_SEVERITY;
    static Severity const S_ERROR     = libtbag::log::level::        ERROR_SEVERITY;
    static Severity const S_WARNING   = libtbag::log::level::      WARNING_SEVERITY;
    static Severity const S_MOTICE    = libtbag::log::level::       NOTICE_SEVERITY;
    static Severity const S_INFO      = libtbag::log::level::INFORMATIONAL_SEVERITY;
    static Severity const S_DEBUG     = libtbag::log::level::        DEBUG_SEVERITY;
    // @formatter:on

    std::string const UPPER_VALUE = string::upper(value);

    // @formatter:off
    if (UPPER_VALUE.compare(S_EMERGENCY.getText()) == 0) { return S_EMERGENCY; }
    if (UPPER_VALUE.compare(S_ALERT    .getText()) == 0) { return S_ALERT    ; }
    if (UPPER_VALUE.compare(S_CRITICAL .getText()) == 0) { return S_CRITICAL ; }
    if (UPPER_VALUE.compare(S_ERROR    .getText()) == 0) { return S_ERROR    ; }
    if (UPPER_VALUE.compare(S_WARNING  .getText()) == 0) { return S_WARNING  ; }
    if (UPPER_VALUE.compare(S_MOTICE   .getText()) == 0) { return S_MOTICE   ; }
    if (UPPER_VALUE.compare(S_INFO     .getText()) == 0) { return S_INFO     ; }
    if (UPPER_VALUE.compare(S_DEBUG    .getText()) == 0) { return S_DEBUG    ; }
    return OFF_SEVERITY;
    // @formatter:on
}

LogXmlNode::MakeType LogXmlNode::parseGeneratorType(std::string const & value)
{
    // @formatter:off
    std::string const LOWER_VALUE = string::lower(value);
    if (LOWER_VALUE == GENERATOR_DEFAULT_COLOR) {
        return MakeType::DEFAULT_COLOR;
    }
    return MakeType::DEFAULT;
    // @formatter:on
}

std::string LogXmlNode::getElementText(Element const * element)
{
    if (element != nullptr && element->GetText() != nullptr) {
        return std::string(element->GetText());
    }
    return std::string();
}

LogXmlNode::LogInfo LogXmlNode::getLogInfo(
        std::string const & name,
        std::string const & sink,
        std::string const & destination,
        std::string const & max_size,
        std::string const & auto_flush,
        std::string const & multithread,
        std::string const & mutex,
        std::string const & severity,
        std::string const & generator)
{
    LogInfo result;
    result.name        = name;
    result.sink        = sink;
    result.destination = destination;
    result.max_size    = parseMaxSize(max_size);
    result.auto_flush  = parseAutoFlush(auto_flush);
    result.multithread = parseMultiThread(multithread);
    result.mutex       = parseMutex(mutex);
    result.severity    = parseSeverity(severity);
    result.generator   = parseGeneratorType(generator);
    return result;
}

LogXmlNode::LogInfo LogXmlNode::getLogInfo(Element const & element)
{
    return getLogInfo(getElementText(element.FirstChildElement(XML_ELEMENT_NAME)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_SINK)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_DESTINATION)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_MAX_SIZE)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_AUTO_FLUSH)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_MULTITHREAD)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_MUTEX)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_SEVERITY)),
                      getElementText(element.FirstChildElement(XML_ELEMENT_GENERATOR)));
}

LogXmlNode::LogInfoVector LogXmlNode::loadLogInfo(Element const & parent)
{
    //if (string::lower(parent.Name()) != XML_ELEMENT_TLOG_NAME) {
    //    return LogInfoVector();
    //}

    LogInfoVector result;
    Element const * cursor = parent.FirstChildElement(XML_ELEMENT_LOGGER_NAME);
    while (cursor != nullptr) {
        result.push_back(getLogInfo(*cursor));
        cursor = cursor->NextSiblingElement();
    }
    return result;
}

bool LogXmlNode::insertName(Element & parent, std::string const & name)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_NAME);
    element->SetText(name.c_str());
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertSink(Element & parent, std::string const & sink)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_SINK);
    element->SetText(sink.c_str());
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertDestination(Element & parent, std::string const & destination)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_DESTINATION);
    element->SetText(destination.c_str());
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertMaxSize(Element & parent, std::size_t max_size)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_MAX_SIZE);
    element->SetText((unsigned int)max_size);
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertAutoFlush(Element & parent, bool auto_flush)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_AUTO_FLUSH);
    element->SetText(auto_flush);
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertMultiThread(Element & parent, bool multithread)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_MULTITHREAD);
    element->SetText(multithread);
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertMutex(Element & parent, bool mutex)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_MUTEX);
    element->SetText(mutex);
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertSeverity(Element & parent, Severity severity)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_SEVERITY);
    element->SetText(severity.getText());
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::insertGeneratorType(Element & parent, MakeType type)
{
    auto * doc = parent.GetDocument();
    auto * element = doc->NewElement(XML_ELEMENT_GENERATOR);

    std::string generator;
    if (type == MakeType::DEFAULT) {
        generator = GENERATOR_DEFAULT;
    } else if (type == MakeType::DEFAULT_COLOR) {
        generator = GENERATOR_DEFAULT_COLOR;
    }

    element->SetText(generator.c_str());
    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::saveLogInfo(Element & parent, LogInfo const & info)
{
    auto * doc = parent.GetDocument();
    if (doc == nullptr) {
        return false;
    }

    auto * element = doc->NewElement(XML_ELEMENT_LOGGER_NAME);
    if (element == nullptr) {
        return false;
    }

    // @formatter:off
    insertName         (*element, info.name);
    insertSink         (*element, info.sink);
    insertDestination  (*element, info.destination);
    insertMaxSize      (*element, info.max_size);
    insertAutoFlush    (*element, info.auto_flush);
    insertMultiThread  (*element, info.multithread);
    insertMutex        (*element, info.mutex);
    insertSeverity     (*element, info.severity);
    insertGeneratorType(*element, info.generator);
    // @formatter:on

    return parent.InsertEndChild(element) != nullptr;
}

bool LogXmlNode::saveLogInfo(Element & parent, LogInfoVector const & infos)
{
    bool result = true;
    for (auto & cursor : infos) {
        result &= saveLogInfo(parent, cursor);
    }
    return result;
}

LogXmlNode::Logger * LogXmlNode::createLogger(LogInfo const & info, bool recreate_if_exists)
{
    return createLogger(info, createDefaultEnvironments(), recreate_if_exists);
}

LogXmlNode::Logger * LogXmlNode::createLogger(LogInfo const & info, Environments const & envs, bool recreate_if_exists)
{
    if (info.name.empty()) {
        return nullptr;
    }

    std::string const NAME = info.name;
    std::string const DEST = envs.convert(info.destination);

    Logger * logger = getLogger(NAME);
    if (logger != nullptr) {
        if (recreate_if_exists) {
            removeLogger(NAME);
        } else {
            return logger;
        }
    }

    if (info.sink == SINK_COUT) {
        logger = createConsoleLogger(NAME, info.generator, info.mutex, info.auto_flush);
    } else if (info.sink == SINK_FILE) {
        logger = createFileLogger(NAME, DEST, info.generator, info.mutex, info.auto_flush);
    } else if (info.sink == SINK_ROTATE_FILE) {
        logger = createRotateFileLogger(NAME, DEST, info.max_size, info.generator, info.mutex, info.auto_flush);
    } else {
        return nullptr;
    }

    if (logger != nullptr) {
        logger->setSeverity(info.severity);
    }
    return logger;
}

LogXmlNode::Environments LogXmlNode::createDefaultEnvironments()
{
    Environments envs;
    envs.push(EnvFlag(ENVS_EXE_PATH, filesystem::Path::getExePath()));
    envs.push(EnvFlag(ENVS_EXE_DIR , filesystem::Path::getExeDir()));
    envs.push(EnvFlag(ENVS_WORK_DIR, filesystem::Path::getWorkDir()));
    envs.push(EnvFlag(ENVS_HOME_DIR, filesystem::Path::getHomeDir()));
    return envs;
}

// ------------------------
// Function implementation.
// ------------------------

int createLoggerWithXmlConfigPath(std::string const & path)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR) {
        return false;
    }

    auto * element = doc.FirstChildElement(LogXmlNode::XML_ELEMENT_TLOG_NAME);
    if (element == nullptr) {
        return 0;
    }
    return createLoggerWithXmlLoggerElement(*element);
}

int createLoggerWithXmlString(std::string const & xml)
{
    tinyxml2::XMLDocument doc;
    if (doc.Parse(xml.c_str(), xml.size()) != tinyxml2::XML_NO_ERROR) {
        return false;
    }

    auto * element = doc.FirstChildElement(LogXmlNode::XML_ELEMENT_TLOG_NAME);
    if (element == nullptr) {
        return 0;
    }
    return createLoggerWithXmlLoggerElement(*element);
}

int createLoggerWithXmlElement(tinyxml2::XMLElement const & parent)
{
    auto * element = parent.FirstChildElement(LogXmlNode::XML_ELEMENT_TLOG_NAME);
    if (element == nullptr) {
        return 0;
    }
    return createLoggerWithXmlLoggerElement(*element);
}

int createLoggerWithXmlLoggerElement(tinyxml2::XMLElement const & element)
{
    int result = 0;
    auto * cursor = element.FirstChildElement(LogXmlNode::XML_ELEMENT_LOGGER_NAME);
    while (cursor != nullptr) {
        if (LogXmlNode::createLogger(LogXmlNode::getLogInfo(*cursor)) != nullptr) {
            ++result;
        }
        cursor = cursor->NextSiblingElement();
    }
    return result;
}

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

