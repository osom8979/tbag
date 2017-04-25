/**
 * @file   LogXmlNode.cpp
 * @brief  LogXmlNode class implementation.
 * @author zer0
 * @date   2017-04-25
 */

#include <libtbag/log/node/LogXmlNode.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

LogXmlNode::LogXmlNode()
{
    // EMPTY.
}

LogXmlNode::~LogXmlNode()
{
    // EMPTY.
}

LogXmlNode::String LogXmlNode::name() const
{
    return TBAG_LOGGERS_XML_NODE_NAME;
}

void LogXmlNode::setup()
{
    // EMPTY.
}

void LogXmlNode::teardown()
{
    // EMPTY.
}

void LogXmlNode::load(Element const & element)
{
    createLoggerWithXmlElement(&element);
}

void LogXmlNode::save(Element & element) const
{
    // EMPTY.
}

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

