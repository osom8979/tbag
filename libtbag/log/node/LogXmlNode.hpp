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
#include <libtbag/string/Environments.hpp>
#include <libtbag/log/LoggerManager.hpp>

#include <vector>
#include <string>

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
 * @date   2019-07-03 (Refactoring)
 *
 * @remarks
 *  XML format:
 *  @code{.xml}
 *   <loggers>
 *     <!-- insert your logger -->
 *     <logger>
 *       <!-- Log name -->
 *       <name>your_logger_name</name>
 *
 *       <!-- Select the sink to which you want to send log messages -->
 *       <!--  console      (console output)                         -->
 *       <!--  file         (File output)                            -->
 *       <!--  null         (Null sink)                              -->
 *       <sink>file</sink>
 *
 *       <!-- Specify the detailed path of the sink                 -->
 *       <!--  <sink: console>     case: 'stdout' or 'stderr'       -->
 *       <!--  <sink: file>        case: log file location path     -->
 *       <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *       <!-- Special exchange variable:          -->
 *       <!-- ${EXE_PATH} : EXE file path         -->
 *       <!-- ${EXE_NAME} : EXE file name         -->
 *       <!-- ${EXE_DIR}  : EXE directory         -->
 *       <!-- ${WORK_DIR} : Working directory     -->
 *       <!-- ${HOME_DIR} : User's HOME directory -->
 *       <arguments>${EXE_DIR}/tbag-logger-test.log</arguments>
 *
 *       <!-- Log message generator                                 -->
 *       <!--  default        (Default log message)                 -->
 *       <!--  default_color  (Default+TCES coloring log message)   -->
 *       <!--  raw            (Raw log message)                     -->
 *       <generator>default</generator>
 *
 *       <!-- Linefeed style                    -->
 *       <!--  none     : No linefeed.          -->
 *       <!--  unix     : Unix (LF) style.      -->
 *       <!--  windows  : Windows (CRLF) style. -->
 *       <!--  auto     : Auto detection.       -->
 *       <line_feed>default</line_feed>
 *
 *       <!-- Log level                                              -->
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
 *   </loggers>
 *  @endcode
 */
class TBAG_API LogXmlNode : public libtbag::dom::xml::XmlModel::NodeInterface
{
public:
    using Info = libtbag::log::LoggerInitParams;
    using Infos = std::vector<LoggerInitParams>;
    using Environments = libtbag::string::Environments;

public:
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_LOGGERS_NAME = "loggers";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_LOGGER_NAME  = "logger";

    TBAG_CONSTEXPR static char const * const XML_ELEMENT_NAME       = "name";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SINK       = "sink";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_ARGUMENTS  = "arguments";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_GENERATOR  = "generator";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_LINE_FEED  = "line_feed";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_SEVERITY   = "severity";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_AUTO_FLUSH = "auto_flush";
    TBAG_CONSTEXPR static char const * const XML_ELEMENT_THREAD     = "thread";

private:
    Environments _envs;
    Infos _infos;

public:
    LogXmlNode();
    virtual ~LogXmlNode();

protected:
    virtual std::string name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    inline Environments       & envs()       TBAG_NOEXCEPT { return _envs; }
    inline Environments const & envs() const TBAG_NOEXCEPT { return _envs; }

    inline Infos       & infos()       TBAG_NOEXCEPT { return _infos; }
    inline Infos const & infos() const TBAG_NOEXCEPT { return _infos; }

public:
    std::vector<std::string> getInfoNames() const;

public:
    /**
     * Create loggers.
     *
     * @return The number of created loggers.
     */
    int createLoggers();
    int createLoggers(std::string & preview_message);

public:
    static Info getLogInfo(Element const & element);
    static Infos getLogInfos(Element const & element);
};

// -----------------------
// Miscellaneous utilities
// -----------------------

TBAG_API int createLoggerWithXmlFile(std::string const & path);
TBAG_API int createLoggerWithXmlFile(std::string const & path, libtbag::string::Environments const & envs);

TBAG_API int createLoggerWithXmlText(std::string const & xml);
TBAG_API int createLoggerWithXmlText(std::string const & xml, libtbag::string::Environments const & envs);

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__

