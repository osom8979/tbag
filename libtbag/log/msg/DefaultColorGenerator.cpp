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

inline static char const * __get_color_prefix(int level) TBAG_NOEXCEPT
{
#ifndef _EMERGENCY_ATTRIBUTE_BG
#define _EMERGENCY_ATTRIBUTE_BG \
    TBAG_TTY_DISPLAY_ATTRIBUTE_BG_RED \
    TBAG_TTY_DISPLAY_ATTRIBUTE_FG_CYAN
#endif
    using namespace libtbag::tty;
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return "";
    case EMERGENCY_LEVEL: return _EMERGENCY_ATTRIBUTE_BG;
    case     ALERT_LEVEL: return DISPLAY_ATTRIBUTE_FG_MAGENTA;
    case  CRITICAL_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case     ERROR_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case   WARNING_LEVEL: return DISPLAY_ATTRIBUTE_FG_YELLOW;
    case    NOTICE_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case      INFO_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case     DEBUG_LEVEL: return DISPLAY_ATTRIBUTE_FG_BLUE;
    case   UNKNOWN_LEVEL: return "";
    default: /*--------*/ return "";
    }
    // clang-format on
#undef _EMERGENCY_ATTRIBUTE_BG
}

int DefaultColorGenerator::make(char * buffer, int buffer_size,
                                char const * UNUSED_PARAM(logger),
                                int level, char const * UNUSED_PARAM(level_name),
                                char const * msg, int msg_size) const
{
    std::stringstream ss;

    ss << __get_color_prefix(level) << getShortPrefix(level)
       << ' ' << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << ' ' << std::string(msg, msg + msg_size);

    if (EMERGENCY_LEVEL <= COMPARE_AND(level) <= DEBUG_LEVEL) {
        ss << libtbag::tty::DISPLAY_ATTRIBUTE_RESET;
    }

    if (LINE_FEED == LineFeedStyle::LFS_AUTO) {
        ss << (isWindowsPlatform() ? libtbag::string::WINDOWS_NEW_LINE : libtbag::string::UNIX_NEW_LINE);
    } else if (LINE_FEED == LineFeedStyle::LFS_UNIX) {
        ss << libtbag::string::UNIX_NEW_LINE;
    } else if (LINE_FEED == LineFeedStyle::LFS_WINDOWS) {
        ss << libtbag::string::WINDOWS_NEW_LINE;
    }

    auto const result_message = ss.str();
    auto const result_message_size = result_message.size();
    if (result_message_size <= buffer_size) {
        ::strncpy(buffer, result_message.c_str(), result_message_size);
        return result_message_size;
    }

    COMMENT("Explicit marking if message is cut off.") {
        ::strncpy(buffer, result_message.c_str(), buffer_size-4);
        buffer[buffer_size-4] = ' ';
        buffer[buffer_size-3] = '.';
        buffer[buffer_size-2] = '.';
        buffer[buffer_size-1] = '.';
    }
    assert(result_message_size > buffer_size);
    return buffer_size;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

