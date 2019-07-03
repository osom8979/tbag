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
    if (level <= EMERGENCY_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_BG_RED << tces::DISPLAY_ATTRIBUTE_FG_CYAN;
    } else if (level <= ALERT_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_MAGENTA;
    } else if (level <= CRITICAL_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= ERROR_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= WARNING_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
    } else if (level <= NOTICE_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= INFO_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= DEBUG_LEVEL) {
        ss << libtbag::tces::DISPLAY_ATTRIBUTE_FG_BLUE;
    }

    ss << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << " [" << level_name
       << "] " << std::string(msg, msg + msg_size)
       << libtbag::tces::DISPLAY_ATTRIBUTE_RESET;

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

