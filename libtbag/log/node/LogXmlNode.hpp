/**
 * @file   LogXmlNode.hpp
 * @brief  LogXmlNode class prototype.
 * @author zer0
 * @date   2017-04-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/XmlModel.hpp>
#include <libtbag/log/level/Severity.hpp>
#include <libtbag/log/msg/PacketGenerator.hpp>
#include <libtbag/log/mgr/Logger.hpp>
#include <libtbag/string/Environments.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

/**
 * LogXmlNode class prototype.
 *
 * @author zer0
 * @date   2017-04-25
 */
class TBAG_API LogXmlNode : public dom::XmlModel::NodeInterface
{
public:
    using Severity = log::level::Severity;
    using MakeType = log::msg::PacketGenerator::MakeType;
    using Logger   = log::mgr::Logger;

    using Element  = dom::XmlModel::Element;
    using String   = dom::XmlModel::NodeInterface::String;

    using Environments = string::Environments;
    using EnvFlag      = Environments::Flag;

public:
    STATIC_ASSERT_CHECK_IS_SAME(String, std::string);

public:
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_TLOG_NAME   = "tlog";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_LOGGER_NAME = "logger";

    TBAG_CONSTEXPR static char const * const XML_ELEMENT_NAME        = "name";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SINK        = "sink";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_DESTINATION = "destination";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MULTITHREAD = "multithread";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MUTEX       = "mutex";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_GENERATOR   = "generator";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SEVERITY    = "severity";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_AUTO_FLUSH  = "auto_flush";

public:
    TBAG_CONSTEXPR static char const * const AUTO_FLUSH_ON  = "true";
    TBAG_CONSTEXPR static char const * const AUTO_FLUSH_OFF = "false";

    TBAG_CONSTEXPR static char const * const MULTITHREAD_ON  = "true";
    TBAG_CONSTEXPR static char const * const MULTITHREAD_OFF = "false";

    TBAG_CONSTEXPR static char const * const MUTEX_ON  = "true";
    TBAG_CONSTEXPR static char const * const MUTEX_OFF = "false";

    TBAG_CONSTEXPR static char const * const GENERATOR_DEFAULT = "default";
    TBAG_CONSTEXPR static char const * const GENERATOR_DEFAULT_COLOR = "default_color";

    TBAG_CONSTEXPR static char const * const SINK_COUT = "cout";
    TBAG_CONSTEXPR static char const * const SINK_FILE = "file";
    TBAG_CONSTEXPR static char const * const SINK_ROTATE_FILE = "rotate_file";

public:
    TBAG_CONSTEXPR static char const * const ENVS_EXE_PATH = "EXE_PATH";
    TBAG_CONSTEXPR static char const * const ENVS_EXE_DIR  = "EXE_DIR";
    TBAG_CONSTEXPR static char const * const ENVS_WORK_DIR = "WORK_DIR";
    TBAG_CONSTEXPR static char const * const ENVS_HOME_DIR = "HOME_DIR";

public:
    struct LogInfo
    {
        String name;
        String sink;
        String destination;
        bool auto_flush;
        bool multithread;
        bool mutex;
        Severity severity;
        MakeType generator;
    };

    using LogInfoVector = std::vector<LogInfo>;

private:
    LogInfoVector _infos;
    Environments  _envs;

public:
    LogXmlNode();
    virtual ~LogXmlNode();

protected:
    virtual String name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    // @formatter:off
    static bool     parseAutoFlush    (String const & value);
    static bool     parseMultiThread  (String const & value);
    static bool     parseMutex        (String const & value);
    static Severity parseSeverity     (String const & value);
    static MakeType parseGeneratorType(String const & value);
    // @formatter:on

public:
    static String getElementText(Element const * element);

public:
    static LogInfo getLogInfo(String const & name,
                              String const & sink,
                              String const & destination,
                              String const & auto_flush,
                              String const & multithread,
                              String const & mutex,
                              String const & severity,
                              String const & generator);
    static LogInfo getLogInfo(Element const & element);

public:
    static LogInfoVector loadLogInfo(Element const & element);

public:
    static bool insertName          (Element & parent, String const & name);
    static bool insertSink          (Element & parent, String const & sink);
    static bool insertDestination   (Element & parent, String const & destination);
    static bool insertAutoFlush     (Element & parent, bool auto_flush);
    static bool insertMultiThread   (Element & parent, bool multithread);
    static bool insertMutex         (Element & parent, bool mutex);
    static bool insertSeverity      (Element & parent, Severity severity);
    static bool insertGeneratorType (Element & parent, MakeType type);

public:
    static bool saveLogInfo(Element & parent, LogInfo const & info);
    static bool saveLogInfo(Element & parent, LogInfoVector const & infos);

public:
    static Logger * createLogger(LogInfo const & info);
    static Logger * createLogger(LogInfo const & info, Environments const & envs);
};

TBAG_API int createLoggerWithXmlConfigPath(std::string const & path);
TBAG_API int createLoggerWithXmlString(std::string const & xml);
TBAG_API int createLoggerWithXmlElement(tinyxml2::XMLElement const & parent);
TBAG_API int createLoggerWithXmlLoggerElement(tinyxml2::XMLElement const & element);

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__

