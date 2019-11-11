/**
 * @file   LogPref.hpp
 * @brief  LogPref class prototype.
 * @author zer0
 * @date   2017-04-25
 * @date   2019-11-03 (Rename: libtbag/log/node/LogXmlNode -> libtbag/pref/details/LogPref)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_LOGPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_LOGPREF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pref/Preferences.hpp>
#include <libtbag/log/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

/**
 * LogPref class prototype.
 *
 * XML format:
 * @code{.xml}
 *  <loggers>
 *    <!-- insert your logger -->
 *    <logger>
 *      <!-- Logger name -->
 *      <name>your_logger_name</name>
 *
 *      <!-- Select the sink to which you want to send log messages -->
 *      <!--  console     (console output)                          -->
 *      <!--  file        (File output)                             -->
 *      <!--  null        (Null sink)                               -->
 *      <!--  rotate_file (Rotate file sink)                        -->
 *      <sink>rotate_file</sink>
 *
 *      <!-- Specify the detailed path of the sink                             -->
 *      <!--  <sink:console>     : 'stdout' or 'stderr' or 'auto'              -->
 *      <!--  <sink:file>        : log file location path                      -->
 *      <!--  <sink:rotate_file> : 'size=10240m counter=? time=? archive=.zip' -->
 *      <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *      <!-- Special exchange variable:          -->
 *      <!-- ${EXE_PATH} : EXE file path         -->
 *      <!-- ${EXE_NAME} : EXE file name         -->
 *      <!-- ${EXE_DIR}  : EXE directory         -->
 *      <!-- ${WORK_DIR} : Working directory     -->
 *      <!-- ${HOME_DIR} : User's HOME directory -->
 *      <arguments>/prefix/${EXE_NAME}-$py$pm$pdT$ph$pi$ps_$pl$pc.log</arguments>
 *
 *      <!-- Log message generator                                 -->
 *      <!--  default        (Default log message)                 -->
 *      <!--  default_color  (Default+TCES coloring log message)   -->
 *      <!--  raw            (Raw log message)                     -->
 *      <generator>default</generator>
 *
 *      <!-- Linefeed style                    -->
 *      <!--  none     : No linefeed.          -->
 *      <!--  unix     : Unix (LF) style.      -->
 *      <!--  windows  : Windows (CRLF) style. -->
 *      <!--  auto     : Auto detection.       -->
 *      <line_feed>default</line_feed>
 *
 *      <!-- Log level                                              -->
 *      <!--  level 0: off       (Hide all messages)                -->
 *      <!--  level 1: emergency (System is unusable)               -->
 *      <!--  level 2: alert     (Action must be taken immediately) -->
 *      <!--  level 3: critical  (Critical conditions)              -->
 *      <!--  level 4: error     (Error conditions)                 -->
 *      <!--  level 5: warning   (Warning conditions)               -->
 *      <!--  level 6: notice    (Normal but significant condition) -->
 *      <!--  level 7: info      (Informational messages)           -->
 *      <!--  level 8: debug     (Debug-level messages)             -->
 *      <!--  level 9: unknown   (Unknown level)                    -->
 *      <severity>critical</severity>
 *
 *      <!-- Auto flush flag: 'true' or 'false' -->
 *      <auto_flush>true</auto_flush>
 *
 *      <!-- Multi-thread flag: 'true' or 'false' -->
 *      <thread>true</thread>
 *    </logger>
 *
 *    <!-- More logger information ... -->
 *  </loggers>
 * @endcode
 *
 * @author zer0
 * @date   2017-04-25
 * @date   2019-07-03 (Refactoring)
 * @date   2019-11-03 (Rename: libtbag/log/node/LogXmlNode -> libtbag/pref/details/LogPref)
 */
class TBAG_API LogPref : public libtbag::pref::Preferences::NodeInterface
{
public:
    using Init = libtbag::log::LoggerInitParams;
    using Inits = std::vector<Init>;

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

public:
    Inits _inits;

public:
    LogPref();
    virtual ~LogPref();

public:
    inline Inits       & inits()       TBAG_NOEXCEPT { return _inits; }
    inline Inits const & inits() const TBAG_NOEXCEPT { return _inits; }

public:
    std::string name() const override;
    bool init() override;
    void clear() override;
    void load(Element const & element) override;
    void save(Element & element) const override;

public:
    Init getInit(Element const & element);
    Inits getInits(Element const & parent);
};

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_LOGPREF_HPP__

