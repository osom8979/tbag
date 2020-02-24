/**
 * @file   LogPref.cpp
 * @brief  LogPref class implementation.
 * @author zer0
 * @date   2017-04-25
 * @date   2019-11-03 (Rename: libtbag/log/node/LogXmlNode -> libtbag/pref/details/LogPref)
 */

#include <libtbag/pref/details/LogPref.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

using LoggerInitParams = LogPref::LoggerInitParams;
using LoggerInitParamsList = LogPref::LoggerInitParamsList;

LogPref::LogPref()
{
    // EMPTY.
}

LogPref::~LogPref()
{
    // EMPTY.
}

std::string LogPref::name() const
{
    return XML_ELEMENT_LOGGERS_NAME;
}

bool LogPref::init()
{
    _params_list.clear();
    return true;
}

void LogPref::clear()
{
    _params_list.clear();
}

void LogPref::load(Element const & element)
{
    _params_list = getInits(element);
}

void LogPref::save(Element & element) const
{
    for (auto & info : _params_list) {
        newElement(element, XML_ELEMENT_LOGGER_NAME, [&](Element & e){
            // clang-format off
            set(e, XML_ELEMENT_NAME      , info.name      );
            set(e, XML_ELEMENT_SINK      , info.sink      );
            set(e, XML_ELEMENT_ARGUMENTS , info.arguments );
            set(e, XML_ELEMENT_GENERATOR , info.generator );
            set(e, XML_ELEMENT_LINE_FEED , info.line_feed );
            set(e, XML_ELEMENT_SEVERITY  , info.severity  );
            set(e, XML_ELEMENT_AUTO_FLUSH, info.auto_flush);
            set(e, XML_ELEMENT_THREAD    , info.thread    );
            // clang-format on
        });
    }
}

LoggerInitParams LogPref::getInit(Element const & element)
{
    LoggerInitParams info;
    // clang-format off
    info.name       = get<std::string>(element, XML_ELEMENT_NAME      );
    info.sink       = get<std::string>(element, XML_ELEMENT_SINK      );
    info.arguments  = get<std::string>(element, XML_ELEMENT_ARGUMENTS );
    info.generator  = get<std::string>(element, XML_ELEMENT_GENERATOR );
    info.line_feed  = get<std::string>(element, XML_ELEMENT_LINE_FEED );
    info.severity   = get<std::string>(element, XML_ELEMENT_SEVERITY  );
    info.auto_flush = get<std::string>(element, XML_ELEMENT_AUTO_FLUSH);
    info.thread     = get<std::string>(element, XML_ELEMENT_THREAD    );
    // clang-format on
    return info;
}

LoggerInitParamsList LogPref::getInits(Element const & parent)
{
    LoggerInitParamsList result;
    Element const * cursor = parent.FirstChildElement(XML_ELEMENT_LOGGER_NAME);
    while (cursor != nullptr) {
        result.push_back(getInit(*cursor));
        cursor = cursor->NextSiblingElement();
    }
    return result;
}

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

