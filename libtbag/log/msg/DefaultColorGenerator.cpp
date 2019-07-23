/**
 * @file   DefaultColorGenerator.cpp
 * @brief  DefaultColorGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultColorGenerator.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/tty/Tces.hpp>

#include <cstring>
#include <thread>
#include <sstream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

DefaultColorGenerator::DefaultColorGenerator(LineFeedStyle line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultColorGenerator::DefaultColorGenerator(std::string const & line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultColorGenerator::~DefaultColorGenerator()
{
    // EMPTY.
}

int DefaultColorGenerator::make(char * buffer, int buffer_size,
                                char const * UNUSED_PARAM(logger),
                                int level, char const * level_name,
                                char const * msg, int msg_size) const
{
    std::stringstream ss;

    using namespace libtbag::tty;
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: /*-------------------------------*/ break;
    case EMERGENCY_LEVEL: ss << DISPLAY_ATTRIBUTE_BG_RED << DISPLAY_ATTRIBUTE_FG_CYAN; break;
    case     ALERT_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_MAGENTA; break;
    case  CRITICAL_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_RED;     break;
    case     ERROR_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_RED;     break;
    case   WARNING_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_YELLOW;  break;
    case    NOTICE_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_GREEN;   break;
    case      INFO_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_GREEN;   break;
    case     DEBUG_LEVEL: ss << DISPLAY_ATTRIBUTE_FG_BLUE;    break;
    case   UNKNOWN_LEVEL: /*-------------------------------*/ break;
    default: /*--------------------------------------------*/ break;
    }
    // clang-format on

    ss << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << " [" << level_name
       << "] " << std::string(msg, msg + msg_size);

    if (EMERGENCY_LEVEL <= COMPARE_AND(level) <= DEBUG_LEVEL) {
        ss << DISPLAY_ATTRIBUTE_RESET;
    }

    if (LINE_FEED == LineFeedStyle::LFS_AUTO) {
        ss << (isWindowsPlatform() ? libtbag::string::WINDOWS_NEW_LINE : libtbag::string::UNIX_NEW_LINE);
    } else if (LINE_FEED == LineFeedStyle::LFS_UNIX) {
        ss << libtbag::string::UNIX_NEW_LINE;
    } else if (LINE_FEED == LineFeedStyle::LFS_WINDOWS) {
        ss << libtbag::string::WINDOWS_NEW_LINE;
    }

    auto const RESULT = ss.str();
    auto const COPY_SIZE = libtbag::algorithm::getMin<int>(buffer_size, RESULT.size());
    ::strncpy(buffer, RESULT.c_str(), COPY_SIZE);
    return COPY_SIZE;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

