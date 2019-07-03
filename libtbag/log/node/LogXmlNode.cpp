/**
 * @file   LogXmlNode.cpp
 * @brief  LogXmlNode class implementation.
 * @author zer0
 * @date   2017-04-25
 */

#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

LogXmlNode::LogXmlNode() : _envs(Environments::createDefaultEnvironments())
{
    // EMPTY.
}

LogXmlNode::~LogXmlNode()
{
    // EMPTY.
}

std::string LogXmlNode::name() const
{
    return XML_ELEMENT_LOGGERS_NAME;
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
    _infos = getLogInfos(element);
}

void LogXmlNode::save(Element & element) const
{
    for (auto & info : _infos) {
        newElement(element, XML_ELEMENT_LOGGER_NAME, [&](Element & e){
            // clang-format off
            set(e, XML_ELEMENT_NAME       , info.name       );
            set(e, XML_ELEMENT_SINK       , info.sink       );
            set(e, XML_ELEMENT_DESTINATION, info.destination);
            set(e, XML_ELEMENT_ARGUMENTS  , info.arguments  );
            set(e, XML_ELEMENT_GENERATOR  , info.generator  );
            set(e, XML_ELEMENT_LINE_FEED  , info.line_feed  );
            set(e, XML_ELEMENT_SEVERITY   , info.severity   );
            set(e, XML_ELEMENT_AUTO_FLUSH , info.auto_flush );
            // clang-format on
        });
    }
}

std::vector<std::string> LogXmlNode::getInfoNames() const
{
    std::vector<std::string> names;
    for (auto & info : _infos) {
        names.push_back(info.name);
    }
    return names;
}

int LogXmlNode::createLoggers()
{
    int count = 0;
    for (auto & info : _infos) {
        if (libtbag::log::createLogger(info, _envs) != nullptr) {
            ++count;
        }
    }
    return count;
}

int LogXmlNode::createLoggers(std::string & preview_message)
{
    int const RESULT = createLoggers();
    if (RESULT >= 1) {
        auto const NAMES = getInfoNames();
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

LogXmlNode::Info LogXmlNode::getLogInfo(Element const & element)
{
    Info info;
    // clang-format off
    info.name        = get<std::string>(element, XML_ELEMENT_NAME       );
    info.sink        = get<std::string>(element, XML_ELEMENT_SINK       );
    info.destination = get<std::string>(element, XML_ELEMENT_DESTINATION);
    info.arguments   = get<std::string>(element, XML_ELEMENT_ARGUMENTS  );
    info.generator   = get<std::string>(element, XML_ELEMENT_GENERATOR  );
    info.line_feed   = get<std::string>(element, XML_ELEMENT_LINE_FEED  );
    info.severity    = get<std::string>(element, XML_ELEMENT_SEVERITY   );
    info.auto_flush  = get<std::string>(element, XML_ELEMENT_AUTO_FLUSH );
    // clang-format on
    return info;
}

LogXmlNode::Infos LogXmlNode::getLogInfos(Element const & parent)
{
    Infos result;
    Element const * cursor = parent.FirstChildElement(XML_ELEMENT_LOGGER_NAME);
    while (cursor != nullptr) {
        result.push_back(getLogInfo(*cursor));
        cursor = cursor->NextSiblingElement();
    }
    return result;
}

// -----------------------
// Miscellaneous utilities
// -----------------------

int createLoggerWithXmlPath(std::string const & path)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        return 0;
    }
    auto * element = doc.FirstChildElement(LogXmlNode::XML_ELEMENT_LOGGERS_NAME);
    if (element == nullptr) {
        return 0;
    }
    return createLoggerWithXmlLoggerElement(*element);
}

int createLoggerWithXmlText(std::string const & xml)
{
    tinyxml2::XMLDocument doc;
    if (doc.Parse(xml.c_str(), xml.size()) != tinyxml2::XML_SUCCESS) {
        return 0;
    }
    auto * element = doc.FirstChildElement(LogXmlNode::XML_ELEMENT_LOGGERS_NAME);
    if (element == nullptr) {
        return 0;
    }
    return createLoggerWithXmlLoggerElement(*element);
}

int createLoggerWithXmlLoggersElement(tinyxml2::XMLElement const & parent)
{
    auto * element = parent.FirstChildElement(LogXmlNode::XML_ELEMENT_LOGGERS_NAME);
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
        if (createLogger(LogXmlNode::getLogInfo(*cursor)) != nullptr) {
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

