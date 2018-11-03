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
#include <libtbag/dom/xml/XmlModel.hpp>
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
 *
 * @remarks
 *  XML format:
 *  @code{.xml}
 *   <tlog>
 *     <!-- insert your logger -->
 *     <logger>
 *       <!-- Log name -->
 *       <name>your_logger_name</name>
 *
 *       <!-- Select the sink to which you want to send log messages -->
 *       <!--  cout         (Console/Standard output)                -->
 *       <!--  stdout       (== cout)                                -->
 *       <!--  console      (Console output                          -->
 *       <!--  file         (File output                             -->
 *       <!--  rotate_file  (Rotate file output                      -->
 *       <sink>file</sink>
 *
 *       <!-- Specify the detailed path of the sink                 -->
 *       <!--  <sink: cout>        case: Not used                   -->
 *       <!--  <sink: stdout>      case: Not used                   -->
 *       <!--  <sink: console>     case: 'stdout' or 'stderr', if not exists use the 'stdout' [CURRENTLY NOT SUPPORTED] -->
 *       <!--  <sink: file>        case: log file location path     -->
 *       <!--  <sink: rotate_file> case: log file location prefix   -->
 *       <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *       <!-- Special exchange variable:          -->
 *       <!-- ${EXE_PATH} : EXE file path         -->
 *       <!-- ${EXE_NAME} : EXE file name         -->
 *       <!-- ${EXE_DIR}  : EXE directory         -->
 *       <!-- ${WORK_DIR} : Working directory     -->
 *       <!-- ${HOME_DIR} : User's HOME directory -->
 *       <destination>${EXE_DIR}/tbag-logger-test.log</destination>
 *
 *       <!-- File size limit           -->
 *       <!-- [CURRENTLY NOT SUPPORTED] -->
 *       <max_size>1024MByte</max_size>
 *
 *       <!-- Destination size limit    -->
 *       <!-- [CURRENTLY NOT SUPPORTED] -->
 *       <max_file_count>10</max_file_count>
 *
 *       <!-- Multi-thread logger       -->
 *       <!-- [CURRENTLY NOT SUPPORTED] -->
 *       <multithread>false</multithread>
 *
 *       <!-- Use the mutex locking: 'true' or 'false' -->
 *       <mutex>true</mutex>
 *
 *       <!-- Log message generator                                 -->
 *       <!--  default        (Default log message)                 -->
 *       <!--  default_color  (Default+TCES coloring log message)   -->
 *       <!--  raw            (Raw log message)                     -->
 *       <generator>default</generator>
 *
 *       <!-- Log level: 'true' or 'false'                           -->
 *       <!--  level 0: off       (Hide all messages)                -->
 *       <!--  level 1: emergency (System is unusable)               -->
 *       <!--  level 2: alert     (Action must be taken immediately) -->
 *       <!--  level 3: critical  (Critical conditions)              -->
 *       <!--  level 4: error     (Error conditions)                 -->
 *       <!--  level 5: warning   (Warning conditions)               -->
 *       <!--  level 6: notice    (Normal but significant condition) -->
 *       <!--  level 7: info      (Informational messages)           -->
 *       <!--  level 8: debug     (Debug-level messages)             -->
 *       <!--  level 9: unknown   (Unknown level)                    -->
 *       <severity>critical</severity>
 *
 *       <!-- Auto flush flag: 'true' or 'false' -->
 *       <auto_flush>true</auto_flush>
 *     </logger>
 *
 *     <!-- More logger information ... -->
 *   </tlog>
 *  @endcode
 */
class TBAG_API LogXmlNode : public dom::xml::XmlModel::NodeInterface
{
public:
    using Severity = log::level::Severity;
    using MakeType = log::msg::PacketGenerator::MakeType;
    using Logger   = log::mgr::Logger;
    using Element  = dom::xml::XmlModel::Element;

    using Environments = string::Environments;
    using EnvFlag      = Environments::Flag;

    using StringVector = std::vector<std::string>;

public:
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_TLOG_NAME   = "tlog";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_LOGGER_NAME = "logger";

    TBAG_CONSTEXPR static char const * const XML_ELEMENT_NAME           = "name";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SINK           = "sink";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_DESTINATION    = "destination";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MAX_SIZE       = "max_size";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MAX_FILE_COUNT = "max_file_count";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MULTITHREAD    = "multithread";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_MUTEX          = "mutex";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_GENERATOR      = "generator";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SEVERITY       = "severity";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_AUTO_FLUSH     = "auto_flush";

public:
    TBAG_CONSTEXPR static char const * const AUTO_FLUSH_ON  = "true";
    TBAG_CONSTEXPR static char const * const AUTO_FLUSH_OFF = "false";

    TBAG_CONSTEXPR static char const * const MULTITHREAD_ON  = "true";
    TBAG_CONSTEXPR static char const * const MULTITHREAD_OFF = "false";

    TBAG_CONSTEXPR static char const * const MUTEX_ON  = "true";
    TBAG_CONSTEXPR static char const * const MUTEX_OFF = "false";

    TBAG_CONSTEXPR static char const * const GENERATOR_DEFAULT       = "default";
    TBAG_CONSTEXPR static char const * const GENERATOR_DEFAULT_COLOR = "default_color";
    TBAG_CONSTEXPR static char const * const GENERATOR_RAW           = "raw";

    TBAG_CONSTEXPR static char const * const SINK_COUT        = "cout";
    TBAG_CONSTEXPR static char const * const SINK_CONSOLE     = "console";
    TBAG_CONSTEXPR static char const * const SINK_STDOUT      = "stdout";
    TBAG_CONSTEXPR static char const * const SINK_FILE        = "file";
    TBAG_CONSTEXPR static char const * const SINK_ROTATE_FILE = "rotate_file";

public:
    TBAG_CONSTEXPR static char const * const ENVS_EXE_PATH = "EXE_PATH";
    TBAG_CONSTEXPR static char const * const ENVS_EXE_NAME = "EXE_NAME";
    TBAG_CONSTEXPR static char const * const ENVS_EXE_DIR  = "EXE_DIR";
    TBAG_CONSTEXPR static char const * const ENVS_WORK_DIR = "WORK_DIR";
    TBAG_CONSTEXPR static char const * const ENVS_HOME_DIR = "HOME_DIR";

public:
    struct LogInfo
    {
        std::string name;
        std::string sink;
        std::string destination;
        std::size_t max_size;
        std::size_t max_file_count;
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

public:
    // @formatter:off
    inline LogInfoVector       & atInfos()       TBAG_NOEXCEPT { return _infos; }
    inline LogInfoVector const & atInfos() const TBAG_NOEXCEPT { return _infos; }
    inline Environments        & atEnvs ()       TBAG_NOEXCEPT { return _envs;  }
    inline Environments  const & atEnvs () const TBAG_NOEXCEPT { return _envs;  }
    // @formatter:on

protected:
    virtual std::string name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    /**
     * Create loggers.
     *
     * @return The number of created loggers.
     */
    int createLoggers();
    int createLoggers(std::string & preview_message);

    /**
     * Remove loggers.
     *
     * @return The number of deleted loggers.
     */
    int removeLoggers();

public:
    StringVector getNames() const;

public:
    // @formatter:off
    static std::size_t parseSize         (std::string const & value);
    static bool        parseAutoFlush    (std::string const & value);
    static bool        parseMultiThread  (std::string const & value);
    static bool        parseMutex        (std::string const & value);
    static Severity    parseSeverity     (std::string const & value);
    static MakeType    parseGeneratorType(std::string const & value);
    // @formatter:on

public:
    static std::string getElementText(Element const * element);

public:
    static LogInfo getLogInfo(std::string const & name,
                              std::string const & sink,
                              std::string const & destination,
                              std::string const & max_size,
                              std::string const & max_file_count,
                              std::string const & auto_flush,
                              std::string const & multithread,
                              std::string const & mutex,
                              std::string const & severity,
                              std::string const & generator);
    static LogInfo getLogInfo(Element const & element);

public:
    static LogInfoVector loadLogInfo(Element const & element);

public:
    static bool insertName          (Element & parent, std::string const & name);
    static bool insertSink          (Element & parent, std::string const & sink);
    static bool insertDestination   (Element & parent, std::string const & destination);
    static bool insertMaxSize       (Element & parent, std::size_t max_size);
    static bool insertMaxFileCount  (Element & parent, std::size_t max_file_count);
    static bool insertAutoFlush     (Element & parent, bool auto_flush);
    static bool insertMultiThread   (Element & parent, bool multithread);
    static bool insertMutex         (Element & parent, bool mutex);
    static bool insertSeverity      (Element & parent, Severity severity);
    static bool insertGeneratorType (Element & parent, MakeType type);

public:
    static bool saveLogInfo(Element & parent, LogInfo const & info);
    static bool saveLogInfo(Element & parent, LogInfoVector const & infos);

public:
    static Logger * createLogger(LogInfo const & info, bool recreate_if_exists = true);
    static Logger * createLogger(LogInfo const & info, Environments const & envs, bool recreate_if_exists = true);

public:
    static Environments createDefaultEnvironments();
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

