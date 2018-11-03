/**
 * @file   DefaultLogXmlNode.cpp
 * @brief  DefaultLogXmlNode class implementation.
 * @author zer0
 * @date   2017-06-02
 */

#include <libtbag/log/node/DefaultLogXmlNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/Unit.hpp>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

TBAG_CONSTEXPR static char const * const DEFAULT_PARENT_XML_NODE_NAME  = "default_logger";
TBAG_CONSTEXPR static char const * const DEFAULT_LOGGER_FILE_EXTENSION = ".log";

DefaultLogXmlNode::DefaultLogXmlNode()
{
    _params = getDefaultParams();
}

DefaultLogXmlNode::DefaultLogXmlNode(std::string const & name)
{
    _params = getDefaultParams(name, name);
}

DefaultLogXmlNode::DefaultLogXmlNode(std::string const & name, std::string const & file_name)
{
    _params = getDefaultParams(name, file_name);
}

DefaultLogXmlNode::DefaultLogXmlNode(InitParams const & params) : _params(params)
{
    // EMPTY.
}

DefaultLogXmlNode::~DefaultLogXmlNode()
{
    // EMPTY.
}

std::string DefaultLogXmlNode::getLoggerName() const
{
    return _params.name;
}

std::string DefaultLogXmlNode::name() const
{
    return DEFAULT_PARENT_XML_NODE_NAME;
}

void DefaultLogXmlNode::setup()
{
    atInfos().clear();
    atInfos().push_back(getLogInfo(_params.name,
                                   _params.sink,
                                   _params.destination,
                                   _params.max_size,
                                   _params.max_file_count,
                                   _params.auto_flush,
                                   _params.multithread,
                                   _params.mutex,
                                   _params.severity,
                                   _params.generator));
}

DefaultLogXmlNode::InitParams DefaultLogXmlNode::getDefaultParams()
{
    return getDefaultParams(log::TBAG_DEFAULT_LOGGER_NAME, log::TBAG_DEFAULT_LOGGER_NAME);
}

DefaultLogXmlNode::InitParams DefaultLogXmlNode::getDefaultParams(std::string const & name,
                                                                  std::string const & file_name)
{
    InitParams params;
    params.name           = name;
    params.auto_flush     = AUTO_FLUSH_ON;
    params.multithread    = MULTITHREAD_OFF;
    params.mutex          = MUTEX_ON;
    params.max_size       = std::to_string(MEGA_BYTE_TO_BYTE);
    params.max_file_count = std::to_string(DEFAULT_LOG_FILE_COUNT);

    std::stringstream ss;
    ss << "${" << Environments::EXE_DIR << "}" << filesystem::details::PATH_SEPARATOR
       << file_name << DEFAULT_LOGGER_FILE_EXTENSION;
    params.destination = ss.str();

    if (isReleaseMode()) {
        params.sink      = SINK_ROTATE_FILE;
        params.severity  = INFO_SEVERITY.getText();
        params.generator = GENERATOR_DEFAULT;
    } else {
        params.sink      = SINK_STDOUT;
        params.severity  = DEBUG_SEVERITY.getText();
        params.generator = GENERATOR_DEFAULT_COLOR;
    }
    return params;
}

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

